#include "type.h"
#include "stack.h"

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

static void _insert_stack(Stack *stack, size_t index, void *value);
static void _free_stack(Stack *stack);

int main(void)
{
    Stack *stack = new_stack(3, STRING_TYPE);
    push_stack(stack, string("hello"));
    push_stack(stack, string("bye"));
    value_t t = pop_stack(stack);

    printf("%s", t.string);
    //printf("%s", pop_stack(stack));

    return 0;
}

typedef struct Stack
{
   vtype_t tvalue;
   value_t *buffer;
   size_t size;
   size_t index;
} Stack;

extern Stack *new_stack(size_t size, vtype_t value)
{
   switch(value)
   {
      case DECIMAL_TYPE: case REAL_TYPE: case STRING_TYPE:
      break;

      default:
      fprintf(stderr, "%s\n", "value type not fount");
      return NULL;
   }

   Stack *stack = (Stack*)malloc(sizeof(Stack));
   stack->buffer = (value_t*)malloc(sizeof(value_t)*size);
   stack->size = size;
   stack->index = 0;

   return stack;
}

extern void free_stack(Stack *stack)
{
   _free_stack(stack);
   free(stack->buffer);
   free(stack);
}

extern void set_stack(Stack *stack, size_t index, void *value)
{
   _insert_stack(stack, index, value);
}

extern void push_stack(Stack *stack, void *value)
{
   _insert_stack(stack, stack->index, value);
   stack->index += 1;
}

extern value_t get_stack(Stack *stack, size_t index)
{
   if (index >= stack->size)
   {
      fprintf(stderr, "%s\n", "index >= stack->size");
      value_t none = { .decimal = 0 };

      return none;
   }

   return stack->buffer[index];
}

extern value_t pop_stack(Stack *stack)
{
    if (stack->index = 0)
    {
       fprintf(stderr, "%s\n", "stack overflow");
       value_t none = { .decimal = 0 };

       return none;
    }

    stack->index -= 1;
    return stack->buffer[stack->index];
}

static void _insert_stack(Stack *stack, size_t index, void *value)
{
     if (index >= stack->size)
   {
      fprintf(stderr, "%s\n", "index >= stack->size");
      return;
   }

   switch (stack->tvalue)
   {
      case DECIMAL_TYPE:
          stack->buffer[index].decimal = (int32_t)(intptr_t)value;
      break;

      case REAL_TYPE:
          stack->buffer[index].real = *(double*)value;
          free((double*) value);
      break;

      case STRING_TYPE:
      {
          size_t size = strlen((char*)value);
          stack->buffer[index].string = (char*)malloc(sizeof(char)*size-1);
          strcpy(stack->buffer[index].string, (char*)value);
      }
      break;

      default: ;
   }
}

static void _free_stack(Stack *stack)
{
   switch(stack->tvalue)
   {
       case STRING_TYPE:
            for(size_t i = 0; i < stack->index; i++)
               free(stack->buffer[i].string);
       break;

       default: ;
   }
}














