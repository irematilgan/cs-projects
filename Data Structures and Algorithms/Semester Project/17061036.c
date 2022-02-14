#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
typedef struct Node{
	char name[100];		//Film veya akt�r�n ismi
	int ID;				//Film veya akt�r�n ID'si (Bu ID her film ve akt�r i�in �zeldir)
	int type;			//Film veya akt�r olup olmad���n�n bilgisi (Filmse 0 Akt�rse 1'e e�ittir)
	struct Node* next;	//Node'un g�sterdi�i kendinden bir sonraki node
}NODE;

typedef struct queue{
	int* items;			//Kuyruktaki elemanlar�n tutuldu�u dizi
	int front,rear;		//Kuyru�un ilk ve en sonundaki eleman� g�steren de�i�kenler
}QUEUE;


typedef struct Graph{
	
	int* visited;				//Breadth-First Searh yap�l�rken grafta gezilen node'lar�n i�aretlenmesini sa�layan dizi
	
	NODE** adjList;				//Grafta her film ve akt�r�n linkli listesini tutan matris
	int numOfVertices;			//Graf�n d���m say�s�
	unsigned long *hashtable;	//Akt�r veya filmin daha �nce grafa eklenip eklenmedi�ini daha k�sa s�rede ve kolayca bulmak i�in
								//Hashtable kullan�ld�. Her akt�r�n ve filmin ID'si hashtable i�erisinde string i�in �retilen �zel bir
								//say�yla saklan�yor.	
}Graph;


QUEUE* createQueue(int);
int isEmpty(QUEUE*);
void enqueue(QUEUE*,int);
int dequeue(QUEUE*);
NODE* createNode(char[],int);
Graph* initGraph(int);
int* BFS(int,Graph*);
void printGraph(Graph*);
unsigned long hash(char*);
int search(char[],unsigned long*,int);
Graph* newAllocation(Graph*);
Graph* addEdge(Graph*,char[],char[]);
NODE* findByID(int,Graph*);
int* buildPathGeneral(int,int*,Graph*);
int* buildPathActor(int,int,int*,Graph*);
FILE* openFile();
char* editName(char*);
Graph* cleanData(char**,int);
char** readFile(FILE*,int*);
char* getName();


