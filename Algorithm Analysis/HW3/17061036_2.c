#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <time.h>
#define TABLE_SIZE 997

//HashTable struct'ýnda yer alan hücreler ayrý bir structta tutulur
typedef struct CELL{
	char word[20]; 		 //Tutulan kelime
	long int hashedWord; //Horner's metodu ile kelimeden elde edilen anahtar deðer
}CELL;

//Hash tablosu struct'ý
typedef struct HashTable{
	CELL* cells; //Tabloda kelimeleri tutan hücreler
}HashTable;


HashTable* insertTable(char*,HashTable*); 		//Hash tablosuna döküman ve döküman kelimelerini yerleþtiren fonksiyon
HashTable* createTable(); 						//HashTable struct'ýný baþlatan (initializing) fonksiyon
char *readDocument(char*,char*);				//Sozluk dosyasýný okuma fonksiyonu
int search(char*,HashTable*);					//HashTable struct'ýnda kelime arama fonksiyonu
long int hornersMethod(char*,int);				//Verili kelimeden anahtar deðer hesaplayan Horner's Yöntemi
int hash1(int);
int hash2(int);
int** create2DMatrix(int,int);					//2 Boyutlu matris olusturan fonksiyon
char* recommendWords(char*,HashTable*);			//Levenshtein Edit Distance algoritmasi ile kullanicilara hatali kelime icin onerilebilir kelimeler sunan fonksiyon
HashTable* insertCrookedTable(char*,char*,HashTable*);	//Hatalý kelimelerin dogrularinin yer aldigi hash tablosuna kelime yerlestirme fonksiyonu
int findDif(char,char); 						//Parametre olarak verilen harflerin ayni olup olmadigini kontrol eden fonksiyon
int findMin(int,int,int);						//LED algoritmasinda 3 secenek icerisinden minimum degeri bulan fonksiyon
void printHT(HashTable*);						//Hash tablosunu yazdirma fonksiyonu
char* convertUpperCase(char*);					//Kelime harflerini upper case'e ceviren fonksiyon
int LED(char*,char*);							//Levenshtein Edit Distance Algoritmasi


int main()
{
	char text[10000];	//Verilen smalldictionary.txt dosyasý icerigini tutan char dizisi
	char sentence[50];		//Aranacak kelimenin tutuldugu degisken
	char* result_text;	//Okunan dosya iceriginin yer aldigi char dizisinin adresini tutan pointer degisken
	char* right_word;	//recommendWords fonksiyonu ile kullanýcýnýn sectigi onerilen kelimeyi tutan degisken
	char *splitted_word; //strtok iþlemleri için kullanýlan geçici char* deðiþkeni
	char newstr[20];	//strtok ile elde edilen kelimeleri tutan char dizisi
	int flag = 1;		//Programýn devamlýlýgýný saglayan bayrak degeri tutan degisken
	result_text = readDocument("smalldictionary.txt",text);	//Dosyadaki icerik okunut
	HashTable* dictionary = createTable();	//Hash Tablosu olusturulur
	dictionary = insertTable(text,dictionary);	//Okunan kelimeler hash tablosuna eklenir
	HashTable* crookedWords = createTable();	//Hatali kelimelerin yer aldigi hash tablosu olusturulur
	do
	{
		printf("\n\t=== MAIN MENU ===");
		printf("\n\tOptions : \n");
		printf("\tIf you want to search for a word please type : ");
		gets(sentence);
		splitted_word = strtok(sentence," ");
		if(splitted_word != NULL)	//Kullanicinin girdigi kelime okunur, input alýnýp alinmadigi kontrol edilir
		{
			while(splitted_word != NULL)
			{
				strcpy(newstr,splitted_word);
				if((!search(newstr,dictionary)) && (!search(newstr,crookedWords)))	//Sozluk tablosunda ve hatali kelime tablosunda kelimenin olup olmadigina bakilir
				{
					//Kelime hicbir yerde yer almýyorsa
					//Levenshtein algoritmasi ile hatali kelimeye en yakýn kelimeler kullaniciya onerilir ve kullanicinin tercih ettigi kelime geri donulur
					right_word = recommendWords(newstr,dictionary);	
					if(right_word != NULL) //Onerilebilir kelime varsa
					{
						//Dogru kelime hatali kelime tablosuna yerlestirilir
						crookedWords = insertCrookedTable(right_word,newstr,crookedWords);
					}	
				}
				splitted_word = strtok(NULL," ");
			}
			getchar(); //Kullanýcýnýn önerilen kelimeyi yazmasýyla gelen '\n' karakteri getchar ile alýnýr 
		}
		else{
			printf("\n\tInvalid Input!..");	//Eger kullanicidan input alinmazsa program durdurulur
			flag = 0;
		}
			
	}
	while(flag);

	return 0;
}

