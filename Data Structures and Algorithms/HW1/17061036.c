#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct NODE{
	int count;
	char value[50];
	
	struct NODE* next;
	struct NODE* prev;
	
	
}NODE;


//Overflow olup olmadýðýný anlamak için kullanýlan fonksiyon
int checkOF(NODE* node)
{
	if(node) //Eðer node NULL deðerine sahip deðilse overflow olmamýþ demektir
		return 1;
	else
	{
		printf("Hata!..Node olusturulamadi!..");
		exit(1);
	}
		
	
}

//Double linked list'in head'ini oluþturmak için kullanýlan fonksiyon
void initHead(NODE** head)
{
	(*head) = (NODE*)malloc(sizeof(NODE));
	if(checkOF((*head))) //head node oluþturulup oluþturulamadýðýný kontrol eder
	{
		//ilk node yaratýldýðý için kendinden öncesini ve sonrasýný gösteren NODE pointerlar NULL gösterir
		(*head)->next = NULL; 
		(*head)->prev = NULL;
		(*head)->count = 1;
	}
	
	
}


//Dosyadan okunan her yeni deðeri Double Linked List'e yerleþtirmek için kullanýlan fonksiyon
void pushNode(NODE** head)
{
	//Yeni node için bellekte yer ayrýlýr
	NODE* newNode = (NODE*)malloc(sizeof(NODE));
	
	//Eðer node baþarýyla oluþturulduysa 
	if(checkOF(newNode))
	{
		newNode->next = (*head); //newNode'un next'i, head'in daha önce iþaretmiþ olduðu node'u,
		newNode->prev = NULL;    //newNode'un prev'i, NULL'u ve
		newNode->next->prev = newNode;  //newNode'dan sonra gelecek node'un prev'i, newNode'u gösterecek þekilde ayarlanýr
		(*head) = newNode;   //Son olarak oluþturulan yeni node head yapýlarak Linked List'in baþýna tamamýyla geçmiþ olur
		newNode->count = 1;  //Node ilk kez oluþturulduðu için (kendiyle ayný deðere sahip baþka bir node olmadýðýndan) sayacý 1 yapýlýr
	}

}

//Double Linked List'in kapasitesini aþýp aþmadýðýný kontrol eden fonksiyon
int checkCapacity(int L)
{
	static int capacity_counter = 0; //fonksiyonun her yeni elemanda artan kapasite deðerini tutabilmesi için sayaç "static" tipinde yaratýldý
	capacity_counter++; //fonksiyon her çaðrýldýðýnda yeni bir node eklenmiþ demektir
	if(L == 0) //Kullanýcýnýn kapasiteyi sýfýr girmesi durumunda program hata verir
	{
		printf("Linked list olusturulamaz!..");
		exit(1);
	}
	else if(capacity_counter > L) //Kapasite aþýlmýþsa baþka bir fonksiyon yardýmýyla listedeki son node silinecektir
	{
		capacity_counter--; //Bu yüzden dolan kapasite miktarý bir azaltýlýr
		return 1;
	}
	else
		return 0;  //Henüz kapasite aþýlmamýþsa
		
}

//Kapasitenin aþýlmasý durumunda çaðýrýlan son node'u silme fonksiyonu
void deleteNode(NODE** head)
{
	NODE* current;
	for(current = *head;current->next != NULL;current = current->next); //Listenin sonuna gidilir
	
	current->prev->next = NULL; //Yeni son node, sondan bir önce gelen node olacaðý için sondan bir önceki node'un next'i
								//NULL gösterecek þekilde ayarlanýr
	
	
	free(current); //Son node bellekten silinir
	
	
}

//Text dosyasýndan okunan her deðerin listedeki elemanlardan biriyle ayný olup olmadýðýný öðrenmek için kullanýlan fonksiyon
int searchNodes(NODE** head,char* string,int threshold)
{
	NODE* current = *head;
	NODE* tmp;
	
	//gezdiðimiz node'lardan biri, gelen stringle ayný olmadýðý ve listeyi gezmeyi bitirmediðimiz sürece aramaya devam edilir
	while((current != NULL) && strcmp(current->value,string)) 
	{
		current = current->next;
	}
	
	if(current == NULL) //Eðer string Double Linked List'teki hiçbir elemanla ayný deðere sahip deðilse
	{
		return 1;
	}
	else
	{
		(current->count)++; //Listedeki elemanlardan biri ile ayný deðere sahipse elemanýn bulunduðu node'un sayacý 1 artar
		if(current->count > threshold) //Mevcut node'un sayacý, eþik deðerini geçmiþse node, head olmak üzere ayarlanýr
		{
			tmp = *head;  				//head saklanmak için tmp isimli NODE* da tutulur
			
			/*
			Eðer son node yeni head olacaksa
			Son node'dan önce gelen node artýk sonuncu node olacaðý için next'i NULL olur
			Ancak eþik deðerini aþan node listenin son node'u deðilse kendinden önce gelen node (prev) 
			kendinden sonra gelen node'u (next) göstermelidir
			*/
			
			current->prev->next = current->next;	//Bu kod satýrý iki durum için de çalýþmaktadýr
				
				
			current->next = *head;		//head artýk listedeki ikinci node olacaðý için yeni head'in next'i eski head'i gösterir
			current->prev = NULL;		//mevcut node, head olacaðý için prev'i artýk NULL gösterir
			(*head)->prev = current;	//head'den önceki node, yeni head node'u gösterir
			*head = current;			//yeni head artýk eþik deðerini geçen node olmuþtur
			
		}
		return 0;
	}
		
	
}