int main()
{
	FILE* fp;
	fp = openFile();	//Dosya a��l�r
	
	char key;								//Kullan�c�n�n akt�r aramak isteyip istemedi�inin bilgisini tutan de�i�ken
	
	int rowCount = 0;						//Textteki sat�r say�s�n�n tutuldu�u de�i�ken
	char** text = readFile(fp,&rowCount);	//A��lan dosyadaki bilgiler okunur
	
	
	Graph* graph = cleanData(text,rowCount);//Graf olu�turulur
	int kevinID = search("Kevin Bacon",graph->hashtable,graph->numOfVertices);	//Kevin Bacon'un ID'si bulunur
	int* prev = BFS(kevinID,graph);												//BFS uygulan�r
	int* kevinNumbers = buildPathGeneral(kevinID,prev,graph);					//Her K.B. Say�s�nda toplam ka� akt�r oldu�u bulunur
	int i;
	int j;
	

	for(i = 1; graph->adjList[i] != NULL;i++)
	{
		if(kevinNumbers[i] > 0)	//Kevin Bacon Say�s�nda akt�r yoksa yazd�r�lmaz
			printf("Total number of actors whose Kevin Bacon Number is %d -> %d\tActors\n",i-1,kevinNumbers[i]);
	}
	
	printf("Total number of actors whose Kevin Bacon Number is Infinite -> %d\tActors\nOther Kevin Bacon numbers does not exist.\n\n",kevinNumbers[0]);
	
	int** distances;						//Bir kez Kevin Bacon'a olan yolu bulunan bir akt�r�n tekrar aranmas�na ihtiya� kalmamas� i�in
											//distances isimli bir tabloya bilgileri yaz�l�r ve tekrar arand���nda bu tablodan bilgi �ekilir
	distances = malloc(sizeof(int*)*i);		//D���m say�s� kadar sat�r ayr�l�r
	
	for(j = 0; j < i; j++)
		distances[i] = NULL;				//Hen�z hi�bir yol kay�t edilmedi�inden t�m sat�rlar NULL yap�l�r

		
	int length;		//�ki ID aras� yol uzunlu�unun tutuldu�u de�i�ken
	char* name;		//Akt�r isminin tutuldu�u char dizisi
	int* path;		//Kevin Bacon'a giden yolun tutuldu�u dizi

	
	printf("\nWould you like to search another Actor's Kevin Bacon number? [y/n] : ");
	scanf("%c",&key);
	while(key == 'y')
	{

		name = getName();
	
		int actorID = search(name,graph->hashtable,graph->numOfVertices);	//Akt�r�n isminden ID'si bulunur
		
		
		if(actorID == -1)	//Akt�r mevcut de�ilse
			printf("\nThis actor does not exist in the list.\n");
		else{
			//�lk olarak yollar�n tutuldu�u distances matrisine bak�l�r, akt�r�n ID'sinin bulundu�u h�cre bo�sa
			//Hen�z yolu hesaplanmam�� demektir, buildPathActor fonksiyonu ile hesaplan�r ve tabloya yaz�l�r
			//Bo� de�ilse fonksiyon �a�r�lmadan tablodan bilgisi al�n�r
			if(distances[actorID] == NULL)	
			{
				path = buildPathActor(kevinID,actorID,prev,graph);
				distances[actorID] = path;
			}
			else
				path = distances[actorID];		
			
			//Bir yol mevcutsa
			if(path)
			{
				for(i = 0; path[i] != -1; i++); //Yoldaki toplam d���m say�s�n� bulunur (i) ve length de�i�kenine atan�r
				length = i;
				printf("%s's Kevin Bacon number is %d\n",findByID(path[0],graph)->name,length/2);	//Kevin bacon say�s�n� bulmak i�in yol uzunlu�u 2'ye b�l�n�r
																									//(2 Akt�r 1 Film �r�nt�s� ile ilerledi�i i�in)
				for(i = 0; i < length-2; i+=2)
					printf("%s - %s  : %s\n",findByID(path[i],graph)->name, findByID(path[i+2],graph)->name,findByID(path[i+1],graph)->name);	
				
				printf("\n");
			}
			else
			{
				printf("%s's Kevin Bacon number is infinite\n",findByID(actorID,graph)->name);
			}
			
			
		}
		printf("Would you like to search another Actor's Kevin Bacon number? [y/n] : ");
		scanf("%c",&key);
	}
	

	return 0;
}

//Dosya a�ma fonksiyonu
FILE* openFile()
{
	FILE* fp = fopen("movies/input-2.txt","r");
	if(fp == NULL)	//Dosya a��lamazsa
	{
		printf("File Error!..");
		exit(0);
	}
	
}

//Dosya okuma fonksiyonu
char** readFile(FILE* fp,int* row)
{
	char** text;							//Dosyan�n tamam�n� yerle�tirece�imiz text matrisi
											//B�ylece tek seferde text'i al�p i�leme sokabilece�iz
															
	text = malloc(sizeof(char*)*15000);
	char line[5000];
	
	while(!feof(fp))
	{
		fgets(line,sizeof(line),fp);		//Okunan her sat�r line de�i�kenine aktar�l�r
		if(line[strlen(line)-1] == '\n')	
			line[strlen(line)-1] = '\0';
		text[*row] = malloc(sizeof(char)*(strlen(line)+1));	//textte gereksiz s�tun say�s�n�n olu�mamas� i�in her sat�rda
															//okunan line uzunlu�u kadar h�cre olu�turulur 
		strcpy(text[*row],line);	//line olu�turulan b�lgeye kopyalan�r ve yerle�tirilir
	
		(*row)++;
	}
	
	fclose(fp);
	return text;
}


