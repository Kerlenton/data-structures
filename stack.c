#include <stdint.h>
#define LIMIT 255

unsigned char count = 0;

void push(uint8_t** stack, uint8_t* num)
{
	if (count != LIMIT)
		stack[count++] = num;
	else
		printf("Stack is full");
}

uint8_t* pop(uint8_t** stack)
{
	if (count != 0)
		return stack[--count];
	else
		printf("Stack is void");
}
