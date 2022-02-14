#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct NODE{
	int count;
	char value[50];
	
	struct NODE* next;
	struct NODE* prev;
	
	
}NODE;


//Overflow olup olmad���n� anlamak i�in kullan�lan fonksiyon
int checkOF(NODE* node)
{
	if(node) //E�er node NULL de�erine sahip de�ilse overflow olmam�� demektir
		return 1;
	else
	{
		printf("Hata!..Node olusturulamadi!..");
		exit(1);
	}
		
	
}

//Double linked list'in head'ini olu�turmak i�in kullan�lan fonksiyon
void initHead(NODE** head)
{
	(*head) = (NODE*)malloc(sizeof(NODE));
	if(checkOF((*head))) //head node olu�turulup olu�turulamad���n� kontrol eder
	{
		//ilk node yarat�ld��� i�in kendinden �ncesini ve sonras�n� g�steren NODE pointerlar NULL g�sterir
		(*head)->next = NULL; 
		(*head)->prev = NULL;
		(*head)->count = 1;
	}
	
	
}


//Dosyadan okunan her yeni de�eri Double Linked List'e yerle�tirmek i�in kullan�lan fonksiyon
void pushNode(NODE** head)
{
	//Yeni node i�in bellekte yer ayr�l�r
	NODE* newNode = (NODE*)malloc(sizeof(NODE));
	
	//E�er node ba�ar�yla olu�turulduysa 
	if(checkOF(newNode))
	{
		newNode->next = (*head); //newNode'un next'i, head'in daha �nce i�aretmi� oldu�u node'u,
		newNode->prev = NULL;    //newNode'un prev'i, NULL'u ve
		newNode->next->prev = newNode;  //newNode'dan sonra gelecek node'un prev'i, newNode'u g�sterecek �ekilde ayarlan�r
		(*head) = newNode;   //Son olarak olu�turulan yeni node head yap�larak Linked List'in ba��na tamam�yla ge�mi� olur
		newNode->count = 1;  //Node ilk kez olu�turuldu�u i�in (kendiyle ayn� de�ere sahip ba�ka bir node olmad���ndan) sayac� 1 yap�l�r
	}

}

//Double Linked List'in kapasitesini a��p a�mad���n� kontrol eden fonksiyon
int checkCapacity(int L)
{
	static int capacity_counter = 0; //fonksiyonun her yeni elemanda artan kapasite de�erini tutabilmesi i�in saya� "static" tipinde yarat�ld�
	capacity_counter++; //fonksiyon her �a�r�ld���nda yeni bir node eklenmi� demektir
	if(L == 0) //Kullan�c�n�n kapasiteyi s�f�r girmesi durumunda program hata verir
	{
		printf("Linked list olusturulamaz!..");
		exit(1);
	}
	else if(capacity_counter > L) //Kapasite a��lm��sa ba�ka bir fonksiyon yard�m�yla listedeki son node silinecektir
	{
		capacity_counter--; //Bu y�zden dolan kapasite miktar� bir azalt�l�r
		return 1;
	}
	else
		return 0;  //Hen�z kapasite a��lmam��sa
		
}

//Kapasitenin a��lmas� durumunda �a��r�lan son node'u silme fonksiyonu
void deleteNode(NODE** head)
{
	NODE* current;
	for(current = *head;current->next != NULL;current = current->next); //Listenin sonuna gidilir
	
	current->prev->next = NULL; //Yeni son node, sondan bir �nce gelen node olaca�� i�in sondan bir �nceki node'un next'i
								//NULL g�sterecek �ekilde ayarlan�r
	
	
	free(current); //Son node bellekten silinir
	
	
}

//Text dosyas�ndan okunan her de�erin listedeki elemanlardan biriyle ayn� olup olmad���n� ��renmek i�in kullan�lan fonksiyon
int searchNodes(NODE** head,char* string,int threshold)
{
	NODE* current = *head;
	NODE* tmp;
	
	//gezdi�imiz node'lardan biri, gelen stringle ayn� olmad��� ve listeyi gezmeyi bitirmedi�imiz s�rece aramaya devam edilir
	while((current != NULL) && strcmp(current->value,string)) 
	{
		current = current->next;
	}
	
	if(current == NULL) //E�er string Double Linked List'teki hi�bir elemanla ayn� de�ere sahip de�ilse
	{
		return 1;
	}
	else
	{
		(current->count)++; //Listedeki elemanlardan biri ile ayn� de�ere sahipse eleman�n bulundu�u node'un sayac� 1 artar
		if(current->count > threshold) //Mevcut node'un sayac�, e�ik de�erini ge�mi�se node, head olmak �zere ayarlan�r
		{
			tmp = *head;  				//head saklanmak i�in tmp isimli NODE* da tutulur
			
			/*
			E�er son node yeni head olacaksa
			Son node'dan �nce gelen node art�k sonuncu node olaca�� i�in next'i NULL olur
			Ancak e�ik de�erini a�an node listenin son node'u de�ilse kendinden �nce gelen node (prev) 
			kendinden sonra gelen node'u (next) g�stermelidir
			*/
			
			current->prev->next = current->next;	//Bu kod sat�r� iki durum i�in de �al��maktad�r
				
				
			current->next = *head;		//head art�k listedeki ikinci node olaca�� i�in yeni head'in next'i eski head'i g�sterir
			current->prev = NULL;		//mevcut node, head olaca�� i�in prev'i art�k NULL g�sterir
			(*head)->prev = current;	//head'den �nceki node, yeni head node'u g�sterir
			*head = current;			//yeni head art�k e�ik de�erini ge�en node olmu�tur
			
		}
		return 0;
	}
		
	
}