//2 Boyutlu matris olusturan fonksiyon
int** create2DMatrix(int dim1,int dim2)
{
	int i;
	int **matrix;
	matrix = calloc(dim1,sizeof(int*));
	for(i = 0; i < dim1; i++)
	{
		matrix[i] = calloc(dim2,sizeof(int));
	}
	
	return matrix;
}

//Parametre olarak verilen harflerin ayni olup olmadigini kontrol eden fonksiyon
int findDif(char a, char b)
{
	if(a > 96)	a = a - ('a'-'A');
	if(b > 96)	b = b - ('b'-'B');
	if(a-b)	return 1;
	return 0;
}

//LED algoritmasinda 3 secenek icerisinden minimum degeri bulan fonksiyon
int findMin(int opt1, int opt2, int opt3)
{
	if(opt1 < opt2 && opt1 < opt3)	return opt1;
	else
	{
		if(opt2 < opt1 && opt2 < opt3)	return opt2;
		else	return opt3;
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
		key = key & 0x7FFFFFFF; //Overflow olmasýný engellemek icin
		i--;
	}
	
	return key;
}

//Levenshtein Edit Distance Algoritmasi
int LED(char* ref, char* old)
{
	int i;
	int j;
	//Aralarindan minimumu bulunacak olan 3 uzakligin tutuldugu degisken
	int opt1,opt2,opt3;		
	int M = strlen(ref);
	int N = strlen(old);	
	int** mat = create2DMatrix(M+1,N+1); //Dinamik programlama icin matris olusturulur
	i = 0;
	while(i <= M)	
	{
		for(j = 0; j <= N; j++)
		{
			if(i == 0 && j == 0)	
				mat[i][j] = 0;
			else if(j == 0)	
				mat[i][j] = mat[i-1][j] + 1;
			else if(i == 0)	
				mat[i][j] = mat[i][j-1] + 1;
			else
			{
				opt1 = mat[i-1][j] + 1;
				opt2 = mat[i][j-1] + 1;
				opt3 = mat[i-1][j-1] + findDif(ref[i-1],old[j-1]);	//harflerin ayni olup olmadigi hesaplanarak opt3 e eklenir
				mat[i][j] = findMin(opt1,opt2,opt3);				//Seceneklerden minimum distance'i veren deger secilir
			}
		}
		i++;
	}
	return mat[M][N];	//Sonuc distance'i donulur
}

//Hash tablosunu yazdirma fonksiyonu
void printHT(HashTable* hashTable)
{
	int i;
	for(i = 0; i < TABLE_SIZE; i++)
	{
		printf("%d\t%s\n",i, hashTable->cells[i].word);
	}
}

//Kelime harflerini upper case'e ceviren fonksiyon
char* convertUpperCase(char* word)
{
	int i;
	char tmp_word[20];

	char* temp = calloc(20,sizeof(char));
	for(i = 0; word[i] != '\0'; i++)
	{
		
		if(word[i] > 96)	//lower case harf ise
			temp[i] = word[i] - ('a'-'A');
		else
			temp[i] = word[i];

	}
	temp[i] = '\0';

	return temp;
}

