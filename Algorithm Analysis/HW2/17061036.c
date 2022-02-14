#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <time.h>
#define esc 27
#define TABLE_SIZE 997

//HashTable struct'ýnda yer alan hücreler ayrý bir structta tutulur
typedef struct CELL{
	char** docs; 		 //Hücredeki kelimeyi içeren dökümanlarýn isimleri yer alýr
	long int hashedWord; //Horner's metodu ile elde edilen anahtar deðeri tutar
	short document_cursor; //Hücredeki döküman sayýsýný tutan deðiþken
}CELL;

//Hash tablosu struct'ý
typedef struct HashTable{
	int M; 	//Tablo boyutu
	int N;	//Tablodaki kelime sayýsý
	CELL* cells; //Tabloda kelimeleri tutan hücreler
	
}HashTable;


void search(char*, HashTable*);					//Hash tablosunda kelime arama fonksiyonu
HashTable* insertTable(char*,char*,HashTable*); //Hash tablosuna döküman ve döküman kelimelerini yerleþtiren fonksiyon
HashTable* createTable(); 						//HashTable struct'ýný baþlatan (initializing) fonksiyon
char *readDocument(char*);						//Döküman dosyasýný okuma fonksiyonu
HashTable* readTable(FILE*,HashTable*);			//hashTable.txt dosyasýndaki verileri hash tablosuna yerleþtiren fonksiyon
long int hornersMethod(char*,int);				//Verili kelimeden anahtar deðer hesaplayan Horner's Yöntemi
void updateFile(FILE*,HashTable*); 				//Hash tablosunu dosyaya yazan fonksiyon
char** createDocs();							//CELL struct'ýnda tutulan dökümanlarý initialize eden fonksiyon
int hash1(int);
int hash2(int);

int main() 
{
	char flag = 1; 	//Programýn istenmediði sürece açýk kalmasýný saðlayan deðiþken
	char ch;		//Ýstenen iþlem türü için kullanýcýdan alýnan input'u tutan deðiþken
	char docName[50], fileName[50], word[20]; //Döküman adý, dosya adý (.txt uzantýlý), kullanýcýný aradýðý kelime
	HashTable* hashTable;
	HashTable* finalTable; //insertion iþleminden sonra oluþan yeni hash tablosu
	FILE* fp;
	
	do {
		printf("\n\t=== MAIN MENU ===");
		printf("\n\tOptions : \n");
		printf("\t1. If you want to insert a new word please type 0\n");
		printf("\t2. If you want to search for a word please type 1\n");
		printf("\n\tPress ESC to quit\n");
		ch = getch(); //Kullanýcý input'u alýnýr
		hashTable = createTable(); //Tablo baþlatýlýr
		switch(ch)
		{
			//ESC tuþuna basýlýrsa program sonlanýr
			case esc:
				flag = 0;
				break;
			case '0':
				//Tabloya deðer yerleþtirilmesi istenirse
				printf("\n\tCase 1\n");
				printf("\tInsert document name here : ");
				scanf("%s",&docName);
				strcpy(fileName,docName);
				strcat(fileName,".txt");
				printf("\n\tDocument Name = %s.txt",docName);
				char* doc = (char*)readDocument(fileName); 
				//Döküman dosyasý mevcut mu kontrol ediir
				if(doc != NULL)
				{
					finalTable = insertTable(doc,docName,hashTable); //Dökümandaki kelimeler tabloya yerleþtirilir
					fp = fopen("hashTable.txt","w");
					updateFile(fp,hashTable);						//Son tablo dosya olarak yazdýrýlýr
					fclose(fp);					
				}
				else
					printf("\n\tFile does not exist!..");
				break;
			case '1':
				//Dökümanlarda kelime aramak istenirse
				printf("\tCase 2\n");
				printf("\tInsert word here : ");
				scanf("%s",&word);
				search(word,hashTable); //Kelime hash tablosunda aratýlýr
				break;
			default:
				printf("\n\tPlease type valid commands!..\n");
				break;
		}
	} 
	while(flag);

	return 0;
}

//Döküman dosyasýný okuma fonksiyonu
char *readDocument(char* docName)
{
	char text[10000];
	FILE* fp;
	if(fp = fopen(docName,"r")) //Döküman mevcutsa
	{
		fread(text,sizeof(char)*(500*20),1,fp); //Dökümandaki tüm kelimeler okunur
		fclose(fp);
		
		return text;
	}
	
	return NULL; //Döküman mevcut deðilse NULL dönülür
}