//Text'in d�zenlenerek graf�n olu�turuldu�u fonksiyon
Graph* cleanData(char** text,int rows)
{
	printf("\nProgram is started. Construction of graph and calculation of Kevin Bacon Numbers may take a while. Please wait...\n");
	
	int i;		//Sat�r indislerini tutan de�i�ken

	Graph* graph = initGraph(15000);//Graf haz�rda 15,000 d���mle ba�lat�l�r
	
	
	char* movie;		//Film ismi
	char* name;			//D�zenlenmemi� akt�r ismi
	char *input;		//strtok fonksiyonu ile ay�raca��m�z kelimelerin bellek adresinin tutuldu�u de�i�ken
	char* actorName;	//D�zenlenmi� akt�r ismi

	
	for(i = 0; i < rows; i++)
	{
		
		input = strtok(text[i],"/");	//strtok fonksiyonu ile '/' i�eren stringteki kelimeler ayr�l�r
		movie = strdup(input);			//al�nan ilk string film ad� oldu�undan string duplicate edilerek movie char dizisine yerle�tirilir
	
		while((input = strtok(NULL,"/")) != NULL)	//T�m sat�r bitene dek akt�r isimleri al�n�p grafa eklenmeye devam eder
		{
		
			//Akt�r isimlerine de filme uygulanan i�lemler uygulan�r
			name = strdup(input);
			actorName = editName(name);
			graph = addEdge(graph,movie,actorName);	//Film-Akt�r d���mleri edge olarak eklenmek �zere addEdge fonksiyonu �a�r�l�r
			
		}
	

		printf("Loading (Building Graph) : %d\r",(i*100/rows));	//B�y�k verilerde s�recin g�r�lebilmesi i�in yaz�ld�

	}

	printf("\nGraph is constructed!..Please wait for the calculation of total number of actors at each Kevin Bacon Number\n");
	
	return graph;

}

//Graf�n initialize edilmesini sa�layan fonksiyon
Graph* initGraph(int numVertices)
{
	
	Graph* graph = (Graph*)malloc(sizeof(Graph));
	graph->numOfVertices = numVertices;
	graph->visited = malloc(sizeof(int)*numVertices);
	graph->adjList = malloc(sizeof(NODE*)*numVertices);
	graph->hashtable = malloc(sizeof(unsigned long)*numVertices);
	
	//Graftaki diziler s�f�rlan�r, hen�z edge eklenmedi�i i�in adjList'teki her sat�r NULL yap�l�r
	int i;
	for(i = 0; i < numVertices; i++)
	{
		graph->adjList[i] = NULL;
		graph->hashtable[i] = 0;
		graph->visited[i] = 0;
	}
	
	return graph;
} 

//Dosyadaki isimlerin d�zenlenmesini sa�layan fonksiyon (Pitt, Brad -> Brad Pitt)
char* editName(char* name)
{
	
	int length = strlen(name)+1; // +1 -> '\0' i�in ismin uzunlu�una eklendi
	int i;
	
	char* lastname = malloc(sizeof(char)*length);
	char* firstname = malloc(sizeof(char)*length);
	char* actorName = malloc(sizeof(char)*length);
	
	for(i = 0; i < length && name[i] != ',';i++)	//Text dosyas�nda ilk akt�r�n soyad� yazd���ndan, virg�l ile kar��la��ncaya kadar 
		lastname[i] = name[i];						//veya isim bitene dek (Sadece ismi yazan akt�rler oldu�undan) lastname de�i�kenine isim atan�r
	
	lastname[i] = '\0';
	
	i+= 2;	//virg�l ve bo�luk atlan�r
	
	int j= 0;
	while(i < length)
		firstname[j++] = name[i++];	//Akt�r ismi, hala harf kald�ysa olu�turulur
	
	//j s�f�r olarak initialize edildi�inden, e�er ilk isime bir �ey atanamad�ysa de�eri s�f�r olarak kalacakt�r
	if(j)	//isim ve soyisim bilgilerine sahipsek
	{
		strcpy(actorName,firstname);	//Akt�r�n tam ad�n�n yazaca�� actorName de�i�kenine ilk ismi kopyalan�r
		strcat(actorName," ");			//Bo�luk eklenir
		strcat(actorName,lastname);		//Son olarak soyismi eklenir
	}
	else	//sadece ismi veya soyismi varsa
		strcpy(actorName,lastname); 	//actorName'e kopyalan�r
	

	return actorName;
}

