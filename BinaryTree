#include <stdlib.h>
#include <stdio.h>

typedef struct node
{
	int data;
	struct node* left;
	struct node* right;
}bin_tr;

bin_tr* create_tree(int data)
{
	bin_tr* node = (bin_tr*)malloc(sizeof(bin_tr));

	node->data = data;
	node->left = NULL;
	node->right = NULL;
}

static void static_printin(bin_tr* tree)
{
	if (tree->left) static_printin(tree->left);
	printf("%d ", tree->data);
	if (tree->right) static_printin(tree->right);
}

static void static_printpre(bin_tr* tree)
{
	printf("%d ", tree->data);
	if (tree->left) static_printpre(tree->left);
	if (tree->right) static_printpre(tree->right);
}

static void static_printpost(bin_tr* tree)
{
	if (tree->left) static_printpost(tree->left);
	if (tree->right) static_printpost(tree->right);
	printf("%d ", tree->data);
}

void printin(bin_tr* tree)
{
	printf("[ ");
	static_printin(tree);
	printf("]\n");
}

void printpre(bin_tr* tree)
{
	printf("[ ");
	static_printpre(tree);
	printf("]\n");
}

void printpost(bin_tr* tree)
{
	printf("[ ");
	static_printpost(tree);
	printf("]\n");
}

void insert(bin_tr** root, bin_tr* child)
{
	if (!*root) *root = child;
	else insert(child->data <= (*root)->data ? &(*root)->left : &(*root)->right, child);
}

bin_tr* search(bin_tr* root, int data)
{
	return !root ? -1 : root->data == data ? root : search(data > root->data ? root->right : root->left, data);
}

void destroy_tree(bin_tr* root)
{
	if (!root)
	{
		destroy_tree(root->left);
		destroy_tree(root->right);
		free(root);
	}
}

main()
{
	bin_tr* node = create_tree(5);
	bin_tr* node2 = create_tree(1);

	insert(&node, node2);
	bin_tr* v = search(node, 5);
	system("PAUSE");
	return 0;
}
