#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>


//CSV dosyasi ile ilgili bilgilerin tutuldugu struct
typedef struct recTable{
	char** users;	//kullanici isimlerinin tutuldugu dizi
	char** books;	//kitap isimlerinin tutuldugu dizi
	int** votes;	//kullanicilarin kitaplar icin verdikleri oylarin tutuldugu matris
	int row;  	//kullanici sayisi
	int col; 	//kitap sayisi
}recTable;

//benzer kullanicilarin veya tahminlemesi yapilan oylarin tutuldugu struct
//en yuksekten dusuge siralama yapilmasi icin linked-list yapisi kullanilmistir
typedef struct node{
	float val;		//hucre degeri (benzerlik/tahmini oy)
	char* name;		//benzeyen kullanici adi/oylanan kitap ismi
	struct node* next;
}node;


//Oneri Tablosunu yazdirma fonksiyonu
void printTable(recTable* rt)
{
	int i;
	int j;
	
	printf("\t");
	for(j = 0; j < rt->col; j++)
	{
		printf("%s\t",rt->books[j]);
	}
	
	printf("\n");
	
	for(i = 0; i < rt->row; i++)
	{
		printf("%s\t\t",rt->users[i]);
		for(j = 0; j < rt->col; j++)
		{
			printf("%d\t\t",rt->votes[i][j]);
		}
		printf("\n");
	}
}

//Kullanicinin isminden bulundugu indisi bulan fonksiyon
int searchUser(char* userno,recTable* rt)
{
	int i;
	//kullanici adi bulunana veya tum kullanici dizisi gezilene kadar
	for(i = 0; strcmp(rt->users[i],userno) && (i < rt->row); i++); 
	//kullanici bulunamazsa -1 donulur
	if(i >= rt->row)
		return -1;
	//bulunursa indisi donulur
	return i;

}

//Kullanicinin bir kitap icin verdigi oylarin ortalamasinni hesaplayan fonksiyon
float calculateMean(recTable* rt, int ind)
{
	int i;
	float total = 0;
	float m;
	for(i = 0; i < rt->col; i++)
	{
		total += rt->votes[ind][i]; //Oylar toplanir
	}
	m = total/(float)i;	//Ortalamasi alinir

	return m;
}

//Iki kullanicinin benzerligini hesaplayan fonksiyon
float calculateSimilarity(recTable* rt, char* uno1, char* uno2)
{
	int i;
	int j;
	int ind1, ind2;
	//Oylarin toplaminin tutuldugu degiskenler
	float total1 = 0;	
	float total2 = 0;
	int vote1,vote2;	//Iki kullanicinin verdigi oy
	float mean1,mean2;	//Iki kullanicinin kitaplar icin verdigi oylarin ortalamasi
	float sq1,sq2;
	float similarity; 	//Pearson korelasyon formulu sonucu
	
	//Ilk iki kullanicinin csv matrisindeki konumlarý bulunur
	ind1 = searchUser(uno1,rt);	
	ind2 = searchUser(uno2,rt);
	
	for(j = 0; j < rt->col; j++)
	{
		vote1 = rt->votes[ind1][j];
		vote2 = rt->votes[ind2][j];
		total1 += vote1;
		total2 += vote2;
	}
	mean1 = (float)total1/(rt->col);
	mean2 = (float)total2/(rt->col);
	
	total1 = 0;
	sq1 = 0;
	sq2 = 0;
	for(j = 0; j < rt->col; j++)
	{
		vote1 = rt->votes[ind1][j];
		vote2 = rt->votes[ind2][j];
		if(vote1 && vote2) //iki kisi de kitaba oy verdiyse Person formulu icin gerekli islemler uygulanir
		{
			total1 += (vote1-mean1)*(vote2-mean2);
			sq1 += pow((vote1-mean1),2);
			sq2 += pow((vote2-mean2),2);
		}
	}
	similarity = (total1)/(sqrt(sq1)*sqrt(sq2));	//benzerlik orani hesaplanir
	return similarity;
}

//node struct'ý initialize etme fonksiyonu
node* createNode(float val, char* userno)
{
	node* newNode = (node*)malloc(sizeof(node));
	newNode->name = (char*)malloc(sizeof(char)*20);
	strcpy(newNode->name,userno);	//verilen kullanici adi eklenir
	newNode->val = val;
	newNode->next = NULL;
	return newNode;
}

