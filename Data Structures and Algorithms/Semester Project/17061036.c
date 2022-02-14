#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
typedef struct Node{
	char name[100];		//Film veya aktörün ismi
	int ID;				//Film veya aktörün ID'si (Bu ID her film ve aktör için özeldir)
	int type;			//Film veya aktör olup olmadýðýnýn bilgisi (Filmse 0 Aktörse 1'e eþittir)
	struct Node* next;	//Node'un gösterdiði kendinden bir sonraki node
}NODE;

typedef struct queue{
	int* items;			//Kuyruktaki elemanlarýn tutulduðu dizi
	int front,rear;		//Kuyruðun ilk ve en sonundaki elemaný gösteren deðiþkenler
}QUEUE;


typedef struct Graph{
	
	int* visited;				//Breadth-First Searh yapýlýrken grafta gezilen node'larýn iþaretlenmesini saðlayan dizi
	
	NODE** adjList;				//Grafta her film ve aktörün linkli listesini tutan matris
	int numOfVertices;			//Grafýn düðüm sayýsý
	unsigned long *hashtable;	//Aktör veya filmin daha önce grafa eklenip eklenmediðini daha kýsa sürede ve kolayca bulmak için
								//Hashtable kullanýldý. Her aktörün ve filmin ID'si hashtable içerisinde string için üretilen özel bir
								//sayýyla saklanýyor.	
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
	fp = openFile();	//Dosya açýlýr
	
	char key;								//Kullanýcýnýn aktör aramak isteyip istemediðinin bilgisini tutan deðiþken
	
	int rowCount = 0;						//Textteki satýr sayýsýnýn tutulduðu deðiþken
	char** text = readFile(fp,&rowCount);	//Açýlan dosyadaki bilgiler okunur
	
	
	Graph* graph = cleanData(text,rowCount);//Graf oluþturulur
	int kevinID = search("Kevin Bacon",graph->hashtable,graph->numOfVertices);	//Kevin Bacon'un ID'si bulunur
	int* prev = BFS(kevinID,graph);												//BFS uygulanýr
	int* kevinNumbers = buildPathGeneral(kevinID,prev,graph);					//Her K.B. Sayýsýnda toplam kaç aktör olduðu bulunur
	int i;
	int j;
	

	for(i = 1; graph->adjList[i] != NULL;i++)
	{
		if(kevinNumbers[i] > 0)	//Kevin Bacon Sayýsýnda aktör yoksa yazdýrýlmaz
			printf("Total number of actors whose Kevin Bacon Number is %d -> %d\tActors\n",i-1,kevinNumbers[i]);
	}
	
	printf("Total number of actors whose Kevin Bacon Number is Infinite -> %d\tActors\nOther Kevin Bacon numbers does not exist.\n\n",kevinNumbers[0]);
	
	int** distances;						//Bir kez Kevin Bacon'a olan yolu bulunan bir aktörün tekrar aranmasýna ihtiyaç kalmamasý için
											//distances isimli bir tabloya bilgileri yazýlýr ve tekrar arandýðýnda bu tablodan bilgi çekilir
	distances = malloc(sizeof(int*)*i);		//Düðüm sayýsý kadar satýr ayrýlýr
	
	for(j = 0; j < i; j++)
		distances[i] = NULL;				//Henüz hiçbir yol kayýt edilmediðinden tüm satýrlar NULL yapýlýr

		
	int length;		//Ýki ID arasý yol uzunluðunun tutulduðu deðiþken
	char* name;		//Aktör isminin tutulduðu char dizisi
	int* path;		//Kevin Bacon'a giden yolun tutulduðu dizi

	
	printf("\nWould you like to search another Actor's Kevin Bacon number? [y/n] : ");
	scanf("%c",&key);
	while(key == 'y')
	{

		name = getName();
	
		int actorID = search(name,graph->hashtable,graph->numOfVertices);	//Aktörün isminden ID'si bulunur
		
		
		if(actorID == -1)	//Aktör mevcut deðilse
			printf("\nThis actor does not exist in the list.\n");
		else{
			//Ýlk olarak yollarýn tutulduðu distances matrisine bakýlýr, aktörün ID'sinin bulunduðu hücre boþsa
			//Henüz yolu hesaplanmamýþ demektir, buildPathActor fonksiyonu ile hesaplanýr ve tabloya yazýlýr
			//Boþ deðilse fonksiyon çaðrýlmadan tablodan bilgisi alýnýr
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
				for(i = 0; path[i] != -1; i++); //Yoldaki toplam düðüm sayýsýný bulunur (i) ve length deðiþkenine atanýr
				length = i;
				printf("%s's Kevin Bacon number is %d\n",findByID(path[0],graph)->name,length/2);	//Kevin bacon sayýsýný bulmak için yol uzunluðu 2'ye bölünür
																									//(2 Aktör 1 Film örüntüsü ile ilerlediði için)
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

//Dosya açma fonksiyonu
FILE* openFile()
{
	FILE* fp = fopen("movies/input-2.txt","r");
	if(fp == NULL)	//Dosya açýlamazsa
	{
		printf("File Error!..");
		exit(0);
	}
	
}

//Dosya okuma fonksiyonu
char** readFile(FILE* fp,int* row)
{
	char** text;							//Dosyanýn tamamýný yerleþtireceðimiz text matrisi
											//Böylece tek seferde text'i alýp iþleme sokabileceðiz
															
	text = malloc(sizeof(char*)*15000);
	char line[5000];
	
	while(!feof(fp))
	{
		fgets(line,sizeof(line),fp);		//Okunan her satýr line deðiþkenine aktarýlýr
		if(line[strlen(line)-1] == '\n')	
			line[strlen(line)-1] = '\0';
		text[*row] = malloc(sizeof(char)*(strlen(line)+1));	//textte gereksiz sütun sayýsýnýn oluþmamasý için her satýrda
															//okunan line uzunluðu kadar hücre oluþturulur 
		strcpy(text[*row],line);	//line oluþturulan bölgeye kopyalanýr ve yerleþtirilir
	
		(*row)++;
	}
	
	fclose(fp);
	return text;
}


//Text'in düzenlenerek grafýn oluþturulduðu fonksiyon
Graph* cleanData(char** text,int rows)
{
	printf("\nProgram is started. Construction of graph and calculation of Kevin Bacon Numbers may take a while. Please wait...\n");
	
	int i;		//Satýr indislerini tutan deðiþken

	Graph* graph = initGraph(15000);//Graf hazýrda 15,000 düðümle baþlatýlýr
	
	
	char* movie;		//Film ismi
	char* name;			//Düzenlenmemiþ aktör ismi
	char *input;		//strtok fonksiyonu ile ayýracaðýmýz kelimelerin bellek adresinin tutulduðu deðiþken
	char* actorName;	//Düzenlenmiþ aktör ismi

	
	for(i = 0; i < rows; i++)
	{
		
		input = strtok(text[i],"/");	//strtok fonksiyonu ile '/' içeren stringteki kelimeler ayrýlýr
		movie = strdup(input);			//alýnan ilk string film adý olduðundan string duplicate edilerek movie char dizisine yerleþtirilir
	
		while((input = strtok(NULL,"/")) != NULL)	//Tüm satýr bitene dek aktör isimleri alýnýp grafa eklenmeye devam eder
		{
		
			//Aktör isimlerine de filme uygulanan iþlemler uygulanýr
			name = strdup(input);
			actorName = editName(name);
			graph = addEdge(graph,movie,actorName);	//Film-Aktör düðümleri edge olarak eklenmek üzere addEdge fonksiyonu çaðrýlýr
			
		}
	

		printf("Loading (Building Graph) : %d\r",(i*100/rows));	//Büyük verilerde sürecin görülebilmesi için yazýldý

	}

	printf("\nGraph is constructed!..Please wait for the calculation of total number of actors at each Kevin Bacon Number\n");
	
	return graph;

}

//Grafýn initialize edilmesini saðlayan fonksiyon
Graph* initGraph(int numVertices)
{
	
	Graph* graph = (Graph*)malloc(sizeof(Graph));
	graph->numOfVertices = numVertices;
	graph->visited = malloc(sizeof(int)*numVertices);
	graph->adjList = malloc(sizeof(NODE*)*numVertices);
	graph->hashtable = malloc(sizeof(unsigned long)*numVertices);
	
	//Graftaki diziler sýfýrlanýr, henüz edge eklenmediði için adjList'teki her satýr NULL yapýlýr
	int i;
	for(i = 0; i < numVertices; i++)
	{
		graph->adjList[i] = NULL;
		graph->hashtable[i] = 0;
		graph->visited[i] = 0;
	}
	
	return graph;
} 

//Dosyadaki isimlerin düzenlenmesini saðlayan fonksiyon (Pitt, Brad -> Brad Pitt)
char* editName(char* name)
{
	
	int length = strlen(name)+1; // +1 -> '\0' için ismin uzunluðuna eklendi
	int i;
	
	char* lastname = malloc(sizeof(char)*length);
	char* firstname = malloc(sizeof(char)*length);
	char* actorName = malloc(sizeof(char)*length);
	
	for(i = 0; i < length && name[i] != ',';i++)	//Text dosyasýnda ilk aktörün soyadý yazdýðýndan, virgül ile karþýlaþýncaya kadar 
		lastname[i] = name[i];						//veya isim bitene dek (Sadece ismi yazan aktörler olduðundan) lastname deðiþkenine isim atanýr
	
	lastname[i] = '\0';
	
	i+= 2;	//virgül ve boþluk atlanýr
	
	int j= 0;
	while(i < length)
		firstname[j++] = name[i++];	//Aktör ismi, hala harf kaldýysa oluþturulur
	
	//j sýfýr olarak initialize edildiðinden, eðer ilk isime bir þey atanamadýysa deðeri sýfýr olarak kalacaktýr
	if(j)	//isim ve soyisim bilgilerine sahipsek
	{
		strcpy(actorName,firstname);	//Aktörün tam adýnýn yazacaðý actorName deðiþkenine ilk ismi kopyalanýr
		strcat(actorName," ");			//Boþluk eklenir
		strcat(actorName,lastname);		//Son olarak soyismi eklenir
	}
	else	//sadece ismi veya soyismi varsa
		strcpy(actorName,lastname); 	//actorName'e kopyalanýr
	

	return actorName;
}

//Düðümlerin baðlandýðý, kenar (edge) oluþturma fonksiyonu
Graph* addEdge(Graph* graph,char src[],char dst[])
{
	//static deðiþkeni newID, gelen her farklý aktör/filme ID atanmasýný saðlar
	//Her çaðrýldýðýnda kaldýðý yerden ID atamasý için static yapýlmýþtýr
	static int newID = 0;

	//Eðer toplam düðüm sayýsý (newID ayný zamandason düðümün ID'sini tutar) grafýn alabileceði 
	//düðüm sayýsýndan fazlaysa yeni yer açýlmasý için newAllocation fonksiyonu çaðrýlýr
	if(graph->numOfVertices <= newID)
		graph = newAllocation(graph);

	
	//Yeni gelen aktör/filmin grafta daha önce olup olmadýðýný bulabilmek için search fonksiyonu çaðrýlýr
	int ID = search(src,graph->hashtable,newID);
	
	//Film için node oluþturulur
	NODE* node = createNode(src,0);
	
	
	if(ID == -1)	//Grafta bu film yoksa
	{
		graph->hashtable[newID] = hash(src);	//hashtable'ýn ID (yeni node'a ait ID) indisli hücresine hash fonksiyonu ile üretilen sayý yerleþtirilir
		node->ID = newID++;	
	}
	else
		node->ID = ID;	//Grafa film daha önce eklenmiþse ID'si alýnýr ve oluþturulan node'un ID'sine atanýr
	
	
	//Ayný iþlemler aktör için de uygulanýr
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
	//    ID    ->	Ýþaret ettiði node
	//-----------  --------------------
	// Aktör ID -> Film
	// Film  ID -> Aktör olacak þekilde graf düzenlenir
	
	node->next = graph->adjList[node2->ID];
	graph->adjList[node2->ID] = node;
	
	
	node2->next = graph->adjList[node->ID];
	graph->adjList[node->ID] = node2;

	
	return graph;
}

//Eðer graf için ayrýlan node sayýsý yetersiz kalýrsa grafý geniþletmek için newAllocation fonksiyonundan yararlanýlýr
Graph* newAllocation(Graph* graph)
{
	//10,000 düðümlük yer açýlýr
	int newsize = (graph->numOfVertices)+10000;
	//adjList e eklenecek yeni linkli listeler için realloc fonksiyonu ile yeni yer eklenir
	graph->adjList = realloc(graph->adjList,newsize*sizeof(NODE*));
	//Ziyaret edilecek node sayýsý da arttýðýndan visited dizisine yeni yer eklenir
	graph->visited = realloc(graph->visited,newsize*sizeof(int));
	
	//Hashtable dizisi de farklý stringler geleceði için geniþletilir
	graph->hashtable = realloc(graph->hashtable,newsize*sizeof(unsigned long));
	
	int i;
	//Grafa eklenen yeni yerler sýfýrlanýr
	for(i = graph->numOfVertices; i < newsize;i++)
	{
		graph->adjList[i] = NULL;
		graph->hashtable[i] = 0;
		graph->visited[i] = 0;
	}
	
	//Grafýn yeni boyutu düðüm sayýsý deðiþkenine atanýr
	graph->numOfVertices = newsize;
	return graph;	
}

//Gelen film veya aktörün grafta olup olmadýðýný bulan fonksiyon
int search(char name[],unsigned long *hashtable, int length)
{

	int i;
	unsigned long key = hash(name); //Her string özel bir sayýya karþýlýk geldiði için aranan isme karþýlýk düþen sayý bulunur
	
	//Hashtable dizisinde key ile ayný deðere sahip bir deðer bulunursa bulunduðu indis yani node'un ID'si dönülür
	for(i = 0; i < length;i++)
	{
		if(key == hashtable[i])
			return i;	
		
	}
	
	//Bulunamazsa -1 dönülür
	return -1;
}

//Node oluþturma fonksiyonu
NODE* createNode(char name[],int type)
{	
	NODE* node = malloc(sizeof(NODE));
	
	//Node'un ismi, tipi (film/aktör) atanýr ve göstereceði bir sonraki node NULL yapýlýr
	strcpy(node->name,name);			
	node->type = type;
	node->next = NULL;

	
	return node;
}

//Hash fonksiyonu djb2
unsigned long hash(char* name)
{
	//Her string için özel bir sayý üretilir. Böylece string arandýðýnda ID'sine çok daha efektif ve hýzlý bir þekilde ulaþýlabilir
	unsigned long hash = 5381;
    int c;

    while (c = *name++){
    	hash = ((hash << 5) + hash) + c; /*hash * 33 + c*/ 
	}
        
    return hash;
}

//Breadth First Search algoritmasýnýn uygulandýðý fonksiyon
int* BFS(int start,Graph* graph)
{

	int* prev; //Gezilen her node'a hangi node'dan ulaþýldýðýný gösteren prev dizisi
	int v;						//Kuyruktan çýkarýlan elemanlarýn tutulduðu deðiþken
	int startVertex = start;	//BFS'nin aramaya baþlayacaðý düðüm (Kevin Bacon)	
	
	
	QUEUE* q = createQueue(graph->numOfVertices); 		//Kuyruk oluþturulur

	prev = malloc(sizeof(int)*(graph->numOfVertices));	
	
	int i;
	for(i = 0; i < graph->numOfVertices; i++)			//Henüz graf gezilmediði için tüm elemanlar -1 yapýlýr
		prev[i] = -1;
	
	
	enqueue(q,startVertex);				//Baþlangýç ID'si kuyruða eklenir ve visited dizisinde gezilmiþ olarak iþaretlenir
	graph->visited[startVertex] = 1;
	
	while(!isEmpty(q))					//Kuyruk boþ olmadýðý sürece
	{

		v = dequeue(q);					//Kuyrukta bekleyen sýradaki eleman çýkarýlýr
		NODE* tmp = graph->adjList[v];
			
		while(tmp != NULL)				
		{
			
			if(graph->visited[tmp->ID] == 0){	//Node daha önce gezilmemiþse kuyruða eklenir ve gezilmiþ olarak iþaretlenir
	
				enqueue(q,tmp->ID);
				graph->visited[tmp->ID] = 1;
				prev[tmp->ID] = v;
			}
			tmp = tmp->next;	//Komþu node'lar gezilir
		}
			
	}
	
	free(q);		//BFS bittiðinde kuyruk (QUEUE) bellek alanýndan temizlenir

	return prev;	//Tüm yollara ulaþmamýzý saðlayan prev dizisi return edilir
}



//Kuyruk oluþturma fonksiyonu
QUEUE* createQueue(int vertices)
{
	QUEUE* q = malloc(sizeof(QUEUE));
	
	q->items = malloc(sizeof(int)*vertices);	//Düðüm sayýsý kadar kuyruk dizisinde yer ayýrýlýr
	q->front = -1;								//Henüz kuyruk boþ olduðundan, bunu belli etmek amacýyla 
	q->rear = -1;								//ilk ve son elemaný gösteren deðiþkenler (front,rear) -1 yapýlýr

	return q;
}

//Kuyruða yeni film/aktör ekleme fonksiyonu. Kuyrukta film/aktörün ID'si tutulur
void enqueue(QUEUE* q,int val)
{
	//Kuyruk boþsa ilk elemaný gösteren deðiþken 0 yapýlýr
	if(isEmpty(q))
		q->front = 0;
	//Sýradaki son elemaný gösteren deðiþken bir arttýrýlýr
	q->rear++;
	q->items[q->rear] = val;//Son elemanýn olduðu göze yeni gelen ID yerleþtirilir
	
}


//Kuyruktan eleman çýkarma fonksiyonu
int dequeue(QUEUE* q)
{
	//Kuyruk boþsa eleman çýkarýlamaz, bu yüzden -1 dönülür
	if(isEmpty(q))
		return -1;
		
	return q->items[q->front++];	//Kuyruk boþ deðilse sýrada bekleyen ilk eleman dönülür, front kuyrukta gelen
									//sýradaki elemaný göstermesi için ayarlanýr
}


//Queue'nun boþ olup olmadýðýný öðrenmek için oluþturulan fonksiyon
int isEmpty(QUEUE* q)
{
	if(q->front == -1 || (q->front > q->rear))	//Kuyruk hiç oluþturulmamýþsa sýradaki elemaný gösteren deðiþken (front) -1'dir
		return 1;								//Kuyruk oluþturulmuþ, kuyruktaki tüm elemanlar gezilmiþse front kuyruktaki son elemaný gösteren
												//deðiþkenden (rear) büyük olur. Bu iki durum da kuyruðun boþ olduðunu gösterir
												
	return 0;	//Kuyruk boþ deðilse
}

//Her Kevin Bacon Sayýsýna sahip kaç aktörün olduðunu bulan fonksiyon
int* buildPathGeneral(int start, int* prev,Graph* graph)
{
	int index;
	int i;
	int j;
	NODE* tmp;
	
	//Kevin Bacon Sayýlarýndaki aktörleri saymak için baconCounter dizisi oluþturulur ve sýfýrlanýr
	int* baconCounter = malloc(sizeof(int)*(graph->numOfVertices));
	for(i = 0; i < graph->numOfVertices; i++)
		baconCounter[i] = 0;


	for(j = 0; graph->adjList[j] != NULL; j++)
	{
		tmp = findByID(j,graph);	//Aktör sayýlarýný hesaplayacaðýmýz için elimizdeki node'un film/aktör bilgisine ulaþmalýyýz
									//Bu yüzden elimizdeki ID'den ait olduðu node'u bulur ve type'ýný kontrol ederiz
		
		if(tmp->type)	//Aktörse (type = 1 aktör, type = 0 film)
		{
			//Aktörden Kevin Bacon'a ulaþmak istiyoruz, bu yüzden baþlangýcý aktörün ID'si yaparýz
			//prev dizisi o ID'ye hangi ID'den ulaþtýðýmýzý bulmamýzda yardýmcý olur. Kevin Bacon'un ID'sine veya baðlantýnýn bittiði (-1)
			//bir noktaya ulaþýncaya dek geriye gidilir -> index = prev[index]
			for(index = j; index != -1 && start != index; index = prev[index])
				i++;
			
			//Kevin Bacon'ýn ID'sine ulaþýlmýþsa sayacýn, bulunan kevin bacon numaralý hücresi arttýrýlýr
			//Sayacýn 0. gözü sonsuz Kevin Bacon Sayýsý'na sahip aktörler için ayrýlmýþtýr
			if(index == start){
				baconCounter[1+(i/2)]++;	//i yolun uzunluðunu gösterir ancak bu uzunluðun içerisinde filmlerin ID'leri de katýlmýþtýr. 
											//Ýki aktörü baðlayan bir film olduðundan toplam uzunluðun 2'ye bölünmesiyle K.B. Sayýsý elde edilebilir
			}
			else
				baconCounter[0]++;			//Kevin Bacon'a giden bir yol bulunamamýþsa sonsuz sayýlarýn toplandýðý 0. göz arttýrýlýr
				
			
		}
		
		
		i=0;
		
	}
	printf("The total number of actors at each Kevin Bacon Number is calculated!..\n\n");
	
	return baconCounter;
}

//Verilen ID'den ID'nin node'unu dönen fonksiyon
NODE* findByID(int ID,Graph* graph)
{
	
	NODE* tmp;
	
	//ID'nin gösterdiði ilk node'un ID'sini kullanarak adjList teki gözüne gidebilir ve aradýðýmýz node'a ulaþabiliriz
	//Detaylý Anlatým : 
	//Baðlantýlar çift yönlü (bipartite) olduðu için Aktör->Film ise Film->Aktör olacaktýr
	//Bu durumda Aktör'ün node'unu bulmak istiyorsak gösterdiði filmin linkli listesine gidebilir ve 
	//orada aradýðýmýz ID'nin node'unu bulabiliriz
	tmp = graph->adjList[graph->adjList[ID]->ID];
	while(tmp != NULL && tmp->ID != ID)
		tmp = tmp->next;
	
	//Linkli liste bitmemiþse aradýðýmýz node'u bulmuþuz demektir
	if(tmp != NULL)
		return tmp;
	
	//Bitmiþse node bulunamamýþ demektir, NULL dönülür
	return NULL;
}

//Spesifik olarak bir aktörün K.B. Sayýsý hesaplanmak istendiðinde buildPathActor fonksiyonu çalýþtýrýlýr
int* buildPathActor(int start, int end, int* prev, Graph* graph)
{
	int index;
	int i;
	//Yol bilgilerini (yol üzerindeki ID'leri) de tutmamýz gerektiðinden path isimli bir int dizi oluþturulur
	int* path = malloc(sizeof(int)*(graph->numOfVertices));
	
	//Henüz yol oluþturulmadýðýndan dizideki her eleman -1 yapýlýr
	for(i = 0; i < graph->numOfVertices; i++)
		path[i] = -1;

	i = 0;

	//buildPathGeneral'da bahsi geçen mantýkla Kevin Bacon bulunana veya yol bitene kadar geriye gidilir
	for(index = end; index != -1 && start != index; index = prev[index])
		path[i++] = index;
	
	//Kevin Bacon'a ulaþýldýysa
	if(index == start)
	{
		path[i++] = index;
		return path;
	}
	free(path);	//Ulaþýlamadýysa path'in kapladýðý bellek alaný free edilir ve NULL dönülür

	return NULL;	
}

//Kullanýcýdan aktör ismi okuyan fonksiyon
char* getName()
{
	char* name = malloc(sizeof(char)*100);
	
	printf("\nActor Name : \n");
	getchar();			//Önceki iþlemden gelen '\n' karakterini yakalamak için getchar() ektra çaðrýlýr
	
	char c = getchar();
	int i = 0;
	while(c != '\n')	//ENTER'a basýlana dek isim alýnýr
	{
		name[i++] = c;
		c = getchar();
	}
	name[i] = '\0';
	
	return name;
}

//Graf yazdýrma fonksiyonu. Bu fonksiyon inceleyen kiþinin grafý görebilmesini saðlamak amacýyla ekstra olarak eklenmiþtir
//Ýstenirse koda eklenerek fonksiyon çalýþtýrýlabilir
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



































