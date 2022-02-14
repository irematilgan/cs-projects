#include <stdio.h>
#include <stdlib.h>

typedef struct Node{
	struct Node* next;
	int val;
}NODE;

NODE* createNode(int val)
{
	NODE* node = malloc(sizeof(NODE));
	node->next = NULL;
	node->val = val;
	
	return node;
}

NODE* reverse(NODE* root)
{
	NODE *current, *prev, *tmp;
	current = root->next;
	prev = root;
	root->next = NULL;
	
	while(current != NULL)
	{
		tmp = current->next;
		current->next = prev;
		prev = current;
		current = tmp;
	}
	
	return prev;
}

void printLinked(NODE* root)
{
	NODE* current;
	
	for(current = root; current != NULL; current = current->next)
		printf("%d\t",current->val);
}


int main()
{
	NODE* root;
	root = createNode(3);
	root->next = createNode(5);
	root->next->next = createNode(9);
	
	NODE* newRoot = reverse(root);
	
	printLinked(newRoot);
	
	return 0;
}