//Kullaniciya kitap onerme fonksiyonu
char* recommendBook(recTable* rt, char* userno, node* head, int k)
{
	int i;
	int j;
	//Onerilecek kitaplarin en yuksek oydan en az oya siralandigi linkli liste
	node* root_books = NULL;
	
	//Oneri yapilacak kullanicinin kullanici dizisinde yeri bulunur
	int userind = searchUser(userno,rt);
		
	//Kullanicinin kitaplar icin verdigi oylarin ortalamasi bulunur
	float usermean = calculateMean(rt,userind);
	
	//Kitap linkli listesi icinde gezebilmek icin olusturulan node* degiskeni
	node* iterator;
	
	//Benzerligi bulunan kullanicilarin verdigi oylarin ortalamasini tutan degisken
	float mean2;
	
	//Benzerligi bulunan kullanicilarin indislerini tutan degisken
	int ind2;
	//Odevde verili oy tahminleme formulunun pay ve payda bolumleri
	float pay = 0;
	float payda = 0;
	
	float pred; //Tahmin sonucunun tutuldugu degisken
	
	node* tmp; //Benzer kullanici linkli listesi icinde gezebilmek icin olusturulan node* degiskeni
	
	for(i = 0; i < rt->col; i++)
	{
		//Kullanicinin oy vermedigi kitap bulunursa
		if(rt->votes[userind][i] == 0)
		{
			tmp = head;
			pay = 0;
			payda = 0;
			j = 0;
			//Benzer kullanicilarin verdigi oylar kullanilir
			while(tmp != NULL && j < k)
			{
				ind2 = searchUser(tmp->name,rt);
				mean2 = calculateMean(rt,ind2);
				pay = pay + (tmp->val)*(float)(rt->votes[ind2][i]-mean2);
				payda += (tmp->val);
				tmp = tmp->next;
				j++;
			}
			//Tahmini oy hesaplanir
			pred = usermean + (pay/payda);
			//Eger linkli listeye hic kitap yerlestirilmediyse
			if(root_books == NULL)
			{
				//root iniialize edilir ve tahmini oy degeri yerlestirilir
				root_books = (node*)malloc(sizeof(node));
				root_books->name = (char*)malloc(sizeof(char)*20);
				strcpy(root_books->name,rt->books[i]);
				root_books->val = pred;
				root_books->next = NULL;
			}
			else //Liste daha once olusturulmussa
			{
				//Tahmini deger, en yuksek oy degerinden buyukse basa yerlestirilir
				if(pred > root_books->val)
				{
					node* newRec = createNode(pred,rt->books[i]);
					newRec->next = root_books;
					root_books = newRec;
				}
				else //Degilse linkli liste gezilerek buyuk oldugu noktaya yerlestirilir
				{
					iterator = root_books;
					while(iterator->next != NULL && iterator->next->val > pred)
						iterator = iterator->next;
					node* newRec = createNode(pred,rt->books[i]);
					newRec->next = iterator->next;
					iterator->next = newRec;
				}
			}
			
			
		}
	 
	}
	printf("%s kullanicisinda okunmamis olan kitaplar icin hesaplanan tahmini begenme degerleri:\n",userno);
	iterator = root_books;
	i = 1;
	while(iterator != NULL)
	{
		printf("%d. %s, %f",i,iterator->name,iterator->val);
		iterator = iterator->next;
		printf("\n");
		i++;
	}
	printf("Sonuc olarak onerilen kitap : %s\n\n",root_books->name);
	return root_books;
}


//Verilen kullaniciya benzer K kullanicinin onerildigi fonksiyon
node* recommendKUsers(recTable* rt, int k, char* userno)
{
	int rows = rt->row;
	int i = 0;
	float sim; //Benzerlik miktarinin tutuldugu fonksiyon
	node* head = (node*)malloc(sizeof(node));
	node* newNode;
	node* iterator = head;	
	
	//Verilen kullanici disinda bir kullanici bulunana dek kullanici dizisi gezilir
	while(!strcmp(rt->users[i],userno))	i++;
	//Benzer kullanicilarin tutuldugu linkli listenin head'i olusturulur
	if(strcmp(rt->users[i],userno))
	{
		//Verilen kullanici ile listedeki kullanici arasindaki benzerlik hesaplanir
		sim = calculateSimilarity(rt,userno,rt->users[i]);
		head->val = sim;
		head->name = (char*)malloc(sizeof(char)*10);
		strcpy(head->name,rt->users[i]);
		i = 0;	
	}
	
	//'N' ile baslayan kullanici isimlerine kadar olan kullanicilar alinir
	while(rt->users[i][0] != 'N' && i < rows)
	{
		//Varolan kullanici, listedeki kullanici degilse
		if(strcmp(rt->users[i],userno))
		{
			//Benzerlik hesaplanir
			sim = calculateSimilarity(rt,userno,rt->users[i]);
			//Benzerlik degeri en buyuk benzerlik degerinden kucukse
			if(sim < head->val)
			{
				//Kendinden kucuk benzerlik degerine sahip bir node bulunana dek linkli liste gezilir
				iterator = head;
				while(iterator->next != NULL && iterator->next->val > sim)	
					iterator = iterator->next;
					
				newNode = createNode(sim,rt->users[i]);
				newNode->next = iterator->next;
				iterator->next = newNode;

			}
			else //Benzerlik degeri, en buyuk benzerlik degerinden buyukse
			{
				//linkli listenin basina yerlestirilir
				newNode = createNode(sim,rt->users[i]);
				newNode->next = head;
				head = newNode;
			}
				
		}
		i++;
	}
	
	printf("%s kullanicisina en yakin kullanicilar(k=%d) ve hesaplanan pearson benzerlikleri sirasiyla,\n",userno,k);
	iterator = head;
	i = 0;
	while(iterator != NULL && i < k)
	{
		printf("%d. %s (%f)\n",i+1,iterator->name,iterator->val);
		iterator = iterator->next;
		i++;
	}
	printf("\n");
	return head;
}


