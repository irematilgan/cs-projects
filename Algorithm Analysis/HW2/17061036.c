#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <time.h>
#define esc 27
#define TABLE_SIZE 997

//HashTable struct'�nda yer alan h�creler ayr� bir structta tutulur
typedef struct CELL{
	char** docs; 		 //H�credeki kelimeyi i�eren d�k�manlar�n isimleri yer al�r
	long int hashedWord; //Horner's metodu ile elde edilen anahtar de�eri tutar
	short document_cursor; //H�credeki d�k�man say�s�n� tutan de�i�ken
}CELL;

//Hash tablosu struct'�
typedef struct HashTable{
	int M; 	//Tablo boyutu
	int N;	//Tablodaki kelime say�s�
	CELL* cells; //Tabloda kelimeleri tutan h�creler
	
}HashTable;


void search(char*, HashTable*);					//Hash tablosunda kelime arama fonksiyonu
HashTable* insertTable(char*,char*,HashTable*); //Hash tablosuna d�k�man ve d�k�man kelimelerini yerle�tiren fonksiyon
HashTable* createTable(); 						//HashTable struct'�n� ba�latan (initializing) fonksiyon
char *readDocument(char*);						//D�k�man dosyas�n� okuma fonksiyonu
HashTable* readTable(FILE*,HashTable*);			//hashTable.txt dosyas�ndaki verileri hash tablosuna yerle�tiren fonksiyon
long int hornersMethod(char*,int);				//Verili kelimeden anahtar de�er hesaplayan Horner's Y�ntemi
void updateFile(FILE*,HashTable*); 				//Hash tablosunu dosyaya yazan fonksiyon
char** createDocs();							//CELL struct'�nda tutulan d�k�manlar� initialize eden fonksiyon
int hash1(int);
int hash2(int);

int main() 
{
	char flag = 1; 	//Program�n istenmedi�i s�rece a��k kalmas�n� sa�layan de�i�ken
	char ch;		//�stenen i�lem t�r� i�in kullan�c�dan al�nan input'u tutan de�i�ken
	char docName[50], fileName[50], word[20]; //D�k�man ad�, dosya ad� (.txt uzant�l�), kullan�c�n� arad��� kelime
	HashTable* hashTable;
	HashTable* finalTable; //insertion i�leminden sonra olu�an yeni hash tablosu
	FILE* fp;
	
	do {
		printf("\n\t=== MAIN MENU ===");
		printf("\n\tOptions : \n");
		printf("\t1. If you want to insert a new word please type 0\n");
		printf("\t2. If you want to search for a word please type 1\n");
		printf("\n\tPress ESC to quit\n");
		ch = getch(); //Kullan�c� input'u al�n�r
		hashTable = createTable(); //Tablo ba�lat�l�r
		switch(ch)
		{
			//ESC tu�una bas�l�rsa program sonlan�r
			case esc:
				flag = 0;
				break;
			case '0':
				//Tabloya de�er yerle�tirilmesi istenirse
				printf("\n\tCase 1\n");
				printf("\tInsert document name here : ");
				scanf("%s",&docName);
				strcpy(fileName,docName);
				strcat(fileName,".txt");
				printf("\n\tDocument Name = %s.txt",docName);
				char* doc = (char*)readDocument(fileName); 
				//D�k�man dosyas� mevcut mu kontrol ediir
				if(doc != NULL)
				{
					finalTable = insertTable(doc,docName,hashTable); //D�k�mandaki kelimeler tabloya yerle�tirilir
					fp = fopen("hashTable.txt","w");
					updateFile(fp,hashTable);						//Son tablo dosya olarak yazd�r�l�r
					fclose(fp);					
				}
				else
					printf("\n\tFile does not exist!..");
				break;
			case '1':
				//D�k�manlarda kelime aramak istenirse
				printf("\tCase 2\n");
				printf("\tInsert word here : ");
				scanf("%s",&word);
				search(word,hashTable); //Kelime hash tablosunda arat�l�r
				break;
			default:
				printf("\n\tPlease type valid commands!..\n");
				break;
		}
	} 
	while(flag);

	return 0;
}

//D�k�man dosyas�n� okuma fonksiyonu
char *readDocument(char* docName)
{
	char text[10000];
	FILE* fp;
	if(fp = fopen(docName,"r")) //D�k�man mevcutsa
	{
		fread(text,sizeof(char)*(500*20),1,fp); //D�k�mandaki t�m kelimeler okunur
		fclose(fp);
		
		return text;
	}
	
	return NULL; //D�k�man mevcut de�ilse NULL d�n�l�r
}

