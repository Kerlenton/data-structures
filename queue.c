#include <stdio.h>
#include <stdint.h>
#define LIMIT 255

typedef struct
{
	uint8_t** arr;
	unsigned char icount;
	unsigned char jcount;
}queue_t;

queue_t* newQueue()
{
	queue_t* queue = (queue_t*)malloc(sizeof(queue_t));
	queue->arr = (uint8_t**)malloc(LIMIT);
	queue->icount = 0;
	queue->jcount = LIMIT;

	return queue;
}

void push(queue_t* queue, uint8_t* data)
{
	if (queue->icount != LIMIT)
	{
		--queue->jcount;
		queue->arr[queue->icount++] = data;
	}
	else
		fprintf(stderr, "ERROR: Queue is full");
}

uint8_t* pop(queue_t* queue)
{
	if (queue->icount != 0)
	{
		return queue->arr[LIMIT - ((queue->jcount++) + (queue->icount--))];
	}
	else
	{
		fprintf(stderr, "ERROR: Queue is void");
		return 0;
	}
}