//D���mlerin ba�land���, kenar (edge) olu�turma fonksiyonu
Graph* addEdge(Graph* graph,char src[],char dst[])
{
	//static de�i�keni newID, gelen her farkl� akt�r/filme ID atanmas�n� sa�lar
	//Her �a�r�ld���nda kald��� yerden ID atamas� i�in static yap�lm��t�r
	static int newID = 0;

	//E�er toplam d���m say�s� (newID ayn� zamandason d���m�n ID'sini tutar) graf�n alabilece�i 
	//d���m say�s�ndan fazlaysa yeni yer a��lmas� i�in newAllocation fonksiyonu �a�r�l�r
	if(graph->numOfVertices <= newID)
		graph = newAllocation(graph);

	
	//Yeni gelen akt�r/filmin grafta daha �nce olup olmad���n� bulabilmek i�in search fonksiyonu �a�r�l�r
	int ID = search(src,graph->hashtable,newID);
	
	//Film i�in node olu�turulur
	NODE* node = createNode(src,0);
	
	
	if(ID == -1)	//Grafta bu film yoksa
	{
		graph->hashtable[newID] = hash(src);	//hashtable'�n ID (yeni node'a ait ID) indisli h�cresine hash fonksiyonu ile �retilen say� yerle�tirilir
		node->ID = newID++;	
	}
	else
		node->ID = ID;	//Grafa film daha �nce eklenmi�se ID'si al�n�r ve olu�turulan node'un ID'sine atan�r
	
	
	//Ayn� i�lemler akt�r i�in de uygulan�r
	NODE* node2 = createNode(dst,1);
	ID = search(dst,graph->hashtable,newID);
	
	if(ID == -1)
	{
		graph->hashtable[newID] = hash(dst);
		node2->ID = newID++;
	}
	else
		node2->ID = ID;
	
	
	//  adjList
	// ----------
	//    ID    ->	��aret etti�i node
	//-----------  --------------------
	// Akt�r ID -> Film
	// Film  ID -> Akt�r olacak �ekilde graf d�zenlenir
	
	node->next = graph->adjList[node2->ID];
	graph->adjList[node2->ID] = node;
	
	
	node2->next = graph->adjList[node->ID];
	graph->adjList[node->ID] = node2;

	
	return graph;
}

//E�er graf i�in ayr�lan node say�s� yetersiz kal�rsa graf� geni�letmek i�in newAllocation fonksiyonundan yararlan�l�r
Graph* newAllocation(Graph* graph)
{
	//10,000 d���ml�k yer a��l�r
	int newsize = (graph->numOfVertices)+10000;
	//adjList e eklenecek yeni linkli listeler i�in realloc fonksiyonu ile yeni yer eklenir
	graph->adjList = realloc(graph->adjList,newsize*sizeof(NODE*));
	//Ziyaret edilecek node say�s� da artt���ndan visited dizisine yeni yer eklenir
	graph->visited = realloc(graph->visited,newsize*sizeof(int));
	
	//Hashtable dizisi de farkl� stringler gelece�i i�in geni�letilir
	graph->hashtable = realloc(graph->hashtable,newsize*sizeof(unsigned long));
	
	int i;
	//Grafa eklenen yeni yerler s�f�rlan�r
	for(i = graph->numOfVertices; i < newsize;i++)
	{
		graph->adjList[i] = NULL;
		graph->hashtable[i] = 0;
		graph->visited[i] = 0;
	}
	
	//Graf�n yeni boyutu d���m say�s� de�i�kenine atan�r
	graph->numOfVertices = newsize;
	return graph;	
}

