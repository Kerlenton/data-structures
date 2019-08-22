#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <math.h>

#define HT_PRIME_1 151
#define HT_PRIME_2 163
#define INITIAL_BASE_SIZE 50 

typedef struct item
{
	uint8_t* data;
	uint8_t* key;
}ht_item;

typedef struct table
{
	int base_size;
	int size;
	int count;
	ht_item** items;
}ht_hash_table;


static ht_item HT_DELETED_ITEM = { NULL, NULL };
static ht_item* ht_new_item(const uint8_t* k, const uint8_t* d);
static ht_hash_table* ht_new_sized(const int base_size);
ht_hash_table* ht_new();
static void ht_del_item(ht_item* i);
void ht_del(ht_hash_table* ht);
static int ht_hash(const uint8_t* s, const int a, const int m);
static ht_get_hash(const uint8_t* s, const int m, const int attempt);
static void ht_resize(ht_hash_table* ht, const int base_size);
static void ht_resize_up(ht_hash_table* ht);
static void ht_resize_down(ht_hash_table* ht);
char* ht_search(ht_hash_table* ht, const uint8_t* key);
void ht_insert(ht_hash_table* ht, const uint8_t* key, const uint8_t* data);
void ht_delete(ht_hash_table* ht, const uint8_t* key);
int is_prime(const int x);
int next_prime(int x);


static ht_item* ht_new_item(const uint8_t* k, const uint8_t* d)
{
	ht_item* i = (ht_item*)malloc(sizeof(ht_item));

	i->key = k;
	i->data = d;

	return i;
}

static ht_hash_table* ht_new_sized(const int base_size)
{
	ht_hash_table* ht = malloc(sizeof(ht_hash_table));
	ht->base_size = base_size;
	ht->size = next_prime(ht->base_size);
	ht->count = 0;
	ht->items = calloc((size_t)ht->size, sizeof(ht_item*));

	return ht;
}

ht_hash_table* ht_new()
{
	return ht_new_sized(INITIAL_BASE_SIZE);
}

static void ht_del_item(ht_item* i)
{
	free(i->key);
	free(i->data);
	free(i);
}

void ht_del(ht_hash_table* ht)
{
	int i;

	for (i = 0; i < ht->size; i++)
	{
		ht_item* item = ht->items[i];

		if (item != NULL)
			ht_del_item(item);
	}

	free(ht->items);
	free(ht);
}

static int ht_hash(const uint8_t* s, const int a, const int m)
{
	int i;
	long hash = 0;
	const int len_s = strlen(s);

	for (i = 0; i < len_s; i++)
	{
		hash += (long)pow(a, len_s - (i + 1)) * s[i];
		hash = hash % m;
	}

	return (int)hash;
}

static ht_get_hash(const uint8_t* s, const int m, const int attempt)
{
	const int hash_a = ht_hash(s, HT_PRIME_1, m);
	const int hash_b = ht_hash(s, HT_PRIME_2, m);

	return (hash_a + (attempt * (hash_b + 1))) % m;
}

static void ht_resize(ht_hash_table* ht, const int base_size)
{
	int i;

	if (base_size < INITIAL_BASE_SIZE)
		return;
	ht_hash_table* new_ht = ht_new_sized(base_size);

	for (i = 0; i < ht->size; i++)
	{
		ht_item* item = ht->items[i];
		if (item != NULL && item != &HT_DELETED_ITEM)
			ht_insert(new_ht, item->key, item->data);
	}

	const int temp_size = ht->size;
	ht->size = new_ht->size;
	new_ht->size = temp_size;

	ht_item** temp_items = ht->items;
	ht->items = new_ht->items;
	new_ht->items = temp_items;

	ht_del(new_ht);
}

static void ht_resize_up(ht_hash_table* ht)
{
	const int new_size = ht->base_size * 2;
	ht_resize(ht, new_size);
}

static void ht_resize_down(ht_hash_table* ht)
{
	const int new_size = ht->base_size / 2;
	ht_resize(ht, new_size);
}


char* ht_search(ht_hash_table* ht, const uint8_t* key)
{
	int index = ht_get_hash(key, ht->size, 0);
	ht_item* item = ht->items[index];
	int i = 1;

	while (item != NULL)
	{
		if (item != &HT_DELETED_ITEM)
			if (strcmp(item->key, key) == 0)
				return item->data;
		index = ht_get_hash(key, ht->size, i);
		item = ht->items[index];
		i++;
	}

	return -1;
}

void ht_insert(ht_hash_table* ht, const uint8_t* key, const uint8_t* data)
{
	const int load = ht->count * 100 / ht->size;

	if (load > 70)
	{
		ht_resize_up(ht);
	}
	ht_item* item = ht_new_item(key, data);
	int index = ht_get_hash(item->key, ht->size, 0);
	ht_item* cur_item = ht->items[index];
	int i = 1;

	while (cur_item != NULL)
	{
		if (cur_item != &HT_DELETED_ITEM)
			if (strcmp(cur_item->key, key) == 0)
			{
				ht_del_item(cur_item);
				ht->items[index] = item;
				return;
			}
		index = ht_get_hash(item->key, ht->size, i);
		cur_item = ht->items[index];
		i++;
	}

	ht->items[index] = cur_item;
	ht->count++;
}

void ht_delete(ht_hash_table* ht, const uint8_t* key)
{
	const int load = ht->count * 100 / ht->size;

	if (load < 10)
		ht_resize_down(ht);
	int index = ht_get_hash(key, ht->size, 0);
	ht_item* item = ht->items[index];
	int i = 0;

	while (item != NULL)
	{
		if (item != &HT_DELETED_ITEM)
			if (strcmp(item->key, key) == 0)
			{
				ht_del_item(item);
				ht->items[index] = &HT_DELETED_ITEM;
			}
		index = ht_get_hash(key, ht->size, i);
		item = ht->items[index];
		i++;
	}

	ht->count--;
}

int is_prime(const int x)
{
	int i;

	if (x < 2) { return -1; }
	if (x < 4) { return 1; }
	if ((x % 2) == 0) { return 0; }

	for (i = 3; i <= floor(sqrt((double)x)); i += 2)
		if ((x % i) == 0)
			return 0;
	return 1;
}

int next_prime(int x)
{
	while (is_prime(x) != 1)
		x++;
	return x;
}