//hashTable.txt dosyasýndaki verileri hash tablosuna yerleþtiren fonksiyon
HashTable* readTable(FILE* fp, HashTable* hTable)
{
	float loadF; //load factor
	int adr;	//hash tablosu adresi deðiþkeni
	long int key; //Horner's Metodu ile hesaplayacaðýmýz anahtar deðerini tutan deðiþken
	char line[200]; //Okunan satýrlarý tutan char dizisi
	char *word; //strtok ve strtol ara iþlemleri için kullanýlan geçici char* deðiþkeni
	char newstr[15]; //strtok ve strtol iþlemleri sonucu word'ün adresinde yer alan kelimenin kopyalandýðý char dizisi
	char *tmp; //strtol iþlemi için geçici oluþturulan char pointer
	char* newline; //satýrlardaki newline karakterlerin adresini tutan char pointer
	
	printf("\n\tREADING TABLE!..");
	fscanf(fp,"%lf",&loadF); //Tablo dosyasýndaki ilk satýr olan load factor deðeri okunur
	fscanf(fp,"%d %d",&hTable->N,&hTable->M); //Ýkinci satýrda tablodaki eleman sayýsý ve tablo boyutu okunur
	fscanf(fp,"\n");
	fgets(line,200,fp); //Buradan itibaren tablodaki her satýr dosya sonuna kadar sýrayla okunur
	while(!feof(fp))
	{
		//newline ('\n') karakter içeren adres bulunur ve '\0' olarak güncellenir
		newline = strchr(line,'\n');
		if(newline)
		  *newline = '\0';
		
		//Empty space karakteri ile elde edilen satýr ayrýþtýrýlýr, adres ve anahtar deðiþkenlerine atanýr
		word = strtok(line," ");
		strcpy(newstr,word);
		adr = strtol(word,&tmp,10);
		word = strtok(NULL," ");
		strcpy(newstr,word);
		key = strtol(word,&tmp,10);
		
		//adreste kaç döküman olduðunu tutan document_cursor deðiþkeni sýfýrlanýr
		hTable->cells[adr].document_cursor = 0;
		
		//Hash Tablosunun verilen adresindeki dökümanlar için yer açýlýr
		hTable->cells[adr].docs = createDocs();
		
		word = strtok(NULL," ");
		while(word != NULL) //Empty space karakteri ile ayrýlacak kelime kalmayana dek ayrýþtýr
		{
			strcpy(newstr,word);
			strcpy(hTable->cells[adr].docs[hTable->cells[adr].document_cursor],newstr); //Hash tablosuna döküman adý yerleþtirilir
			(hTable->cells[adr].document_cursor)++; //Döküman sayýsý artar
			word = strtok(NULL," ");
		}
		
		//kelimenin Horner's Method ile oluþturulmuþ anahtarý yerleþtir
		hTable->cells[adr].hashedWord = key;
		
		//Gelecek satýrý oku
		fgets(line,200,fp);
	}
	printf("\n\tREADING IS DONE!..");
	return hTable;
}


//HashTable struct'ýný baþlatan (initializing) fonksiyon
HashTable* createTable() 
{
	HashTable* ht = (HashTable*)malloc(sizeof(HashTable));
	ht->M = TABLE_SIZE; //Tablo boyutu atanýr
	ht->N = 0;			//Tablodaki kelime sayýsý sýfýrlanýr
	ht->cells = (CELL*)malloc(sizeof(CELL)*TABLE_SIZE); //Tablo hücreleri için yer ayrýlýr
	
	int i;
	for(i = 0; i < 997; i++) {
		//Tablodaki her bir hücre henüz deðer yerleþtirilmediðini belirtmek üzere -1 ile iþaretlenir
		ht->cells[i].hashedWord = -1; 
	}
	
		
	return ht;
		
}

//CELL struct'ýnda tutulan dökümanlarý initialize eden fonksiyon
char** createDocs()
{
	int i;
	//Her hücrede 50 dökümanlýk yer oluþturulur ve her bir döküman ismi en fazla
	//20 karakter olacak þekilde ayarlanýr
	char** docs = (char**)malloc(sizeof(char*)*50);
	for(i = 0; i < 50; i++)
		docs[i] = (char*)malloc(sizeof(char)*20);
		
	return docs;
}

