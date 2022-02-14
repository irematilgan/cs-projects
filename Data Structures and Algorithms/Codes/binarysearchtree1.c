#include <stdio.h>
#include <stdlib.h>

//Node tan�mlamas�
typedef struct NODE{
	int val;
	struct NODE *right,*left;
}NODE;


//Node yaratma
NODE* create(int value){
	NODE* tmp = (NODE*)malloc(sizeof(NODE)); //bellekte yer ay�r
	//node'un sa� ve sol pointer'�n�n hi�bir �eyi g�stermemesini sa�la
	tmp->left = NULL; 
	tmp->right = NULL;
	tmp->val = value; //node'un de�erini belirle
	
	return tmp; //olu�turulan node'u d�n
}

//yeni gelen de�eri a�aca yerle�tirmek i�in kullan�lan metod
NODE* insert(NODE* root,int value){
	if(!(root)) //root'u yoksa (a�ac�n terminaline ula��lm��sa)
		return create(value); //yeni node yarat
	if((root)->val > value)//root'taki de�er yeni gelen de�erden b�y�kse
		(root)->left = insert((root->left),value);//yeni gelen de�eri root'un soluna yerle�tir 
		//(solunda ba�ka bir node daha varsa recursive �ekilde insert fonksiyonu �a��r�lmaya devam eder)
	else
		if((root)->val < value) //root'taki de�er, yeni gelen de�erden k���kse,
			(root)->right = insert(root->right,value); //root node'un sa��na yerle�tir
			//(sa��nda ba�ka bir node daha varsa recursive �ekilde insert fonksiyonu �a��r�lmaya devam eder)
		
}

//Girilen bir de�eri i�eren node'u bulmay� sa�layan fonksiyon
NODE* search(NODE* root,int value){

	NODE* tmp;
	tmp = root;
	
	while(tmp && (tmp->val != value)){ //de�er bulunmad��� VE a�ac�n sonuna ula��lmad��� s�rece 
	 	//arad���m�z de�er bulundu�umuz node'daki de�erden k���kse soldaki, b�y�kse sa�daki node'a gider.
		if(tmp->val > value)
			tmp = tmp->left;
		else
			tmp = tmp->right;
	}
	
	return tmp; //Son bulundu�umuz node'un adresini d�n (E�er de�er bulunamam��sa NULL d�ner)
}

//A�ac� yazd�rma fonksiyonu (Recursive)
void printTree(NODE* root){
	
	if(!root){//A�ac�n terminaline gelinmi�se
	}
	else{
		
		printTree(root->left);
		printf("%d\t",root->val);
		printTree(root->right); //Node'un kendisi, bununla birlikte sa�� ve solundaki node'lar�n de�eri yazd�r�l�r
		//�nce soldaki node'a gidilerek fonksiyon s�rekli olarak �a��r�l�r
		
	}
}

/*
void freeTree(NODE **root){
	
}
*/

NODE* findInorderSuccessor(NODE* root){
	
	NODE *current = root;
	while(current && (current->left != NULL))
		current = current->left;
	return current;
	
}

NODE* deleteNode(NODE *root,int value){
	
	//Arad���m�z de�eri bulamam��sak son node NULL demektir
	if(!root){
		return root;
	}
	if(root->val > value){ //arad��m�z de�er, bulundu�umuz node'daki de�erden k���kse
		root->left = deleteNode(root->left,value); //soldaki node'a git
	}
	else if(root->val < value) //b�y�kse
		root->right = deleteNode(root->right,value); //sa�daki node'a git
	else{ //arad���m�z de�eri bulmu�sak 
		if(root->left == NULL){ //Bulundu�umuz node'un solunun NULL'u g�sterip g�stermedi�ine bakar�z, g�steriyorsa
			NODE *tmp = root->right; //Bulundu�umuz node'u silmeden �nce, g�sterdi�i sa� node'un adresini tmp NODE pointer'�nda tutar�z
			free(root); //Bulundu�umuz node'u bellekten sileriz
			return tmp; //Sa�daki node'un adresini return ederiz
		}
		else if(root->right == NULL){//Bulundu�umuz node'un sa��n�n NULL'u g�sterip g�stermedi�ine bakar�z, g�steriyorsa
			NODE *tmp = root->left; //Bulundu�umuz node'u silmeden �nce, g�sterdi�i sa� node'un adresini tmp NODE pointer'�nda tutar�z
			free(root); //..
			return tmp; //..
		}
		//Bu iki ko�ul, tek �ocu�u olan node'lar�n silinmesi i�lemi i�in tasarlanm��t�r
		
		//�ki �ocu�u olan node'lar� silmek istedi�imizde,
		
		NODE* tmp = findInorderSuccessor(root->right);
		root->val = tmp->val;
		root->right = deleteNode(root->right,tmp->val);
	}
	
	
	return root;

}


int main()
{
	int list[] = {25,19,36,16,23,28,21,25};
	NODE* root = NULL; //A�a� hen�z olu�turulmad��� i�in root node NULL'u i�aret eder
	
	root = insert(root,list[0]);
	
	int i;
	for(i = 1; i < 8; i++){
		insert(root,list[i]); //Listedeki de�erler tek tek a�aca yerle�tirilir
	}
	
	printTree(root);
	root = deleteNode(root,25);
	printf("\n");
	printTree(root);
	//freeTree(&root);
	return 0;
}
