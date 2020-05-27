#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

typedef enum
{
	DECIMAL_ELEM,
	REAL_ELEM,
	STRING_ELEM
}vtype_tree_t;

typedef union
{
	int64_t decimal;
	double real;
	uint8_t *string;
} value_tree_t;

typedef struct tree_node
{
	struct
	{
		value_tree_t key;
		value_tree_t value;
	} data;
	struct tree_node *left;
	struct tree_node *right;
	struct tree_node *parent;
} tree_node;

typedef struct Tree
{
	struct 
	{
		vtype_tree_t key;
		vtype_tree_t value;
	} type;
	struct tree_node *node;
} Tree;

extern Tree *new_tree(vtype_tree_t key, vtype_tree_t value);
extern void free_tree(Tree *tree);

extern value_tree_t get_tree(Tree *tree, void *key);
extern void set_tree(Tree *tree, void *key, void *value);
extern value_tree_t del_tree(Tree *tree, void *key);
extern _Bool in_tree(Tree *tree, void *key);

extern void *decimal(int64_t x);
extern void *real(double x);
extern void *string(uint8_t x);

extern void print_tree(Tree *tree);
extern void print_tree_as_list(Tree *tree);
static void _print_tree(tree_node *node, vtype_tree_t tkey, vtype_tree_t tvalue);
static void _print_tree_as_list(tree_node *node, vtype_tree_t tkey, vtype_tree_t tvalue);
static void _print_tree_elem(tree_node *node, vtype_tree_t tkey, vtype_tree_t tvalue);

static void _set_tree(tree_node *node, vtype_tree_t tkey, vtype_tree_t tvalue, void *key, void *value);
static void _free_tree(tree_node *node);

static tree_node *_new_node(vtype_tree_t tkey, vtype_tree_t tvalue, void *key, void *value);
static void _set_value(tree_node *node, vtype_tree_t tvalue, void *value);
static void _set_key(tree_node *node, vtype_tree_t tkey, void *key);

main()
{
	Tree *tree = new_tree(DECIMAL_ELEM, STRING_ELEM);

	set_tree(tree, decimal(50), string("A"));
	set_tree(tree, decimal(40), string("b"));
	set_tree(tree, decimal(80), string("c"));

	print_tree(tree);
	free_tree(tree);

	system("PAUSE");

	return 0;
}

extern Tree *new_tree(vtype_tree_t key, vtype_tree_t value)
{
	switch (key)
	{
	case DECIMAL_ELEM: case STRING_ELEM: 
		break;
	default:
		fprintf(stderr, "%s\n", "key type is not supported");
		return NULL;
	}

	switch (value)
	{
	case DECIMAL_ELEM: case STRING_ELEM: case REAL_ELEM:
		break;
	default:
		fprintf(stderr, "%s\n", "value type is not supported");
		return NULL;
	}

	Tree *tree = (Tree*)malloc(sizeof(Tree));
	tree->type.key = key;
	tree->type.value = value;
	tree->node = NULL;

	return tree;
}

extern void set_tree(Tree *tree, void *key, void *value)
{
	if (tree->node == NULL)
	{
		tree->node = _new_node(tree->type.key, tree->type.value, key, value);
		return;
	}
	_set_tree(tree->node, tree->type.key, tree->type.value, key, value);
}

extern void *decimal(int64_t x)
{
	return (void*) x;
}

extern void *real(double x)
{
	double *f = (double*)malloc(sizeof(double));
	*f = x;

	return (void*)f;
}
extern void *string(uint8_t *x)
{
	return (void*) x;
}

extern void print_tree_as_list(Tree *tree)
{
	putchar('[');
	_print_tree_as_list(tree->node, tree->type.key, tree->type.value);
	printf("]\n");
}


extern void print_tree(Tree *tree)
{
	_print_tree(tree->node, tree->type.key, tree->type.value);
	putchar('\n');
}

extern void free_tree(Tree *tree)
{
	_free_tree(tree->node);
	free(tree);
}

static void _free_tree(tree_node *node)
{
	if (node == NULL)
		return;
	_free_tree(node->left);
	_free_tree(node->right);
	free(node);
}

