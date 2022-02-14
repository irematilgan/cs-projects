#include <stdio.h>
#include <stdlib.h>

//Node tanýmlamasý
typedef struct NODE{
	int val;
	struct NODE *right,*left;
}NODE;


//Node yaratma
NODE* create(int value){
	NODE* tmp = (NODE*)malloc(sizeof(NODE)); //bellekte yer ayýr
	//node'un sað ve sol pointer'ýnýn hiçbir þeyi göstermemesini saðla
	tmp->left = NULL; 
	tmp->right = NULL;
	tmp->val = value; //node'un deðerini belirle
	
	return tmp; //oluþturulan node'u dön
}

//yeni gelen deðeri aðaca yerleþtirmek için kullanýlan metod
NODE* insert(NODE* root,int value){
	if(!(root)) //root'u yoksa (aðacýn terminaline ulaþýlmýþsa)
		return create(value); //yeni node yarat
	if((root)->val > value)//root'taki deðer yeni gelen deðerden büyükse
		(root)->left = insert((root->left),value);//yeni gelen deðeri root'un soluna yerleþtir 
		//(solunda baþka bir node daha varsa recursive þekilde insert fonksiyonu çaðýrýlmaya devam eder)
	else
		if((root)->val < value) //root'taki deðer, yeni gelen deðerden küçükse,
			(root)->right = insert(root->right,value); //root node'un saðýna yerleþtir
			//(saðýnda baþka bir node daha varsa recursive þekilde insert fonksiyonu çaðýrýlmaya devam eder)
		
}

//Girilen bir deðeri içeren node'u bulmayý saðlayan fonksiyon
NODE* search(NODE* root,int value){

	NODE* tmp;
	tmp = root;
	
	while(tmp && (tmp->val != value)){ //deðer bulunmadýðý VE aðacýn sonuna ulaþýlmadýðý sürece 
	 	//aradýðýmýz deðer bulunduðumuz node'daki deðerden küçükse soldaki, büyükse saðdaki node'a gider.
		if(tmp->val > value)
			tmp = tmp->left;
		else
			tmp = tmp->right;
	}
	
	return tmp; //Son bulunduðumuz node'un adresini dön (Eðer deðer bulunamamýþsa NULL döner)
}

//Aðacý yazdýrma fonksiyonu (Recursive)
void printTree(NODE* root){
	
	if(!root){//Aðacýn terminaline gelinmiþse
	}
	else{
		
		printTree(root->left);
		printf("%d\t",root->val);
		printTree(root->right); //Node'un kendisi, bununla birlikte saðý ve solundaki node'larýn deðeri yazdýrýlýr
		//Önce soldaki node'a gidilerek fonksiyon sürekli olarak çaðýrýlýr
		
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
	
	//Aradýðýmýz deðeri bulamamýþsak son node NULL demektir
	if(!root){
		return root;
	}
	if(root->val > value){ //aradðýmýz deðer, bulunduðumuz node'daki deðerden küçükse
		root->left = deleteNode(root->left,value); //soldaki node'a git
	}
	else if(root->val < value) //büyükse
		root->right = deleteNode(root->right,value); //saðdaki node'a git
	else{ //aradýðýmýz deðeri bulmuþsak 
		if(root->left == NULL){ //Bulunduðumuz node'un solunun NULL'u gösterip göstermediðine bakarýz, gösteriyorsa
			NODE *tmp = root->right; //Bulunduðumuz node'u silmeden önce, gösterdiði sað node'un adresini tmp NODE pointer'ýnda tutarýz
			free(root); //Bulunduðumuz node'u bellekten sileriz
			return tmp; //Saðdaki node'un adresini return ederiz
		}
		else if(root->right == NULL){//Bulunduðumuz node'un saðýnýn NULL'u gösterip göstermediðine bakarýz, gösteriyorsa
			NODE *tmp = root->left; //Bulunduðumuz node'u silmeden önce, gösterdiði sað node'un adresini tmp NODE pointer'ýnda tutarýz
			free(root); //..
			return tmp; //..
		}
		//Bu iki koþul, tek çocuðu olan node'larýn silinmesi iþlemi için tasarlanmýþtýr
		
		//Ýki çocuðu olan node'larý silmek istediðimizde,
		
		NODE* tmp = findInorderSuccessor(root->right);
		root->val = tmp->val;
		root->right = deleteNode(root->right,tmp->val);
	}
	
	
	return root;

}


int main()
{
	int list[] = {25,19,36,16,23,28,21,25};
	NODE* root = NULL; //Aðaç henüz oluþturulmadýðý için root node NULL'u iþaret eder
	
	root = insert(root,list[0]);
	
	int i;
	for(i = 1; i < 8; i++){
		insert(root,list[i]); //Listedeki deðerler tek tek aðaca yerleþtirilir
	}
	
	printTree(root);
	root = deleteNode(root,25);
	printf("\n");
	printTree(root);
	//freeTree(&root);
	return 0;
}
