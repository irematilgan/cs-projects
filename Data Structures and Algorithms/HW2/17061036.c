#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#define MAX 100


typedef struct node{
	char word;
	int frequency;
	struct node *right,*left,*next;
}NODE;


//C�mle konsoldan okunmak istenirse
void scanConsole(char sentence[]){

	int i = 0;
	char c;
	
	//Daha �nce kullan�c�dan input karakter al�nd���ndan, arkas�ndan gelen '\n' karakterini
	//almas� i�in getchar() yeniden kullan�l�r
	getchar();
	
	printf("Cumleniz : ");

	while((i < MAX) && (c = getchar())!= '\n')
	{
		sentence[i] = c;
		i++;
	}	
	sentence[i] = '\0';	//C�mle sonu oldu�unun bilgisayar taraf�ndan anla��labilmesi i�in
						//dizinin sonuna '\0' yerle�tirilir
	
}

//C�mle dosyadan okunmak istenirse 
void scanFile(char sentence[]){
	
	char fname[20],tmp;
	int i = 0;
	
	//Kullan�c�dan dosyan�n ad� al�n�r
	printf("Dosya adini giriniz (txt) : "); scanf("%s",fname);
	strcat(fname,".txt");
	FILE* fp = fopen(fname,"r");
	
	
	if(fp != NULL){
		
		while((sentence[i] = fgetc(fp)) != EOF){
			i++;
		}
		sentence[i] = '\0';
		
	}
	else{
		printf("Dosya Acilamadi!..");
		exit(1);
	}
	
	fclose(fp);
		
}

//Linked list'i yazd�rma fonksiyonu
void printList(NODE* root){
	
	NODE* current;
	
	for(current = root; current->next != NULL; current = current->next)
	{
		
		printf("[  %c  ( %d )  ] --> ",current->word, current->frequency);
	
	}
	printf("[  %c  ( %d )  ]",current->word, current->frequency);
}

NODE* createNode(char key){
	
	NODE* node = (NODE*)malloc(sizeof(NODE)); 		//node i�in haf�zada yer ayr�l�r
	node->word = key;						  		//yerle�tirmek istedi�imiz harf word'e atan�r
	node->frequency = 1;
	node->right = node->left = node->next = NULL;
	return node;
}


//Gelen harfin node'u olu�turulmadan �nce linkli listede olup olmad���na bak�l�r
NODE* searchWord(NODE* root, char key){
	
	NODE* current = root;
	
	
	while(current->next != NULL && key != current->word ){
		current = current->next;
	}
	//E�er listenin sonuna geldiysek ve hala bulamad�ysak
	if(current->word != key)
		current->next = createNode(key);	//Yeni node yarat�l�r
	else
		current->frequency++;				//Harf listede bulunduysa ilgili node'un frekans� artt�r�l�r
	
	return root;
	
	
	
}

NODE* addLinkedList(char input[]){
		
	NODE *root = createNode(input[0]);		//root node olu�turulur
	int i = 1;

	while(input[i] != '\0'){
		
		root = searchWord(root,input[i]);	//Al�nan input harf, linkli listede olup olmad���n� kontrol
											//etmek �zere searchWord fonksiyonuna g�nderilir
		i++;
		if(input[i] == ' ')					//C�mledeki bo�luklar�n say�lmamas� i�in bo�luk g�r�ld���nde
			i++;							//bir sonraki karaktere ge�ilir
			
	}

	return root;
}


//Linkli liste olu�turulduktan sonra insertion sort ile s�ralan�r
NODE* insertionSort(NODE* root){
	
	NODE* i = root;
	NODE* j;
	
	NODE* next;
	NODE* tmp = NULL;
	
	while(i != NULL){
		
		next = i->next; //i node'u �zerinde de�i�iklik yapaca��m�z i�in gelecek node'unu
						//kaybetmemek i�in next NODE pointer'�nda tutar�z
		
		if(tmp == NULL ||(tmp->frequency >= i->frequency)){	//tmp hen�z bir node'u g�stermiyorsa (ba�lang��) veya g�sterdi�i node'un frekans� 
															//bulundu�umuz node'un frekans�ndan b�y�k ve e�itse (k���kten b�y��e s�ralama yap�yoruz)
			//�ki node aras�nda swap i�lemi yap�l�r
			i->next = tmp;
			tmp = i;
		}
		else{			
			//E�er tmp'nin g�sterdi�i node'un frekans�, bulundu�umuz node'un frekans�ndan k���kse
			//Frekans� kendinden k���k bir node bulunana dek t�m liste gezilir 
			
			j = tmp;
			
			while(j->next != NULL && (j->next->frequency < i->frequency)){
				j = j->next;
			}
			
	
			//�ki node aras�nda swap i�lemi yap�l�r
			i->next = j->next;
			j->next = i;
			
		}
		
		//Listede bir sonraki node'a ge�ilir
		i = next;
	}
	
	root = tmp;
	

	
	return root;
	
}





//A�a� y�ksekli�ini bulma fonksiyonu
int findHeight(NODE* root) 
{ 
    if (root == NULL) //K�k bo�sa
        return 0; 
    else
    { 
        //T�m a�a� gezilir
        int lheight = findHeight(root->left); 
        int rheight = findHeight(root->right); 
  
        //Bulunan y�kseklikler kar��la�t�r�l�p en b�y��� se�ilerek 
		//sonunda a�ac�n en b�y�k y�ksekli�i bulunur
        if (lheight > rheight) 
            return(lheight+1); 
        else return(rheight+1); 
    } 
} 