//hashTable.txt dosyas�ndaki verileri hash tablosuna yerle�tiren fonksiyon
HashTable* readTable(FILE* fp, HashTable* hTable)
{
	float loadF; //load factor
	int adr;	//hash tablosu adresi de�i�keni
	long int key; //Horner's Metodu ile hesaplayaca��m�z anahtar de�erini tutan de�i�ken
	char line[200]; //Okunan sat�rlar� tutan char dizisi
	char *word; //strtok ve strtol ara i�lemleri i�in kullan�lan ge�ici char* de�i�keni
	char newstr[15]; //strtok ve strtol i�lemleri sonucu word'�n adresinde yer alan kelimenin kopyaland��� char dizisi
	char *tmp; //strtol i�lemi i�in ge�ici olu�turulan char pointer
	char* newline; //sat�rlardaki newline karakterlerin adresini tutan char pointer
	
	printf("\n\tREADING TABLE!..");
	fscanf(fp,"%lf",&loadF); //Tablo dosyas�ndaki ilk sat�r olan load factor de�eri okunur
	fscanf(fp,"%d %d",&hTable->N,&hTable->M); //�kinci sat�rda tablodaki eleman say�s� ve tablo boyutu okunur
	fscanf(fp,"\n");
	fgets(line,200,fp); //Buradan itibaren tablodaki her sat�r dosya sonuna kadar s�rayla okunur
	while(!feof(fp))
	{
		//newline ('\n') karakter i�eren adres bulunur ve '\0' olarak g�ncellenir
		newline = strchr(line,'\n');
		if(newline)
		  *newline = '\0';
		
		//Empty space karakteri ile elde edilen sat�r ayr��t�r�l�r, adres ve anahtar de�i�kenlerine atan�r
		word = strtok(line," ");
		strcpy(newstr,word);
		adr = strtol(word,&tmp,10);
		word = strtok(NULL," ");
		strcpy(newstr,word);
		key = strtol(word,&tmp,10);
		
		//adreste ka� d�k�man oldu�unu tutan document_cursor de�i�keni s�f�rlan�r
		hTable->cells[adr].document_cursor = 0;
		
		//Hash Tablosunun verilen adresindeki d�k�manlar i�in yer a��l�r
		hTable->cells[adr].docs = createDocs();
		
		word = strtok(NULL," ");
		while(word != NULL) //Empty space karakteri ile ayr�lacak kelime kalmayana dek ayr��t�r
		{
			strcpy(newstr,word);
			strcpy(hTable->cells[adr].docs[hTable->cells[adr].document_cursor],newstr); //Hash tablosuna d�k�man ad� yerle�tirilir
			(hTable->cells[adr].document_cursor)++; //D�k�man say�s� artar
			word = strtok(NULL," ");
		}
		
		//kelimenin Horner's Method ile olu�turulmu� anahtar� yerle�tir
		hTable->cells[adr].hashedWord = key;
		
		//Gelecek sat�r� oku
		fgets(line,200,fp);
	}
	printf("\n\tREADING IS DONE!..");
	return hTable;
}


//HashTable struct'�n� ba�latan (initializing) fonksiyon
HashTable* createTable() 
{
	HashTable* ht = (HashTable*)malloc(sizeof(HashTable));
	ht->M = TABLE_SIZE; //Tablo boyutu atan�r
	ht->N = 0;			//Tablodaki kelime say�s� s�f�rlan�r
	ht->cells = (CELL*)malloc(sizeof(CELL)*TABLE_SIZE); //Tablo h�creleri i�in yer ayr�l�r
	
	int i;
	for(i = 0; i < 997; i++) {
		//Tablodaki her bir h�cre hen�z de�er yerle�tirilmedi�ini belirtmek �zere -1 ile i�aretlenir
		ht->cells[i].hashedWord = -1; 
	}
	
		
	return ht;
		
}