//Hash tablosunda kelime arama fonksiyonu
void search(char* word, HashTable* hashTable)
{
	FILE* fp;
	HashTable* hTable;
	
	//Hash tablosu dosyasýnýn varlýðý kontrol edilir
	if(fp = fopen("hashTable.txt","r")) {
		char** tmp;		//Geçici olarak döküman adlarýný tutan string dizisi
		short cursor;	//Döküman string'i içerisinde ilerlerken kullanacaðýmýz indis deðiþkeni
		int k = 0;		//Collision olma durumunda yeni adresi bulurken düzenli olarak arttýracaðýmýz deðiþken
		hTable = readTable(fp,hashTable); //Belgede yer alan hash tablosu HashTable struct'ýna aktarýlýr
		
		//Horner's Yöntemi ile kelimenin anahtarý oluþturulur
		long int key = hornersMethod(word,strlen(word));
		//Double hashing ile yeni adres hesaplanýr
		int hash = (hash1(key) + k*hash2(key))%(hashTable->M);
		
		if(hashTable->cells[hash].hashedWord != -1) //Adres boþ deðilse 
		{
			//Tüm tablo dolaþýlmadýðý ve bulduðumuz adresteki kelime ile aradýðýmýz kelime eþleþmediði sürece
			//k deðiþkenini arttýrmaya ve yeni adres deðerini hesaplamaya devam et
			while(k < hashTable->M && hashTable->cells[hash].hashedWord != key) 
			{
				k++;
				hash = (hash1(key) + k*hash2(key))%(hashTable->M);
			}
					
			if(k < hashTable->M) //Kelime bulunduysa
			{
				printf("\n\tWORD FOUND!..TOTAL STEPS = %d",k+1); //Kelimenin kaç adým sonra bulunduðunu yazdýr
				printf("\n\tIN DOCUMENTS : ");
				tmp = hashTable->cells[hash].docs;
				for(cursor = 0; cursor < hashTable->cells[hash].document_cursor; cursor++)
					printf("%s\t",tmp[cursor]);
				
				printf("\n");
			}
			else
			{
				printf("\n\tThis word does not exist in the document!..");
			}
			
			
		}
		else //Adreste bir kelime yer almýyorsa kelime tabloda yok demektir
		{
			printf("\n\tThis word does not exist in the document!..");
		}
		fclose(fp);
	} 
	else //Tablo dosyasý bulunamadýysa
	{
		printf("\n\tHASH TABLE NOT FOUND!..");
	}
}

int hash1(int key)
{
	return key%(TABLE_SIZE);
}

int hash2(int key)
{
	return (1+(key%(TABLE_SIZE-1)));
}

//Hash tablosuna döküman ve döküman kelimelerini yerleþtiren fonksiyon
HashTable* insertTable(char* document, char* documentName, HashTable* hashTable)
{
	clock_t begin = clock();
	FILE* fp;			
	HashTable* hTable;	
	int i = 0;			
	int j = 0;
	int k = 0;			//Collision olma durumunda yeni adresi bulurken düzenli olarak arttýracaðýmýz deðiþken
	int cursor;			//Döküman string dizisi içerisinde ilerlerken kullanacaðýmýz indis deðiþkeni  
	long int key;		//Horner's Metodu ile hesaplayacaðýmýz anahtar deðerini tutan deðiþken
	char word[50];		//Okunacak dökümandaki kelimeleri tutan char dizisi
	int hash;
	float loadF = 0;
	
	//Tablo dosyasý mevcutsa dosya okunarak HashTable struct'ýna aktarýlýr
	if(fp = fopen("hashTable.txt","r")) {
		hTable = readTable(fp,hashTable);
		fclose(fp);
	} 
	
	//Dökümanýn sonuna kadar harfleri oku
	while(document[i] != '\0')
	{
		//Her boþluk veya newline'da kelimeyi al
		if(document[i] == ' ' || document[i] == '\n'){
			
			word[j] = '\0';
			key = hornersMethod(word,j); //Kelimeden anahtar çýkarýlýr
			hash = (hash1(key) + k*hash2(key))%(hashTable->M); //Double hashing ile adres hesaplanýr
			k+=1;
			
			//load factor 1 olmadýðý sürece tablo üzerinde güncellemeler yapýlýr
			if(loadF < 1)
			{
				//Tüm tablo dolaþýlmadýðý, bulunan adres boþ olmadýðý ve bulduðumuz adresteki kelime ile aradýðýmýz kelime eþleþmediði sürece
				//k deðiþkenini arttýrmaya ve yeni adres deðerini hesaplamaya devam et
				while(k < hashTable->M && hashTable->cells[hash].hashedWord != -1 && key != hashTable->cells[hash].hashedWord) //Collision'dan kaçýnmak için
				{
					hash = (hash1(key) + k*hash2(key))%(hashTable->M);
					k+=1;
				}
				
				if(k < hashTable->M) //Boþ yer ya da ayný kelime bulunmuþsa
				{
				
					if(hashTable->cells[hash].hashedWord == -1) //Boþ yer bulunduysa
					{
						//Boþ hücreye yerleþtirilecek kelimenin anahtar deðeri, ve kelimeyi içeren dökümanýn adý yerleþtirilir
						hashTable->cells[hash].hashedWord = key;
						hashTable->cells[hash].document_cursor = 1; //Hücredeki döküman sayýsý güncellenir
						hashTable->cells[hash].docs = createDocs(); //Hücreye döküman isimlerinin yerleþtirilmesi için yer açýlýr
						strcpy(hashTable->cells[hash].docs[0],documentName); //Döküman ismi ilk döküman olarak yerleþtirilir

						(hashTable->N)++; //Tabloda yer alan kelime sayýsý arttýrýlýr
						loadF = (float)(hashTable->N)/(hashTable->M); //Load Factor hesaplanýr
						
						printf("\n\t");
						if(loadF > 0.8)	//Load Factor 0.8'den büyükse uyarý ver
							printf("WARNING..");
								
						printf("LOAD FACTOR = %lf",loadF);
						if(loadF >= 1) //Load Factor 1 olursa dökümanda bundan sonraki kelimelerin eklenemeyeceðini haber ver
						{
							printf("\n\tTABLE IS FULL!..");
							printf("\n\tDISCARDED WORDS = ");
						}
	
					}
					else //Adreste yer alan ayný kelime dökümandaki kelimeyle eþleþiyorsa
					{
						
						cursor = 0;
						//Döküman isminin hücrede yer alan dökümanlar arasýnda olup olmadýðýný kontrol et
						while(cursor < hashTable->cells[hash].document_cursor && (strcmp(documentName,hashTable->cells[hash].docs[cursor])))
						{
							cursor++;
						}
							
						if(cursor >= hashTable->cells[hash].document_cursor) //Döküman hücrede bulunmadýysa ismini döküman dizisine ekle
						{
							strcpy(hashTable->cells[hash].docs[hashTable->cells[hash].document_cursor],documentName);
							(hashTable->cells[hash].document_cursor)++; //Döküman sayýsýnýn kaydýný arttýr
						}
							
	
					}
				}
				else
				{
					printf("\n\tThere is no available space for word %s in hash table",word);
				}
			}
			else //Load factor 1 ise eklenemeyen kelimeler yazdýrýlýr
			{
				printf("\n\t%s",word);
			}
			
			
			k = 0;
			j = 0;
			i++;
			
		}
		if(document[i] <= 90) //Upper case harf varsa lower case'e çevrilerek aktarýlýr
			word[j++] = document[i++] - ('A'-'a');
		else
			word[j++] = document[i++];
		
	}
	
	clock_t end = clock();	
	double time_spent = (double)(end-begin) / CLOCKS_PER_SEC; //fonksiyonun çalýþma süresini yazdýr
	printf("\n\tINSERTION TIME = %lf\n",time_spent);
	


	return hashTable;
}