//HashTable struct'ýnda kelime arama fonksiyonu
int search(char* word, HashTable* ht)
{
	long int key;		//Horner's Metodu ile hesaplayacaðýmýz anahtar deðerini tutan deðiþken
	int hash;			//Hashing sonucu hesaplanacak adres degeri
	int k = 0;			//Collision olma durumunda yeni adresi bulurken düzenli olarak arttýracaðýmýz deðiþken
	char* converted; 	//Upper case'e donusecek hatali kelime
	
	//Aranan kelimenin harfleri upper case yapilir
	converted = convertUpperCase(word);
	//Horner's Yöntemi ile kelimenin anahtarý oluþturulur
	key = hornersMethod(converted,strlen(word));
	//Double hashing ile yeni adres hesaplanýr
	hash = (hash1(key) + k*hash2(key))%(TABLE_SIZE);
	
	//Tum tablo dolasilana ya da aranan kelime bulunana kadar tabloda arama yapilir
	while(k < TABLE_SIZE && ht->cells[hash].hashedWord != key)
	{
		key = hornersMethod(converted,strlen(word));
		hash = (hash1(key) + k*hash2(key))%(TABLE_SIZE);
		k++;
	}
	
	//Kelime bulunursa
	if(k < TABLE_SIZE && ht->cells[hash].hashedWord == key)
	{
		printf("\t%s\n\tWord has been found!..\n",ht->cells[hash].word);
		return hash;
	}
	
	return 0;	//Kelime bulunamazsa
}

//Levenshtein Edit Distance algoritmasi ile kullanicilara hatali kelime icin onerilebilir kelimeler sunan fonksiyon
char* recommendWords(char* word, HashTable* dict)
{
	int i;
	int cursor = 0;			//Onerilebilir kelime listesi index'i
	int dist;				//Kelimelerin mesafelerini (edit distance) tutan degisken
	int lim = 2;			//Distance limiti 2 olarak belirlenmistir
	char new_word[20];		//Kullanicinin onerilecek kelimeler arasindan sectigi kelimeyi tutacak char dizisi
	
	//Onerilen kelimeler listesi initialize edilir
	char** word_list = (char**)calloc(100,sizeof(char*));
	for(i = 0; i < 100; i++)
		word_list[i] = (char*)calloc(20,sizeof(char));
	
	//Hash tablosundaki her kelime gezilerek hatali kelimeyle mesafeleri hesaplanir	
	for(i = 0; i < TABLE_SIZE; i++)
	{
		//printf("i = %d\n",i);
		if(strlen(dict->cells[i].word) != 0)	//Hash tablosunun bos hucresi degilse
		{
			dist = LED(word,dict->cells[i].word);	//Levenshtein Edit Distance hesaplanir
			//Eger limitten az mesafe bulunduysa ve minimum mesafeye esit ya da kucukse oneri listesine eklenir
			if(dist <= lim)			
			{
				strcpy(word_list[cursor],dict->cells[i].word);
				cursor++;
			}
		}
	
	}
	
	printf("\n\t%s\n",word);
	if(cursor == 0)	//Listeye uygun hicbir kelime eklenemezse NULL donulur
	{
		printf("\tThere are no words can be recommended!..Please try again\n");
		return NULL;
	}
	//Onerilebilir kelimeler kullaniciya sunulur
	printf("\tDid you mean : ");
	for(i = 0; i < cursor-1; i++){
		printf(" \"%s\" or",word_list[i]);
	}
	printf(" \"%s\"\n\t>>",word_list[i]);
	scanf("%s",new_word);	//Kullanicidan tercih ettigi, onerilen kelime alinir
	
	return new_word;	
}