//Gelen film veya akt�r�n grafta olup olmad���n� bulan fonksiyon
int search(char name[],unsigned long *hashtable, int length)
{

	int i;
	unsigned long key = hash(name); //Her string �zel bir say�ya kar��l�k geldi�i i�in aranan isme kar��l�k d��en say� bulunur
	
	//Hashtable dizisinde key ile ayn� de�ere sahip bir de�er bulunursa bulundu�u indis yani node'un ID'si d�n�l�r
	for(i = 0; i < length;i++)
	{
		if(key == hashtable[i])
			return i;	
		
	}
	
	//Bulunamazsa -1 d�n�l�r
	return -1;
}

//Node olu�turma fonksiyonu
NODE* createNode(char name[],int type)
{	
	NODE* node = malloc(sizeof(NODE));
	
	//Node'un ismi, tipi (film/akt�r) atan�r ve g�sterece�i bir sonraki node NULL yap�l�r
	strcpy(node->name,name);			
	node->type = type;
	node->next = NULL;

	
	return node;
}

//Hash fonksiyonu djb2
unsigned long hash(char* name)
{
	//Her string i�in �zel bir say� �retilir. B�ylece string arand���nda ID'sine �ok daha efektif ve h�zl� bir �ekilde ula��labilir
	unsigned long hash = 5381;
    int c;

    while (c = *name++){
    	hash = ((hash << 5) + hash) + c; /*hash * 33 + c*/ 
	}
        
    return hash;
}

//Breadth First Search algoritmas�n�n uyguland��� fonksiyon
int* BFS(int start,Graph* graph)
{

	int* prev; //Gezilen her node'a hangi node'dan ula��ld���n� g�steren prev dizisi
	int v;						//Kuyruktan ��kar�lan elemanlar�n tutuldu�u de�i�ken
	int startVertex = start;	//BFS'nin aramaya ba�layaca�� d���m (Kevin Bacon)	
	
	
	QUEUE* q = createQueue(graph->numOfVertices); 		//Kuyruk olu�turulur

	prev = malloc(sizeof(int)*(graph->numOfVertices));	
	
	int i;
	for(i = 0; i < graph->numOfVertices; i++)			//Hen�z graf gezilmedi�i i�in t�m elemanlar -1 yap�l�r
		prev[i] = -1;
	
	
	enqueue(q,startVertex);				//Ba�lang�� ID'si kuyru�a eklenir ve visited dizisinde gezilmi� olarak i�aretlenir
	graph->visited[startVertex] = 1;
	
	while(!isEmpty(q))					//Kuyruk bo� olmad��� s�rece
	{

		v = dequeue(q);					//Kuyrukta bekleyen s�radaki eleman ��kar�l�r
		NODE* tmp = graph->adjList[v];
			
		while(tmp != NULL)				
		{
			
			if(graph->visited[tmp->ID] == 0){	//Node daha �nce gezilmemi�se kuyru�a eklenir ve gezilmi� olarak i�aretlenir
	
				enqueue(q,tmp->ID);
				graph->visited[tmp->ID] = 1;
				prev[tmp->ID] = v;
			}
			tmp = tmp->next;	//Kom�u node'lar gezilir
		}
			
	}
	
	free(q);		//BFS bitti�inde kuyruk (QUEUE) bellek alan�ndan temizlenir

	return prev;	//T�m yollara ula�mam�z� sa�layan prev dizisi return edilir
}



//Kuyruk olu�turma fonksiyonu
QUEUE* createQueue(int vertices)
{
	QUEUE* q = malloc(sizeof(QUEUE));
	
	q->items = malloc(sizeof(int)*vertices);	//D���m say�s� kadar kuyruk dizisinde yer ay�r�l�r
	q->front = -1;								//Hen�z kuyruk bo� oldu�undan, bunu belli etmek amac�yla 
	q->rear = -1;								//ilk ve son eleman� g�steren de�i�kenler (front,rear) -1 yap�l�r

	return q;
}