//Queue olu�turmak i�in
NODE** createQueue(int* front, int* rear){
	
	NODE** queue = (NODE**)malloc(sizeof(NODE*)*MAX);
	*front = *rear = 0;
	
	return queue;
}

//Kuyru�a eleman ekleme fonksiyonu
void enqueue(NODE** queue, NODE* node, int* rear){
	queue[*rear] = node;
	(*rear)++;
}

//Kuyrukta �ndeki eleman� ��karma fonksiyonu
NODE* dequeue(NODE** queue, int* front){
	(*front)++;
	return queue[*front-1];
}

//A�ac� y�ksekli�ine g�re yazd�rma fonksiyonu
void printLevelOrder(NODE* root, int h)
{
	int front, rear;
	
	//kuyruk NODE* dizisi olmak �zere olu�turulur
	NODE** queue = createQueue(&front,&rear);
	NODE* current = (NODE*)malloc(sizeof(NODE));
	current = root;
	NODE* node;
	
	
	int tmp, i = 0;
	int counter = 0;
	
	
	//Her seviyede ka� eleman olmas� gerekti�i 2'nin kuvveti al�narak (binary tree) hesaplan�r
	tmp = pow(2,i);
	
	printf("\n\nTREE :\n\n");
	
	
	//A�ac�n y�ksekli�ine ula��lana kadar 
	while(tmp < pow(2,h)){
		
		
		//E�er NODE'un frekans� 0 de�ilse yani NODE bo� (NULL) de�ilse
		if(current->frequency != 0){
			printf("%d",current->frequency); 
			if(current->word != 0)	//NODE harf sakl�yorsa
				printf("%c",current->word);
			
		}
		
		printf("\t");
		
		counter++; //counter seviyenin tamamen gezilip gezilmedi�ini kontrol etmek amac�yla kullan�l�r
		
		if(counter == tmp) 	//t�m seviye gezildiyse
		{
			counter = 0;	//counter s�f�rlan�r
			i++;			//bir sonraki seviyeye ge�ilir
			tmp = pow(2,i);	//yeni seviyede gezilmesi gereken NODE say�s� hesaplan�r
		
			printf(" \n");	//yeni seviyeye ge�ildi�i i�in alt sat�ra ge�ilir
		}
		
		//NODE'un solunda veya sa��nda i�aret etti�i NODE bo� de�ilse kuyru�a eklenir
		if(current != NULL && current->left){
			enqueue(queue,current->left,&rear);	
		}
		else	//NODE'un solundaki veya sa��ndaki NODE bo� ise kuyru�a bo� oldu�unu g�stermek i�in 0 frekansa sahip bir node eklenir
		{
			node = (NODE*)malloc(sizeof(NODE));
			node->frequency = 0;
			
			queue[rear] = node;
			rear++;
			
		}
		if(current != NULL && current->right){
			enqueue(queue,current->right,&rear);
		}
		else
		{
			node = (NODE*)malloc(sizeof(NODE));
			node->frequency = 0;
			queue[rear] = node;
			rear++;
		}
	
		//Kuyruktan s�ras� gelen eleman ��kar�l�r ve yazd�r�l�r
		current = dequeue(queue,&front);
		
	}
	
	
}

//Linkli liste insertion sort ile s�raland�ktan sonra Huffman a�ac� olu�turulur
NODE* createHuffmanTree(NODE* root){
	
	NODE* newRoot;
	NODE* tmp; NODE* node;
	int h;
	
	
	//A�a� tamamen olu�tu�unda k�kte sadece bir node olaca��ndan,
	//G�sterdi�i gelecek bir node olmayacakt�r
	while(root->next->next != NULL){
		
		
		node = createNode(0); //Yeni bir node olu�turulur
		node->next = root->next->next; 	//Yeni olu�an node'un linkli listeye ba�lanabilmesi i�in
										//Node'un k�kten 2 sonraki node'u g�stermesi sa�lan�r
		root->next->next = node;		//yeni node, 3. node yap�l�r 
		
		node->frequency = root->frequency + root->next->frequency; 	//�lk iki node'un frekanslar�n�n toplam�
																	//3. node'a aktar�l�r
		
		//node'un solu ilk node'u sa�� ise ikinci node'u g�sterir
		node->left = root;			
		node->right = root->next;
		node->left->next = NULL;
		node->right->next = NULL;
		
		//Olu�an son yap� tekrar s�ralanmak �zere insertionSort fonksiyonuna g�nderilir
		//ve yeni k�k (frekans� en k���k node) belirlenir
		
		root = insertionSort(node);
			
	}
	
	node = createNode(0);
	node->frequency = root->frequency + root->next->frequency; 	
	node->left = root;			
	node->right = root->next;
	node->left->next = NULL;
	node->right->next = NULL;
	root = node;
	
	
	
	h = findHeight(root);
	printLevelOrder(root,h);
	
	
	return root;
}




int main(){
	
	char input[MAX];
	int key, height;
	
	printf("\nCumlenizi konsoldan yazmak isterseniz 1, dosyadan yazdirmak isterseniz 0 giriniz : ");
	scanf("%d",&key);
	
	
	if(key)
		scanConsole(input);
	else{
		scanFile(input);
		printf("\n\nCumle : \"%s\"",input);
	}
	printf("\n");
	
	
	
	NODE* root = addLinkedList(input);
	
	printf("\nLinkli Liste :\n");
	printList(root);
	
	NODE* newRoot = insertionSort(root);
	
	printf("\n\nLinkli Liste (Insertion Sort) :\n");
	printList(newRoot);
	
	NODE* newRoot2 = createHuffmanTree(newRoot);
	

	
	return 0;
}
