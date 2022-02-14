#include <stdio.h>
#include <stdlib.h>
#define SIZE 23

typedef struct node{
	struct node* next;
	int key;
	int val;
}node;

node* createNode(int val)
{
	node* newNode = (node*)malloc(sizeof(node));
	newNode->val = val;
	newNode->next = NULL;
	return newNode;
}

int hashed(int element)
{
	element = element % SIZE;
	return element;
}

int insertTable(node* hashT, int element)
{
	int i;
	int key;
	node* tmp;
	key = hashed(element);

	tmp = hashT[key];
	if(tmp == NULL)
		tmp = createNode(element);
	else
	{
		while(tmp->next != NULL && tmp->val != element)
		{
			tmp = tmp->next;
		}
		if(tmp != NULL)
			return -1;
		else
		{
			tmp->next = createNode(element);
			
		}
	}
	return 1;
	
}

int main()
{
	node* hashTable = (node*)malloc(sizeof(node)*23);
	
	
	return 0;
}