//Kuyru�a yeni film/akt�r ekleme fonksiyonu. Kuyrukta film/akt�r�n ID'si tutulur
void enqueue(QUEUE* q,int val)
{
	//Kuyruk bo�sa ilk eleman� g�steren de�i�ken 0 yap�l�r
	if(isEmpty(q))
		q->front = 0;
	//S�radaki son eleman� g�steren de�i�ken bir artt�r�l�r
	q->rear++;
	q->items[q->rear] = val;//Son eleman�n oldu�u g�ze yeni gelen ID yerle�tirilir
	
}


//Kuyruktan eleman ��karma fonksiyonu
int dequeue(QUEUE* q)
{
	//Kuyruk bo�sa eleman ��kar�lamaz, bu y�zden -1 d�n�l�r
	if(isEmpty(q))
		return -1;
		
	return q->items[q->front++];	//Kuyruk bo� de�ilse s�rada bekleyen ilk eleman d�n�l�r, front kuyrukta gelen
									//s�radaki eleman� g�stermesi i�in ayarlan�r
}


//Queue'nun bo� olup olmad���n� ��renmek i�in olu�turulan fonksiyon
int isEmpty(QUEUE* q)
{
	if(q->front == -1 || (q->front > q->rear))	//Kuyruk hi� olu�turulmam��sa s�radaki eleman� g�steren de�i�ken (front) -1'dir
		return 1;								//Kuyruk olu�turulmu�, kuyruktaki t�m elemanlar gezilmi�se front kuyruktaki son eleman� g�steren
												//de�i�kenden (rear) b�y�k olur. Bu iki durum da kuyru�un bo� oldu�unu g�sterir
												
	return 0;	//Kuyruk bo� de�ilse
}

//Her Kevin Bacon Say�s�na sahip ka� akt�r�n oldu�unu bulan fonksiyon
int* buildPathGeneral(int start, int* prev,Graph* graph)
{
	int index;
	int i;
	int j;
	NODE* tmp;
	
	//Kevin Bacon Say�lar�ndaki akt�rleri saymak i�in baconCounter dizisi olu�turulur ve s�f�rlan�r
	int* baconCounter = malloc(sizeof(int)*(graph->numOfVertices));
	for(i = 0; i < graph->numOfVertices; i++)
		baconCounter[i] = 0;


	for(j = 0; graph->adjList[j] != NULL; j++)
	{
		tmp = findByID(j,graph);	//Akt�r say�lar�n� hesaplayaca��m�z i�in elimizdeki node'un film/akt�r bilgisine ula�mal�y�z
									//Bu y�zden elimizdeki ID'den ait oldu�u node'u bulur ve type'�n� kontrol ederiz
		
		if(tmp->type)	//Akt�rse (type = 1 akt�r, type = 0 film)
		{
			//Akt�rden Kevin Bacon'a ula�mak istiyoruz, bu y�zden ba�lang�c� akt�r�n ID'si yapar�z
			//prev dizisi o ID'ye hangi ID'den ula�t���m�z� bulmam�zda yard�mc� olur. Kevin Bacon'un ID'sine veya ba�lant�n�n bitti�i (-1)
			//bir noktaya ula��ncaya dek geriye gidilir -> index = prev[index]
			for(index = j; index != -1 && start != index; index = prev[index])
				i++;
			
			//Kevin Bacon'�n ID'sine ula��lm��sa sayac�n, bulunan kevin bacon numaral� h�cresi artt�r�l�r
			//Sayac�n 0. g�z� sonsuz Kevin Bacon Say�s�'na sahip akt�rler i�in ayr�lm��t�r
			if(index == start){
				baconCounter[1+(i/2)]++;	//i yolun uzunlu�unu g�sterir ancak bu uzunlu�un i�erisinde filmlerin ID'leri de kat�lm��t�r. 
											//�ki akt�r� ba�layan bir film oldu�undan toplam uzunlu�un 2'ye b�l�nmesiyle K.B. Say�s� elde edilebilir
			}
			else
				baconCounter[0]++;			//Kevin Bacon'a giden bir yol bulunamam��sa sonsuz say�lar�n topland��� 0. g�z artt�r�l�r
				
			
		}
		
		
		i=0;
		
	}
	printf("The total number of actors at each Kevin Bacon Number is calculated!..\n\n");
	
	return baconCounter;
}