//Hash tablosunu dosyaya yazan fonksiyon
void updateFile(FILE* fp, HashTable* hashTable)
{
	clock_t begin = clock();
	char** tmp; //Geçici olarak döküman adlarýný tutan string dizisi
	short cursor;	//Döküman string'i içerisinde ilerlerken kullanacaðýmýz indis deðiþkeni
	float loadF = (float)(hashTable->N)/(hashTable->M); //load factor hesaplanýr
	fprintf(fp,"%lf\n",loadF); //Ýlk satýrda load factor yer alýr
	fprintf(fp,"%d %d\n",hashTable->N,hashTable->M); //Dosyanýn ikinci satýrýna tablodaki kelime sayýsý ve tablo boyutu yazdýrýlýr
	
	int i;
	for(i = 0 ; i < TABLE_SIZE; i++)
	{
		//Hücre boþ deðilse dosyaya hücrenin indisi, kelimenin Horner's Metodu ile hesaplanmýþ anahtar deðeri ve
		//kelimeyi içeren dökümanlar sýrasýyla satýr satýr dosyaya yazdýrýlýr
		if(hashTable->cells[i].hashedWord != -1) 
		{
			tmp = hashTable->cells[i].docs;
			fprintf(fp,"%d %d",i,hashTable->cells[i].hashedWord);
			for(cursor = 0; cursor < hashTable->cells[i].document_cursor; cursor++)
				fprintf(fp," %s",tmp[cursor]);
			
			fprintf(fp,"\n");
		}
			
	}
	printf("\n\tWRITING COMPLETED!..");
	clock_t end = clock();	
	double time_spent = (double)(end-begin) / CLOCKS_PER_SEC;  //fonksiyonun çalýþma süresini yazdýr
	printf("\n\tWRITING TIME = %lf\n",time_spent);
}

//Verili kelimeden anahtar deðer hesaplayan Horner's Yöntemi
long int hornersMethod(char* word, int length)
{
	long int key = 0;
	int i;
	int r = 31; //asal sayý

	i = length;
	while(i >= 0)
	{
		key += word[i]*pow(r,i);
		key = key & 0x7FFFFFFF; //Overflow olmasýný engellemek için
		i--;
	}
	
	return key;
}
