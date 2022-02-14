#include <stdio.h>
#include <stdlib.h>

typedef struct Node{
	int val;
	struct Node *left,*right;
}NODE;

NODE* createNode(){
	NODE* node = (NODE*)malloc(sizeof(NODE));
	node->left = node->right = NULL;
	return node;
}

NODE* insert(NODE* root,int value){
	if(root == NULL)
	{
		root = createNode();
		root->val = value;
		return root;
	}
	if(value > root->val)
		root->right = insert(root->right,value);
	else
		root->left = insert(root->left,value);
		
}

NODE* search(NODE* root,int value){
	if(root == NULL || root->val == value)
		return root;
	
	if(value > root->val)
		search(root->right,value);
	else
		search(root->left,value);
}

NODE* findInorderSuccessor(NODE* root)
{
	if(root == NULL || root->left == NULL)
		return root;
	root = findInorderSuccessor(root->left);
}

NODE* deleteNode(NODE* root,int value){
	
	if(root == NULL)
		return root;
	if(root->val > value)
		root->left = deleteNode(root->left,value);
	else if(root->val < value)
		root->right = deleteNode(root->right,value);
	else{
		if(root->left == NULL)
		{
			NODE* tmp = root->right;
			free(root);
			return tmp;
		}
		else if(root->right == NULL)
		{
			NODE* tmp = root->left;
			free(root);
			return tmp;
		}
		
		NODE* tmp = findInorderSuccessor(root->right);
		root->val = tmp->val;
		root->right = deleteNode(root->right,tmp->val);
	}
}

void printPreorder(NODE* root){
	if(root == NULL)
		return;
	printf("%d\t",root->val);
	printPreorder(root->left);
	printPreorder(root->right);
	
}


int main()
{
	NODE* root = NULL;
	root = insert(root,25);
	insert(root,19);
	insert(root,36);
	insert(root,23); 
	insert(root,28);
	insert(root,21);
	insert(root,24);
	
	printPreorder(root);
	root = deleteNode(root,25);
	printf("\n");
	printPreorder(root);
	
	return 0;
}