//Hatalý kelimelerin dogrularinin yer aldigi hash tablosuna kelime yerlestirme fonksiyonu
HashTable* insertCrookedTable(char* right_word, char* crooked_word, HashTable* cw)
{
	char* converted;		//Upper case'e donusecek hatali kelime
	char* right_converted;	//Upper case'e donusecek dogru kelime
	int k = 0;				//Collision olma durumunda yeni adresi bulurken düzenli olarak arttýracaðýmýz deðiþken
	long int key;			//Horner's Metodu ile hesaplayacaðýmýz anahtar deðerini tutan deðiþken
	int hash;				//Hashing sonucu hesaplanacak adres degeri
	
	//Dogru ve hatali kelime upper case hale getirilir
	right_converted = convertUpperCase(right_word);
	converted = convertUpperCase(crooked_word);
	
	//Kelimeden anahtar çýkarýlýr
	key = hornersMethod(converted,strlen(converted));
	//Double hashing ile yeni adres hesaplanýr
	hash = (hash1(key) + k*hash2(key))%(TABLE_SIZE);
	
	//Hatali kelimeden hesaplanan adres, collision olmayana dek yeniden hesaplanýr
	while(k < TABLE_SIZE && cw->cells[hash].hashedWord != NULL && cw->cells[hash].hashedWord != hash)
	{
		key = hornersMethod(converted,strlen(converted));
		hash = (hash1(key) + k*hash2(key))%(TABLE_SIZE);
		k++;
	}
	//Eger tabloda bos yer bulunmussa
	if(k < TABLE_SIZE)
	{
		cw->cells[hash].hashedWord = key; //Hatali kelimenin adresine hatali kelime icin uretilen anahtar deger yerlestirilir
		strncpy(cw->cells[hash].word,right_converted,sizeof(cw->cells[hash].word));	//Kelimenin dogrusu adrese eklenir		
	}
	return cw;
}

//Sozluk dosyasýný okuma fonksiyonu
char *readDocument(char* fileName, char* text)
{
	FILE* fp;
	if(fp = fopen(fileName,"r")) //Dokuman mevcutsa
	{
		fread(text,sizeof(char)*(500*20),1,fp); //Dokumandaki tüm kelimeler okunur
		fclose(fp);
		
		return text;
	}
	
	return NULL; //Dokuman mevcut deðilse NULL dönülür
}

//Hash tablosu olusturma fonksiyonu
HashTable* createTable()
{
	HashTable* ht = (HashTable*)calloc(1,sizeof(HashTable));
	ht->cells = (CELL*)calloc(TABLE_SIZE, sizeof(CELL));
	return ht;
}

//HashTable struct'ina metin yerlestiren fonksiyon
HashTable* insertTable(char* text, HashTable* ht) 
{
	
	int i = 0;
	int m = 0;	//Okunan kelimenin index'ini tutan degisken
	
	int k; 				//Collision olma durumunda yeni adresi bulurken düzenli olarak arttýracaðýmýz deðiþken
	long int key; 		//Horner's Metodu ile hesaplayacaðýmýz anahtar deðerini tutan deðiþken
	int hash;			//Hashing sonucu hesaplanacak adres degeri
	char word[20];		//Metinde okunan her kelimeyi gecici olarak tutan char dizisi
	char* converted;	//Upper case'e donusecek hatali kelime
	for(i = 0; text[i] != '\0'; i++)
	{
		if(text[i] == ' ')
		{
			word[m] = '\0';
			converted = convertUpperCase(word);

			//Horner's Yöntemi ile kelimenin anahtarý oluþturulur
			key = hornersMethod(converted,strlen(converted));
			//Double hashing ile yeni adres hesaplanýr
			hash = (hash1(key) + k*hash2(key))%(TABLE_SIZE);
			
			k = 0;
			//Tüm tablo dolaþýlmadýðý, bulunan adres boþ olmadýðý ve bulduðumuz adresteki kelime ile aradýðýmýz kelime eþleþmediði sürece
			//k deðiþkenini arttýrmaya ve yeni adres deðerini hesaplamaya devam et
			while(k < TABLE_SIZE && ht->cells[hash].hashedWord != NULL && ht->cells[hash].hashedWord != hash)
			{
				key = hornersMethod(converted,strlen(converted));
				hash = (hash1(key) + k*hash2(key))%(TABLE_SIZE);
				k++;
			}
			//Boþ yer ya da ayný kelime bulunmuþsa
			if(k < TABLE_SIZE)
			{
				//Boþ hücreye yerleþtirilecek kelimenin anahtar deðeri, ve kelime yerleþtirilir
				ht->cells[hash].hashedWord = key;
				strncpy(ht->cells[hash].word,converted,sizeof(ht->cells[hash].word));
			}
			
			m = 0;
		}
		else
			word[m++] = text[i];
	}
	
	return ht;
}




