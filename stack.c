#include <stdio.h>
#include <stdint.h>
#define LIMIT 255

typedef struct 
{
	uint8_t** arr;
	unsigned char count;
}stack_t;

stack_t* newStack()
{
	stack_t* stack = (stack_t*)malloc(sizeof(stack_t));
	stack->arr = (uint8_t**)malloc(LIMIT);
	stack->count = 0;

	return stack;
}

void push(stack_t* stack, uint8_t* data)
{
	if (stack->count != LIMIT)
		stack->arr[stack->count++] = data;
	else
		fprintf(stderr, "ERROR: Stack is full");
}

uint8_t* pop(stack_t* stack)
{
	if (stack->count != 0)
		return stack->arr[--stack->count];
	else
	{
		fprintf(stderr, "ERROR: Stack is void!");
		return 0;
	}
}