//Double Linked List elemanlar�n� yazd�rma fonksiyonu
void listNodes(NODE* head)
{
	NODE* current;
	current = head;
	
	printf("   %s , %d\t",current->value, current->count);
	for(current = head->next;current != NULL;current = current->next)
		printf("-> <-\t%s , %d\t\t",current->value, current->count);
	printf("\n");
		
}


//Kullan�c� linked list elemanlar�n� txt dosyas�ndan girmek isterse �al��acak fonksiyon
NODE* scanFile(FILE* fp,int threshold, int capacity)
{	
	NODE* head;
	initHead(&head); 	//head'e bellekte yer ay�ran ve NODE pointer'lar�n� d�zenleyen fonksiyon
	char tmp[50];		//dosyadan okuyaca��m�z stringleri ge�ici olarak alan char dizisi
	
	//Head node olu�turuldu�unda kapasitenin a��l�p a��lmad��� kontrol edilir
	if(checkCapacity(capacity))
	{
		deleteNode(&head); //Kapasite a��ld�ysa node'u sil
	}
	
	
	fscanf(fp,"%s",head->value);  //Dosyadan okunan ilk de�er head node'un i�aret etti�i adresteki 'value' de�i�kenine yaz�l�r
	listNodes(head);
	
	while(!feof(fp))
	{
		
		fscanf(fp,"%s",tmp);

		//Kullan�c�dan al�nan veya dosyadan okunan string'in node'lardan birinde olup olmad��� kontrol edilir
		//E�er hi�bir node ile ayn� de�ere sahip de�ilse 1, aksi taktirde 0 d�ner
		if(searchNodes(&head,tmp,threshold)) 
		{
			//Listeye yeni bir node yerle�ece�i i�in kapasitenin a��l�p a��lmad��� kontrol edilir
			if(checkCapacity(capacity))
				deleteNode(&head);	//A��lm��sa son node'un silinmesi i�in deleteNode fonksiyonu �a�r�l�r
			
			pushNode(&head);		//Yeni node'un listeye eklenmesi i�in pushNode fonksiyonu �a�r�l�r
			
			strcat(head->value,tmp);	//Yeni head node'un g�sterdi�i adrese input olarak ald���m�z string yerle�tirilir
			
		}
		
		
		listNodes(head);
		
			
	}
	
	
	return head;	//Olu�an NODE pointer, main fonksiyonuna g�nderilir

}

//Kullan�c� string de�erlerini konsoldan girmek isterse
NODE* scanConsole(int threshold,int capacity)
{	
	char inputs[50];
	short n;	//Giri� yap�lacak string say�s�
	int i;
	
	printf("\nListeye kac eleman ekleyeceginizi giriniz : ");scanf("%d",&n);
	printf("\n");
	
	//Bu k�s�m scanFile fonksiyonundaki kodlarda yaz�lan mant�kla �al��maktad�r
	NODE* head;
	initHead(&head);
	if(checkCapacity(capacity))	//Kapasite kontrol�
	{
		deleteNode(&head);		//Kapasite a��l�rsa node'u sil
	}
	
	scanf("%s",head->value);
	listNodes(head);
	
	for(i = 1; i < n; i++)
	{
		scanf("%s",inputs);
		
		if(searchNodes(&head,inputs,threshold))	//Girilen de�ere e�it node var m� diye kontrol et
		{
			if(checkCapacity(capacity))
				deleteNode(&head);
				
			pushNode(&head);					//E�it node bulunamam��sa listeye yeni node'u ekle
			
			strcat(head->value,inputs);
		}
		
		listNodes(head);
	}
	
	return head;
	
}


//Dosya a�ma fonksiyonu
FILE* openFile()
{
	FILE *fp;
	
	fp = fopen("Input.txt","r");
	
	if(fp)
		return fp;
	else
	{
		perror("Dosya acilamadi!..");
		exit(1);
	}
		
		
}

//Kullan�c�n�n isterse cache buffer'� temizleyebilmesi i�in kullan�lan freeNodes fonksiyonu
void freeNodes(NODE** head)
{
	NODE* current = *head;
	for(current = (*head)->next; current->next != NULL; current = current->next)
		free(current->prev);
	
	
	free(current);
	
	printf("\nTum elemanlar temizlendi..");
}



int main()
{	
	char ans;
	int file_or_console;
	NODE* head;
	
	int T,L;
	printf("Esik Degeri Giriniz (T) = ");scanf("%d",&T);
	printf("Kapasite Giriniz (L) = ");scanf("%d",&L);
	printf("\n");
	
	printf("Dosya ile giris yapmak icin 1, Konsoldan giris yapmak icin 0 giriniz : ");scanf("%d",&file_or_console);
	
	if(file_or_console)				//Dosya ile giri� yap�lacaksa
	{
		FILE* fp = openFile();
		head = scanFile(fp,T,L);
		fclose(fp);
	}
	else							//Konsoldan giri� yap�lacaksa
		head = scanConsole(T,L);
	
		
	printf("\nCache Buffer'i temizlemek istiyor musunuz ? [y\\n] ");
	scanf(" %c",&ans);
	if(ans == 'y')
		freeNodes(&head);			//Kullan�c� cache buffer'� temizlemek isterse

	
	return 0;
}

