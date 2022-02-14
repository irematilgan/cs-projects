#include <stdio.h>
#include <stdlib.h>

typedef struct node {
	struct node* next;
	int val;
}node;



node* createNode(int val) {
	node* newNode = (node*)malloc(sizeof(node));
	newNode->next =NULL;
	newNode->val = val;
	
	return newNode;
}

int insert(node** hash, int key, int m) {
	int i = 0;
	int adr = key%m;
	printf("deneme\n");

	if(hash[adr] == NULL) {
		hash[adr] = (node*)malloc(sizeof(node));
		hash[adr]->next = createNode(key);
	
		printf("1\n");
		
	} else {
		node* tmp = hash[adr]->next;
		hash[adr]->next = createNode(key);
		hash[adr]->next->next = tmp;
	
	}
	printf("\n2\n");
	return 1;
	
}



void printList(node** hash,int m) {
	int i;
	node* tmp;
	for(i = 0; i < m; i++) {
		if(hash[i] == NULL)
			printf("NULL\t");
		else {
			printf("%d\t",hash[i]->val);
			tmp = hash[i]->next;
			while(tmp != NULL) {
				printf("%d\t",tmp->val);
				tmp = tmp->next;
			}
		}
		

		printf("\n");
	}
	
	
}

int main()
{
	int key1 = 8, key2 = 13;
	int i;
	node** closedArr = (node**)malloc(sizeof(int*)*5);
	
	for(i = 0; i < 5; i++) {
		closedArr[i] = NULL;
	}
		
	printf("aa\n");
	if(insert(closedArr,key1,5) && insert(closedArr,key2,5) && insert(closedArr,23,5))
		printf("Yerlestirme Basarili!..\n");
	
	
	printf("aa\n");
	printList(closedArr,5);
	
	
	return 0;
}