//CELL struct'�nda tutulan d�k�manlar� initialize eden fonksiyon
char** createDocs()
{
	int i;
	//Her h�crede 50 d�k�manl�k yer olu�turulur ve her bir d�k�man ismi en fazla
	//20 karakter olacak �ekilde ayarlan�r
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
	
	//Hash tablosu dosyas�n�n varl��� kontrol edilir
	if(fp = fopen("hashTable.txt","r")) {
		char** tmp;		//Ge�ici olarak d�k�man adlar�n� tutan string dizisi
		short cursor;	//D�k�man string'i i�erisinde ilerlerken kullanaca��m�z indis de�i�keni
		int k = 0;		//Collision olma durumunda yeni adresi bulurken d�zenli olarak artt�raca��m�z de�i�ken
		hTable = readTable(fp,hashTable); //Belgede yer alan hash tablosu HashTable struct'�na aktar�l�r
		
		//Horner's Y�ntemi ile kelimenin anahtar� olu�turulur
		long int key = hornersMethod(word,strlen(word));
		//Double hashing ile yeni adres hesaplan�r
		int hash = (hash1(key) + k*hash2(key))%(hashTable->M);
		
		if(hashTable->cells[hash].hashedWord != -1) //Adres bo� de�ilse 
		{
			//T�m tablo dola��lmad��� ve buldu�umuz adresteki kelime ile arad���m�z kelime e�le�medi�i s�rece
			//k de�i�kenini artt�rmaya ve yeni adres de�erini hesaplamaya devam et
			while(k < hashTable->M && hashTable->cells[hash].hashedWord != key) 
			{
				k++;
				hash = (hash1(key) + k*hash2(key))%(hashTable->M);
			}
					
			if(k < hashTable->M) //Kelime bulunduysa
			{
				printf("\n\tWORD FOUND!..TOTAL STEPS = %d",k+1); //Kelimenin ka� ad�m sonra bulundu�unu yazd�r
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
		else //Adreste bir kelime yer alm�yorsa kelime tabloda yok demektir
		{
			printf("\n\tThis word does not exist in the document!..");
		}
		fclose(fp);
	} 
	else //Tablo dosyas� bulunamad�ysa
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

//Hash tablosuna d�k�man ve d�k�man kelimelerini yerle�tiren fonksiyon
HashTable* insertTable(char* document, char* documentName, HashTable* hashTable)
{
	clock_t begin = clock();
	FILE* fp;			
	HashTable* hTable;	
	int i = 0;			
	int j = 0;
	int k = 0;			//Collision olma durumunda yeni adresi bulurken d�zenli olarak artt�raca��m�z de�i�ken
	int cursor;			//D�k�man string dizisi i�erisinde ilerlerken kullanaca��m�z indis de�i�keni  
	long int key;		//Horner's Metodu ile hesaplayaca��m�z anahtar de�erini tutan de�i�ken
	char word[50];		//Okunacak d�k�mandaki kelimeleri tutan char dizisi
	int hash;
	float loadF = 0;
	
	//Tablo dosyas� mevcutsa dosya okunarak HashTable struct'�na aktar�l�r
	if(fp = fopen("hashTable.txt","r")) {
		hTable = readTable(fp,hashTable);
		fclose(fp);
	} 
	
	//D�k�man�n sonuna kadar harfleri oku
	while(document[i] != '\0')
	{
		//Her bo�luk veya newline'da kelimeyi al
		if(document[i] == ' ' || document[i] == '\n'){
			
			word[j] = '\0';
			key = hornersMethod(word,j); //Kelimeden anahtar ��kar�l�r
			hash = (hash1(key) + k*hash2(key))%(hashTable->M); //Double hashing ile adres hesaplan�r
			k+=1;
			
			//load factor 1 olmad��� s�rece tablo �zerinde g�ncellemeler yap�l�r
			if(loadF < 1)
			{
				//T�m tablo dola��lmad���, bulunan adres bo� olmad��� ve buldu�umuz adresteki kelime ile arad���m�z kelime e�le�medi�i s�rece
				//k de�i�kenini artt�rmaya ve yeni adres de�erini hesaplamaya devam et
				while(k < hashTable->M && hashTable->cells[hash].hashedWord != -1 && key != hashTable->cells[hash].hashedWord) //Collision'dan ka��nmak i�in
				{
					hash = (hash1(key) + k*hash2(key))%(hashTable->M);
					k+=1;
				}
				
				if(k < hashTable->M) //Bo� yer ya da ayn� kelime bulunmu�sa
				{
				
					if(hashTable->cells[hash].hashedWord == -1) //Bo� yer bulunduysa
					{
						//Bo� h�creye yerle�tirilecek kelimenin anahtar de�eri, ve kelimeyi i�eren d�k�man�n ad� yerle�tirilir
						hashTable->cells[hash].hashedWord = key;
						hashTable->cells[hash].document_cursor = 1; //H�credeki d�k�man say�s� g�ncellenir
						hashTable->cells[hash].docs = createDocs(); //H�creye d�k�man isimlerinin yerle�tirilmesi i�in yer a��l�r
						strcpy(hashTable->cells[hash].docs[0],documentName); //D�k�man ismi ilk d�k�man olarak yerle�tirilir

						(hashTable->N)++; //Tabloda yer alan kelime say�s� artt�r�l�r
						loadF = (float)(hashTable->N)/(hashTable->M); //Load Factor hesaplan�r
						
						printf("\n\t");
						if(loadF > 0.8)	//Load Factor 0.8'den b�y�kse uyar� ver
							printf("WARNING..");
								
						printf("LOAD FACTOR = %lf",loadF);
						if(loadF >= 1) //Load Factor 1 olursa d�k�manda bundan sonraki kelimelerin eklenemeyece�ini haber ver
						{
							printf("\n\tTABLE IS FULL!..");
							printf("\n\tDISCARDED WORDS = ");
						}
	
					}
					else //Adreste yer alan ayn� kelime d�k�mandaki kelimeyle e�le�iyorsa
					{
						
						cursor = 0;
						//D�k�man isminin h�crede yer alan d�k�manlar aras�nda olup olmad���n� kontrol et
						while(cursor < hashTable->cells[hash].document_cursor && (strcmp(documentName,hashTable->cells[hash].docs[cursor])))
						{
							cursor++;
						}
							
						if(cursor >= hashTable->cells[hash].document_cursor) //D�k�man h�crede bulunmad�ysa ismini d�k�man dizisine ekle
						{
							strcpy(hashTable->cells[hash].docs[hashTable->cells[hash].document_cursor],documentName);
							(hashTable->cells[hash].document_cursor)++; //D�k�man say�s�n�n kayd�n� artt�r
						}
							
	
					}
				}
				else
				{
					printf("\n\tThere is no available space for word %s in hash table",word);
				}
			}
			else //Load factor 1 ise eklenemeyen kelimeler yazd�r�l�r
			{
				printf("\n\t%s",word);
			}
			
			
			k = 0;
			j = 0;
			i++;
			
		}
		if(document[i] <= 90) //Upper case harf varsa lower case'e �evrilerek aktar�l�r
			word[j++] = document[i++] - ('A'-'a');
		else
			word[j++] = document[i++];
		
	}
	
	clock_t end = clock();	
	double time_spent = (double)(end-begin) / CLOCKS_PER_SEC; //fonksiyonun �al��ma s�resini yazd�r
	printf("\n\tINSERTION TIME = %lf\n",time_spent);
	


	return hashTable;
}

//Hash tablosunu dosyaya yazan fonksiyon
void updateFile(FILE* fp, HashTable* hashTable)
{
	clock_t begin = clock();
	char** tmp; //Ge�ici olarak d�k�man adlar�n� tutan string dizisi
	short cursor;	//D�k�man string'i i�erisinde ilerlerken kullanaca��m�z indis de�i�keni
	float loadF = (float)(hashTable->N)/(hashTable->M); //load factor hesaplan�r
	fprintf(fp,"%lf\n",loadF); //�lk sat�rda load factor yer al�r
	fprintf(fp,"%d %d\n",hashTable->N,hashTable->M); //Dosyan�n ikinci sat�r�na tablodaki kelime say�s� ve tablo boyutu yazd�r�l�r
	
	int i;
	for(i = 0 ; i < TABLE_SIZE; i++)
	{
		//H�cre bo� de�ilse dosyaya h�crenin indisi, kelimenin Horner's Metodu ile hesaplanm�� anahtar de�eri ve
		//kelimeyi i�eren d�k�manlar s�ras�yla sat�r sat�r dosyaya yazd�r�l�r
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
	double time_spent = (double)(end-begin) / CLOCKS_PER_SEC;  //fonksiyonun �al��ma s�resini yazd�r
	printf("\n\tWRITING TIME = %lf\n",time_spent);
}

//Verili kelimeden anahtar de�er hesaplayan Horner's Y�ntemi
long int hornersMethod(char* word, int length)
{
	long int key = 0;
	int i;
	int r = 31; //asal say�

	i = length;
	while(i >= 0)
	{
		key += word[i]*pow(r,i);
		key = key & 0x7FFFFFFF; //Overflow olmas�n� engellemek i�in
		i--;
	}
	
	return key;
}