//Double Linked List elemanlarýný yazdýrma fonksiyonu
void listNodes(NODE* head)
{
	NODE* current;
	current = head;
	
	printf("   %s , %d\t",current->value, current->count);
	for(current = head->next;current != NULL;current = current->next)
		printf("-> <-\t%s , %d\t\t",current->value, current->count);
	printf("\n");
		
}


//Kullanýcý linked list elemanlarýný txt dosyasýndan girmek isterse çalýþacak fonksiyon
NODE* scanFile(FILE* fp,int threshold, int capacity)
{	
	NODE* head;
	initHead(&head); 	//head'e bellekte yer ayýran ve NODE pointer'larýný düzenleyen fonksiyon
	char tmp[50];		//dosyadan okuyacaðýmýz stringleri geçici olarak alan char dizisi
	
	//Head node oluþturulduðunda kapasitenin aþýlýp aþýlmadýðý kontrol edilir
	if(checkCapacity(capacity))
	{
		deleteNode(&head); //Kapasite aþýldýysa node'u sil
	}
	
	
	fscanf(fp,"%s",head->value);  //Dosyadan okunan ilk deðer head node'un iþaret ettiði adresteki 'value' deðiþkenine yazýlýr
	listNodes(head);
	
	while(!feof(fp))
	{
		
		fscanf(fp,"%s",tmp);

		//Kullanýcýdan alýnan veya dosyadan okunan string'in node'lardan birinde olup olmadýðý kontrol edilir
		//Eðer hiçbir node ile ayný deðere sahip deðilse 1, aksi taktirde 0 döner
		if(searchNodes(&head,tmp,threshold)) 
		{
			//Listeye yeni bir node yerleþeceði için kapasitenin aþýlýp aþýlmadýðý kontrol edilir
			if(checkCapacity(capacity))
				deleteNode(&head);	//Aþýlmýþsa son node'un silinmesi için deleteNode fonksiyonu çaðrýlýr
			
			pushNode(&head);		//Yeni node'un listeye eklenmesi için pushNode fonksiyonu çaðrýlýr
			
			strcat(head->value,tmp);	//Yeni head node'un gösterdiði adrese input olarak aldýðýmýz string yerleþtirilir
			
		}
		
		
		listNodes(head);
		
			
	}
	
	
	return head;	//Oluþan NODE pointer, main fonksiyonuna gönderilir

}

//Kullanýcý string deðerlerini konsoldan girmek isterse
NODE* scanConsole(int threshold,int capacity)
{	
	char inputs[50];
	short n;	//Giriþ yapýlacak string sayýsý
	int i;
	
	printf("\nListeye kac eleman ekleyeceginizi giriniz : ");scanf("%d",&n);
	printf("\n");
	
	//Bu kýsým scanFile fonksiyonundaki kodlarda yazýlan mantýkla çalýþmaktadýr
	NODE* head;
	initHead(&head);
	if(checkCapacity(capacity))	//Kapasite kontrolü
	{
		deleteNode(&head);		//Kapasite aþýlýrsa node'u sil
	}
	
	scanf("%s",head->value);
	listNodes(head);
	
	for(i = 1; i < n; i++)
	{
		scanf("%s",inputs);
		
		if(searchNodes(&head,inputs,threshold))	//Girilen deðere eþit node var mý diye kontrol et
		{
			if(checkCapacity(capacity))
				deleteNode(&head);
				
			pushNode(&head);					//Eþit node bulunamamýþsa listeye yeni node'u ekle
			
			strcat(head->value,inputs);
		}
		
		listNodes(head);
	}
	
	return head;
	
}


//Dosya açma fonksiyonu
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

//Kullanýcýnýn isterse cache buffer'ý temizleyebilmesi için kullanýlan freeNodes fonksiyonu
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
	
	if(file_or_console)				//Dosya ile giriþ yapýlacaksa
	{
		FILE* fp = openFile();
		head = scanFile(fp,T,L);
		fclose(fp);
	}
	else							//Konsoldan giriþ yapýlacaksa
		head = scanConsole(T,L);
	
		
	printf("\nCache Buffer'i temizlemek istiyor musunuz ? [y\\n] ");
	scanf(" %c",&ans);
	if(ans == 'y')
		freeNodes(&head);			//Kullanýcý cache buffer'ý temizlemek isterse

	
	return 0;
}