static void _print_tree_as_list(tree_node *node, vtype_tree_t tkey, vtype_tree_t tvalue)
{
	if (node == NULL)
	{
		return;
	}
	_print_tree_as_list(node->left, tkey, tvalue);
	_print_tree_elem(node, tkey, tvalue);
	_print_tree_as_list(node->right, tkey, tvalue);
}


static void _print_tree_elem(tree_node *node, vtype_tree_t tkey, vtype_tree_t tvalue)
{
	switch (tkey)
	{
	case DECIMAL_ELEM:
		printf(" [%d => ", node->data.key.decimal);
		switch (tvalue)
		{
		case DECIMAL_ELEM:
			printf("%d] ", node->data.value.decimal);
			break;

		case REAL_ELEM:
			printf("%lf] ", node->data.value.real);
			break;

		case STRING_ELEM:
			printf("'%s'] ", node->data.value.string);
			break;
		}
		break;

	case STRING_ELEM:
		printf(" ['%s' =>  ", node->data.key.string);
		switch (tvalue)
		{
		case DECIMAL_ELEM:
			printf("%d] ", node->data.value.decimal);
			break;

		case REAL_ELEM:
			printf("%lf] ", node->data.value.real);
			break;

		case STRING_ELEM:
			printf("'%s'] ", node->data.value.string);
			break;
		}
		break;
	}
}

static void _print_tree(tree_node *node, vtype_tree_t tkey, vtype_tree_t tvalue)
{
	if (node == NULL)
	{
		printf("null");
		return;
	}
	putchar('(');
	_print_tree(node->left, tkey, tvalue);
	_print_tree_elem(node, tkey, tvalue);
	_print_tree(node->right, tkey, tvalue);
	putchar(')');
}

static void _set_tree(tree_node *node, vtype_tree_t tkey, vtype_tree_t tvalue, void *key, void *value)
{
	int cond = 0;
	switch (tkey)
	{
	case DECIMAL_ELEM:
		if ((int64_t)key > node->data.key.decimal)
		{
			if (node->right == NULL)
			{
				node->right = _new_node(tkey, tvalue, key, value);
				node->right->parent = node;
			}
			else
				_set_tree(node->right, tkey, tvalue, key, value);
		}

		else if ((int64_t)key < node->data.key.decimal)
		{
			if (node->left == NULL)
			{
				node->left = _new_node(tkey, tvalue, key, value);
				node->left->parent = node;
			}
			else
				_set_tree(node->left, tkey, tvalue, key, value);
		}

		else
		{
			_set_value(node, tvalue, value);
		}
		break;

	case STRING_ELEM:
		cond = strcmp((uint8_t*)key, node->data.key.string);
		if (cond > 0)
		{
			if (node->right == NULL)
			{
				node->right = _new_node(tkey, tvalue, key, value);
				node->right->parent = node;
			}
			else
				_set_tree(node->right, tkey, tvalue, key, value);
		}

		else if (cond < 0)
		{
			if (node->left == NULL)
			{
				node->left = _new_node(tkey, tvalue, key, value);
				node->left->parent = node;
			}
			else
				_set_tree(node->left, tkey, tvalue, key, value);
		}

		break;
	}
}

static tree_node *_new_node(vtype_tree_t tkey, vtype_tree_t tvalue, void *key, void *value)
{
	tree_node *node = (tree_node*)malloc(sizeof(tree_node));
	_set_key(node, tkey, key);
	_set_value(node, tvalue, value);
	node->left = NULL;
	node->right = NULL;
	node->parent = NULL;
}

static void _set_key(tree_node *node, vtype_tree_t tkey, void *key)
{
	switch (tkey)
	{
	case DECIMAL_ELEM:
		node->data.key.decimal = (int64_t)key;
		break;

	case STRING_ELEM:
		node->data.key.string = (uint8_t*)key;
		break;
	}
}

static void _set_value(tree_node *node, vtype_tree_t tvalue, void *value)
{
	switch (tvalue)
	{
	case DECIMAL_ELEM:
		node->data.value.decimal = (int64_t)value;
		break;

	case REAL_ELEM:
		node->data.value.real = *(double*)value;
		free((double*)value);
		break;

	case STRING_ELEM:
		node->data.value.string = (uint8_t*)value;
		break;
	}
}
