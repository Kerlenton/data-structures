#include <stdio.h>
#include <stdlib.h>

typedef struct node
{
	int data;
	struct node* next;
}node_t;

node_t* create_node(int data)
{
	node_t* head = (node_t*)malloc(sizeof(node_t));
	head->data = data;
	head->next = NULL;

	return head;
}

void print_list(node_t* head)
{
	printf("[ ");

	while (head != NULL)
	{
		printf("%d ", head->data);
		head = head->next;
	}
	printf("]\n");
		
}

void push_end(node_t* head, int data)
{
	while (head->next != NULL)
		head = head->next;

	head->next = (node_t*)malloc(sizeof(node_t));
	head->next->data = data;
	head->next->next = NULL;
}

void push_begin(node_t** head, int data)
{
	node_t* new_node = (node_t*)malloc(sizeof(node_t));

	new_node->data = data;
	new_node->next = *head;
	*head = new_node;
}

void delete_begin(node_t** head)
{
	node_t* temp;

	if (*head == NULL)
		return -1;

	temp = (*head)->next;
	free(*head);
	*head = temp;
}

void delete_end(node_t* head)
{
	while (head->next->next != NULL)
		head = head->next;

	free(head->next);
	head->next = NULL;
}

void delete_some(node_t** head, int number)
{
	int i;
	node_t* temp;

	if (number = 0)
		return delete_begin(*head);

	for (i = 0; i < n - 1; i++)
	{
		if ((*head)->next == NULL)
			return -1;
		*head = (*head)->next;
	}

	temp = (*head)->next;
	(*head)->next = temp->next;
	free(temp); 
}

int pop_begin(node_t* head)
{
	int retval;

	if (head == NULL)
		return -1;

	retval = head->data;
	
	return retval;
}

int pop_end(node_t* head)
{
	int retval;

	while (head->next != NULL)
		head = head->next;

	retval = head->data;

	return retval;
}

int pop_some(node_t* head, int number)
{
	int i;
	int retval;

	if (number == 0)
		return head->data;

	for (i = 0; i < number - 1; i++)
	{
		if (head->next = NULL)
			return -1;
		head = head->next;
	}

	return head->data;
}