int main()
{
	char line[1024];
    char* tmp;
    char* word = (char*)malloc(sizeof(char)*20);
    char* token;
    char* username = (char*)malloc(sizeof(char)*10);
    int num_books = 0;
    int num_users = 0;
    int i;
    int k;
    
    
    FILE* stream = fopen("recdataset.csv", "r"); //csv dosyasi okunur
    recTable* table = malloc(sizeof(recTable));	//csv iceriginin tutuldugu tablo olusturulur
    table->books = (char**)calloc(15,sizeof(char*)); //tablodaki kitaplar icin yer ayrilir
	
	//Kitap isimleri icin yer ayrilir
    for(i = 0; i < 15; i++)
    {
    	table->books[i] = (char*)calloc(20, sizeof(char));
	}
	//Kullanici isimlerinin tutuldugu string dizisi olusturulur ve yer ayrilir
	table->users = (char**)calloc(50,sizeof(char*));
	for(i = 0; i < 50; i++)
    {
    	table->users[i] = (char*)calloc(10, sizeof(char));
	}
	
	//KITAP BASLIKLARI ALINIR
	i = 0;
	fgets(line, 1024, stream);
    tmp = strdup(line);
	word = strchr(tmp,'\n');
	if(word)
		*word = 0;
	token = strtok(tmp,";");
	token = strtok(NULL,";");
	while(token)
	{
		strcpy(table->books[i++],token);
		num_books += 1;
		token = strtok(NULL,";");
	}
	table->books[i] = '\0';
	table->col = num_books;
	printf("\n");
    
    //KULLANICI ADI VE KULLANICI OYLARI ALINIR
    int counter;
    int val;
    char* num = malloc(sizeof(char)*5);
    int ind_letter = 0;
    
    table->votes = (int**)calloc(50,sizeof(int*));
    for(i = 0; i < 50; i++)
    {
    	table->votes[i] = (int*)calloc(num_books,sizeof(int));
	}
    
    while (fgets(line, 1024, stream))
    {
    	i = 0;
    	num_users++;
    	counter = 0;
    	while(line[i] != ';')
    	{
    		word[ind_letter++] = line[i++];
		}
		word[ind_letter] = '\0';
		strcpy(table->users[(num_users-1)], word);
		ind_letter = 0;
		i++;
    	while(line[i] != '\n')
    	{
    		if(line[i] == ';' || line[i] == " ")
    		{
    			num[ind_letter] = '\0';
    			
    			if(ind_letter == 0)
    			{
    				val = 0;	
    				ind_letter = 0;
				}
    			else
    			{
    				val = atoi(num);
    				ind_letter = 0;
				}
		
    			table->votes[(num_users-1)][counter++] = val;
			}
			else
    			num[ind_letter++] = line[i];
    		
    		i++;
		}

    	num[ind_letter] = '\0';
	    if(ind_letter == 0)
		{
			val = 0;	
			ind_letter = 0;
		}
		else
		{
			val = atoi(num);
			ind_letter = 0;
		}
		table->votes[(num_users-1)][counter] = val;
    }
    
    //Tablodaki kullanici ve kitap sayisi tablo struct'ýnda tutulur
    table->col = counter+1;
    table->row = num_users;
    
    printf("Kitap onerisi yapilacak kullanici adi : ");
    scanf("%s",username);
    printf("Benzer kullanici sayisi(k): ");
    scanf("%d",&k);
    
    
    node* list = recommendKUsers(table,k,username);
    recommendBook(table,username,list,k);

    return 0;
}