//Verilen ID'den ID'nin node'unu d�nen fonksiyon
NODE* findByID(int ID,Graph* graph)
{
	
	NODE* tmp;
	
	//ID'nin g�sterdi�i ilk node'un ID'sini kullanarak adjList teki g�z�ne gidebilir ve arad���m�z node'a ula�abiliriz
	//Detayl� Anlat�m : 
	//Ba�lant�lar �ift y�nl� (bipartite) oldu�u i�in Akt�r->Film ise Film->Akt�r olacakt�r
	//Bu durumda Akt�r'�n node'unu bulmak istiyorsak g�sterdi�i filmin linkli listesine gidebilir ve 
	//orada arad���m�z ID'nin node'unu bulabiliriz
	tmp = graph->adjList[graph->adjList[ID]->ID];
	while(tmp != NULL && tmp->ID != ID)
		tmp = tmp->next;
	
	//Linkli liste bitmemi�se arad���m�z node'u bulmu�uz demektir
	if(tmp != NULL)
		return tmp;
	
	//Bitmi�se node bulunamam�� demektir, NULL d�n�l�r
	return NULL;
}

//Spesifik olarak bir akt�r�n K.B. Say�s� hesaplanmak istendi�inde buildPathActor fonksiyonu �al��t�r�l�r
int* buildPathActor(int start, int end, int* prev, Graph* graph)
{
	int index;
	int i;
	//Yol bilgilerini (yol �zerindeki ID'leri) de tutmam�z gerekti�inden path isimli bir int dizi olu�turulur
	int* path = malloc(sizeof(int)*(graph->numOfVertices));
	
	//Hen�z yol olu�turulmad���ndan dizideki her eleman -1 yap�l�r
	for(i = 0; i < graph->numOfVertices; i++)
		path[i] = -1;

	i = 0;

	//buildPathGeneral'da bahsi ge�en mant�kla Kevin Bacon bulunana veya yol bitene kadar geriye gidilir
	for(index = end; index != -1 && start != index; index = prev[index])
		path[i++] = index;
	
	//Kevin Bacon'a ula��ld�ysa
	if(index == start)
	{
		path[i++] = index;
		return path;
	}
	free(path);	//Ula��lamad�ysa path'in kaplad��� bellek alan� free edilir ve NULL d�n�l�r

	return NULL;	
}

//Kullan�c�dan akt�r ismi okuyan fonksiyon
char* getName()
{
	char* name = malloc(sizeof(char)*100);
	
	printf("\nActor Name : \n");
	getchar();			//�nceki i�lemden gelen '\n' karakterini yakalamak i�in getchar() ektra �a�r�l�r
	
	char c = getchar();
	int i = 0;
	while(c != '\n')	//ENTER'a bas�lana dek isim al�n�r
	{
		name[i++] = c;
		c = getchar();
	}
	name[i] = '\0';
	
	return name;
}

//Graf yazd�rma fonksiyonu. Bu fonksiyon inceleyen ki�inin graf� g�rebilmesini sa�lamak amac�yla ekstra olarak eklenmi�tir
//�stenirse koda eklenerek fonksiyon �al��t�r�labilir
void printGraph(Graph* graph)
{
	int i;
	printf("\n");
	NODE* temp;
	for(i = 0; graph->adjList[i] != NULL; i++)
	{
		printf("%d - > ",i);
		temp = graph->adjList[i];
		while(temp != NULL)
		{
			printf("%s/",temp->name);
			temp = temp->next;
		}
		printf("\n");
	}
}



































