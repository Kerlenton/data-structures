#define LIMIT 255

double stack[LIMIT];
unsigned char count = 0;

void push(int num)
{
	if (count != LIMIT)
		stack[count++] = num;
	else
		printf("Stack is full");
}

int pop(void)
{
	if (count != 0)
		return stack[--count];
	else
		printf("Stack is void");
}
