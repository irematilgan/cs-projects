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


//Cümle konsoldan okunmak istenirse
void scanConsole(char sentence[]){

	int i = 0;
	char c;
	
	//Daha önce kullanýcýdan input karakter alýndýðýndan, arkasýndan gelen '\n' karakterini
	//almasý için getchar() yeniden kullanýlýr
	getchar();
	
	printf("Cumleniz : ");

	while((i < MAX) && (c = getchar())!= '\n')
	{
		sentence[i] = c;
		i++;
	}	
	sentence[i] = '\0';	//Cümle sonu olduðunun bilgisayar tarafýndan anlaþýlabilmesi için
						//dizinin sonuna '\0' yerleþtirilir
	
}

//Cümle dosyadan okunmak istenirse 
void scanFile(char sentence[]){
	
	char fname[20],tmp;
	int i = 0;
	
	//Kullanýcýdan dosyanýn adý alýnýr
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

//Linked list'i yazdýrma fonksiyonu
void printList(NODE* root){
	
	NODE* current;
	
	for(current = root; current->next != NULL; current = current->next)
	{
		
		printf("[  %c  ( %d )  ] --> ",current->word, current->frequency);
	
	}
	printf("[  %c  ( %d )  ]",current->word, current->frequency);
}

NODE* createNode(char key){
	
	NODE* node = (NODE*)malloc(sizeof(NODE)); 		//node için hafýzada yer ayrýlýr
	node->word = key;						  		//yerleþtirmek istediðimiz harf word'e atanýr
	node->frequency = 1;
	node->right = node->left = node->next = NULL;
	return node;
}


//Gelen harfin node'u oluþturulmadan önce linkli listede olup olmadýðýna bakýlýr
NODE* searchWord(NODE* root, char key){
	
	NODE* current = root;
	
	
	while(current->next != NULL && key != current->word ){
		current = current->next;
	}
	//Eðer listenin sonuna geldiysek ve hala bulamadýysak
	if(current->word != key)
		current->next = createNode(key);	//Yeni node yaratýlýr
	else
		current->frequency++;				//Harf listede bulunduysa ilgili node'un frekansý arttýrýlýr
	
	return root;
	
	
	
}

NODE* addLinkedList(char input[]){
		
	NODE *root = createNode(input[0]);		//root node oluþturulur
	int i = 1;

	while(input[i] != '\0'){
		
		root = searchWord(root,input[i]);	//Alýnan input harf, linkli listede olup olmadýðýný kontrol
											//etmek üzere searchWord fonksiyonuna gönderilir
		i++;
		if(input[i] == ' ')					//Cümledeki boþluklarýn sayýlmamasý için boþluk görüldüðünde
			i++;							//bir sonraki karaktere geçilir
			
	}

	return root;
}


//Linkli liste oluþturulduktan sonra insertion sort ile sýralanýr
NODE* insertionSort(NODE* root){
	
	NODE* i = root;
	NODE* j;
	
	NODE* next;
	NODE* tmp = NULL;
	
	while(i != NULL){
		
		next = i->next; //i node'u üzerinde deðiþiklik yapacaðýmýz için gelecek node'unu
						//kaybetmemek için next NODE pointer'ýnda tutarýz
		
		if(tmp == NULL ||(tmp->frequency >= i->frequency)){	//tmp henüz bir node'u göstermiyorsa (baþlangýç) veya gösterdiði node'un frekansý 
															//bulunduðumuz node'un frekansýndan büyük ve eþitse (küçükten büyüðe sýralama yapýyoruz)
			//Ýki node arasýnda swap iþlemi yapýlýr
			i->next = tmp;
			tmp = i;
		}
		else{			
			//Eðer tmp'nin gösterdiði node'un frekansý, bulunduðumuz node'un frekansýndan küçükse
			//Frekansý kendinden küçük bir node bulunana dek tüm liste gezilir 
			
			j = tmp;
			
			while(j->next != NULL && (j->next->frequency < i->frequency)){
				j = j->next;
			}
			
	
			//Ýki node arasýnda swap iþlemi yapýlýr
			i->next = j->next;
			j->next = i;
			
		}
		
		//Listede bir sonraki node'a geçilir
		i = next;
	}
	
	root = tmp;
	

	
	return root;
	
}





//Aðaç yüksekliðini bulma fonksiyonu
int findHeight(NODE* root) 
{ 
    if (root == NULL) //Kök boþsa
        return 0; 
    else
    { 
        //Tüm aðaç gezilir
        int lheight = findHeight(root->left); 
        int rheight = findHeight(root->right); 
  
        //Bulunan yükseklikler karþýlaþtýrýlýp en büyüðü seçilerek 
		//sonunda aðacýn en büyük yüksekliði bulunur
        if (lheight > rheight) 
            return(lheight+1); 
        else return(rheight+1); 
    } 
} 

//Queue oluþturmak için
NODE** createQueue(int* front, int* rear){
	
	NODE** queue = (NODE**)malloc(sizeof(NODE*)*MAX);
	*front = *rear = 0;
	
	return queue;
}

//Kuyruða eleman ekleme fonksiyonu
void enqueue(NODE** queue, NODE* node, int* rear){
	queue[*rear] = node;
	(*rear)++;
}

//Kuyrukta öndeki elemaný çýkarma fonksiyonu
NODE* dequeue(NODE** queue, int* front){
	(*front)++;
	return queue[*front-1];
}

//Aðacý yüksekliðine göre yazdýrma fonksiyonu
void printLevelOrder(NODE* root, int h)
{
	int front, rear;
	
	//kuyruk NODE* dizisi olmak üzere oluþturulur
	NODE** queue = createQueue(&front,&rear);
	NODE* current = (NODE*)malloc(sizeof(NODE));
	current = root;
	NODE* node;
	
	
	int tmp, i = 0;
	int counter = 0;
	
	
	//Her seviyede kaç eleman olmasý gerektiði 2'nin kuvveti alýnarak (binary tree) hesaplanýr
	tmp = pow(2,i);
	
	printf("\n\nTREE :\n\n");
	
	
	//Aðacýn yüksekliðine ulaþýlana kadar 
	while(tmp < pow(2,h)){
		
		
		//Eðer NODE'un frekansý 0 deðilse yani NODE boþ (NULL) deðilse
		if(current->frequency != 0){
			printf("%d",current->frequency); 
			if(current->word != 0)	//NODE harf saklýyorsa
				printf("%c",current->word);
			
		}
		
		printf("\t");
		
		counter++; //counter seviyenin tamamen gezilip gezilmediðini kontrol etmek amacýyla kullanýlýr
		
		if(counter == tmp) 	//tüm seviye gezildiyse
		{
			counter = 0;	//counter sýfýrlanýr
			i++;			//bir sonraki seviyeye geçilir
			tmp = pow(2,i);	//yeni seviyede gezilmesi gereken NODE sayýsý hesaplanýr
		
			printf(" \n");	//yeni seviyeye geçildiði için alt satýra geçilir
		}
		
		//NODE'un solunda veya saðýnda iþaret ettiði NODE boþ deðilse kuyruða eklenir
		if(current != NULL && current->left){
			enqueue(queue,current->left,&rear);	
		}
		else	//NODE'un solundaki veya saðýndaki NODE boþ ise kuyruða boþ olduðunu göstermek için 0 frekansa sahip bir node eklenir
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
	
		//Kuyruktan sýrasý gelen eleman çýkarýlýr ve yazdýrýlýr
		current = dequeue(queue,&front);
		
	}
	
	
}

//Linkli liste insertion sort ile sýralandýktan sonra Huffman aðacý oluþturulur
NODE* createHuffmanTree(NODE* root){
	
	NODE* newRoot;
	NODE* tmp; NODE* node;
	int h;
	
	
	//Aðaç tamamen oluþtuðunda kökte sadece bir node olacaðýndan,
	//Gösterdiði gelecek bir node olmayacaktýr
	while(root->next->next != NULL){
		
		
		node = createNode(0); //Yeni bir node oluþturulur
		node->next = root->next->next; 	//Yeni oluþan node'un linkli listeye baðlanabilmesi için
										//Node'un kökten 2 sonraki node'u göstermesi saðlanýr
		root->next->next = node;		//yeni node, 3. node yapýlýr 
		
		node->frequency = root->frequency + root->next->frequency; 	//Ýlk iki node'un frekanslarýnýn toplamý
																	//3. node'a aktarýlýr
		
		//node'un solu ilk node'u saðý ise ikinci node'u gösterir
		node->left = root;			
		node->right = root->next;
		node->left->next = NULL;
		node->right->next = NULL;
		
		//Oluþan son yapý tekrar sýralanmak üzere insertionSort fonksiyonuna gönderilir
		//ve yeni kök (frekansý en küçük node) belirlenir
		
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
