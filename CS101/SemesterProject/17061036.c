#include <stdio.h>
#include <stdlib.h>
#include <time.h>

//kullanacagim bazi tuslari global'de tanımlıyorum
#define esc 27
#define up 72
#define down 80
#define left 75
#define right 77
#define enter '\r'
//ASCII tablosunda yukari 72, sol 75, sag 77, asagi 80, escape 27'dir. '\r' enter tusunu temsil ediyor


//SCOREBOARD'DA KULLANILACAK ISIM VE SKOR LISTESINI GOSTEREN FONKSIYONU YAZDIM
void print_liste(char (*isim_listesi)[50], int *lastscore,int count)
{
	int i,j;
	system("CLS");//ekrani temizlemek icin kullandim
	i = 0; j = 0;
	
	printf("\n\n\t\t================================================================\n");
	printf("\t\t=========================SCOREBOARD=============================\n");
	printf("\t\t================================================================\n");
	while(i < count)//kaydettigimiz skorlari yazdirmak icin
	{		
		printf("\t\t");
		printf("%s",isim_listesi[i]);//isim_listesi matrisinin her satirini, yani isimleri yazdiriyoruz
		printf("\t%d",lastscore[i]);//her oynayanin ismiyle beraber ulastigi skoru da yazdiriyoruz
		printf("\n");
		printf("\t\t----------------------------------------------------------------\n");
		
		i++;
	}
	
	
}

void print_tablo(int (*tablo)[4], int score, int maxscore, int a, int b, int joker)
{
	/*her islem bittiginde tabloyu yazdiracagiz, kod kalabaligi olmasin diye
	tabloyu yazdiran bir fonksiyon yazdim
	*/
	int m,n;
	
	
	printf("\n");
	printf("\t\t  _______________________________________________________________  \n");
		for(m = 0; m < 4; m++)
		{
			printf("\t\t");
			
			printf(" |\t \t |\t \t |\t \t |\t \t |\t\n");
			printf("\t\t |      ");
			for(n = 0; n < 4; n++)
			{	
				if((m == a && n == b) && tablo[m][n] != 0)//olusturulan her yeni sayiyi basina '*' koyarak yazdirmak istiyoruz
				{
					printf("*%d\t |\t",tablo[m][n]);
				}
				else if(tablo[m][n] != 0)//tablodaki eleman 0 degilse yazdir 0 ise yazdırma(gorsellik icin yaptim)
				{
					printf("%d\t |\t",tablo[m][n]);
				}
				else{printf("         |      ");}
			}
			printf("\n");
			printf("\t\t |_______________|_______________|_______________|_______________|\t\n");
		}
		printf("\t\t");
		
		printf("\n\t\tSCORE = %d\n\n\t\tMAX SCORE = %d\n\n",score,maxscore);//skoru ve maksimum skoru yazdir
		if(joker >= 10)//jokerlerimiz 10 ve üzerinde bir sayıya ulaştıysa "JOKER" yazısının kırmızı olmasını sağlıyorum
		{
			printf("\t\t");printf("\033[0;31m");printf("JOKER = %d\n",joker);printf("\033[0m");
		}
		else
		{
			printf("\t\tJOKER = %d\n",joker);
		}
		printf("\t\t------------------------------------------------------------------\n");
		printf("\n\t\tYENIDEN BASLAT = X\n\n");
		printf("\t\tGERI DON = R\n\n\t\tOYUNU BITIR = E\n");
} 

//KURALLARI YAZDIRAN FONKSİYON
void print_rules()
{
		printf("\n\n");
		printf("\t\t|");printf("===================================================================================");printf("|\n");
		printf("\n\t\t  Oyunun Kurallari :\n\t\t  -Saga kaydirmak icin SAG OK TUSUNA..\n");
		printf("\t\t  -Sola kaydirmak icin SOL OK TUSUNA..");
		printf("\n\t\t  -Yukari kaydirmak icin YUKARI OK TUSUNA\n\t\t  -Asagi kaydirmak icin ASAGI OK TUSUNA BASMALISINIZ... ");
		printf("\n\t\t  -Oyun esnasinda ESC'ye bastiginizda menu acilir..");
		printf("\n\t\t  -Oyun icerisinde yaptiginiz her toplama islemi size 1 JOKER kazandirir");
		printf("\n\t\t    -Joker'e 'J' tusuna basarak erisebilir ve istediginiz bir hucreyi silebilirsiniz\n\n");
		printf("\t\t  (Eger oyunu yeniden baslatmak istiyorsaniz X'e basiniz)\n");
		printf("\n\n\n\t\t\tGERI DON = R\n\n\n");
		printf("\t\t|");printf("===================================================================================");printf("|\n");
}

//MENUYU YAZDIRAN FONKSİYON
void print_menu(char ch, int key)
{
	int i,j;
	

	//EN UST SECENEGE GELİRSEK, SEÇENEĞİN RENGİNİ KIRMIZIYA BOYAMAK İÇİN:	
	if(key == 3)
	{
		system("CLS");
		printf("\n\n");
		printf("\t\t____________________________________________________________________\n");
		printf("\t\t|");printf("\t\t\t\t\t\t\t\t   |\n");
		printf("\t\t|\t\t\t\t\t\t\t\t   |\n\t\t|\t\t\t\t\t\t\t\t   |\n");
		printf("\t\t|\t\t\t      -2048-\t\t\t\t   |\n\t\t|\t\t\t\t\t\t\t\t   |\n");
		printf("\t\t|\t\t\tOYUNA HOSGELDINIZ\t\t\t   |\n\t\t|\t\t\t\t\t\t\t\t   |\n");
		printf("\t\t|\t\tBu oyun BBG dersi icin hazirlanmistir\t\t   |\n\t\t|\t\t\t\t\t\t\t\t   |\n");
		printf("\t\t|\t\t\t\t\t\t\t\t   |\n\t\t|\t\t\t\t\t\t\t\t   |\n");
		printf("\t\t|\t\t\t- ");printf("\033[0;31m");printf("OYUN");printf("\033[0m");printf("\t\t\t\t\t   |\n\t\t|\t\t\t KURALLAR\t\t\t\t   |\n");
		printf("\t\t|\t\t\t SCOREBOARD\t\t\t\t   |\n\t\t|\t\t\t CIKIS\t\t\t\t\t   |\n");
		printf("\t\t|\t\t\t\t\t\t\t\t   |\n\t\t|\t\t\t\t\t\t\t\t   |\n");
		printf("\t\t|\t\t\t\t\t\t\t\t   |\n\t\t|\t\t\t\t\t\t\t\t   |\n");
		printf("\t\t|\t\t\t\t\t\t\t\t   |\n\t\t|\t\t\t\t\t\t\t\t   |\n");
		printf("\t\t|\t\t\t\t\t\t\t\t   |\n\t\t|\t\t\t\t\t\t\t\t   |\n");
		printf("\t\t|__________________________________________________________________|\n");
		
	}
	//2. SEÇENEK
	else if(key == 2)
	{
		system("CLS");
		printf("\n\n");
		printf("\t\t____________________________________________________________________\n");
		printf("\t\t|");printf("\t\t\t\t\t\t\t\t   |\n");
		printf("\t\t|\t\t\t\t\t\t\t\t   |\n\t\t|\t\t\t\t\t\t\t\t   |\n");
		printf("\t\t|\t\t\t      -2048-\t\t\t\t   |\n\t\t|\t\t\t\t\t\t\t\t   |\n");
		printf("\t\t|\t\t\tOYUNA HOSGELDINIZ\t\t\t   |\n\t\t|\t\t\t\t\t\t\t\t   |\n");
		printf("\t\t|\t\tBu oyun BBG dersi icin hazirlanmistir\t\t   |\n\t\t|\t\t\t\t\t\t\t\t   |\n");
		printf("\t\t|\t\t\t\t\t\t\t\t   |\n\t\t|\t\t\t\t\t\t\t\t   |\n");
		printf("\t\t|\t\t\t OYUN\t\t\t\t\t   |\n\t\t|\t\t\t- ");printf("\033[0;31m");printf("KURALLAR");printf("\033[0m");
		printf("\t\t\t\t   |\n");
		printf("\t\t|\t\t\t SCOREBOARD\t\t\t\t   |\n\t\t|\t\t\t CIKIS\t\t\t\t\t   |\n");
		printf("\t\t|\t\t\t\t\t\t\t\t   |\n\t\t|\t\t\t\t\t\t\t\t   |\n");
		printf("\t\t|\t\t\t\t\t\t\t\t   |\n\t\t|\t\t\t\t\t\t\t\t   |\n");
		printf("\t\t|\t\t\t\t\t\t\t\t   |\n\t\t|\t\t\t\t\t\t\t\t   |\n");
		printf("\t\t|\t\t\t\t\t\t\t\t   |\n\t\t|\t\t\t\t\t\t\t\t   |\n");
		printf("\t\t|__________________________________________________________________|\n");
	}
	//3.SEÇENEK
	else if(key == 1)
	{
		system("CLS");
		printf("\n\n");
		printf("\t\t____________________________________________________________________\n");
		printf("\t\t|");printf("\t\t\t\t\t\t\t\t   |\n");
		printf("\t\t|\t\t\t\t\t\t\t\t   |\n\t\t|\t\t\t\t\t\t\t\t   |\n");
		printf("\t\t|\t\t\t      -2048-\t\t\t\t   |\n\t\t|\t\t\t\t\t\t\t\t   |\n");
		printf("\t\t|\t\t\tOYUNA HOSGELDINIZ\t\t\t   |\n\t\t|\t\t\t\t\t\t\t\t   |\n");
		printf("\t\t|\t\tBu oyun BBG dersi icin hazirlanmistir\t\t   |\n\t\t|\t\t\t\t\t\t\t\t   |\n");
		printf("\t\t|\t\t\t\t\t\t\t\t   |\n\t\t|\t\t\t\t\t\t\t\t   |\n");
		printf("\t\t|\t\t\t OYUN\t\t\t\t\t   |\n\t\t|\t\t\t KURALLAR\t\t\t\t   |\n");
		printf("\t\t|\t\t\t- ");printf("\033[0;31m");printf("SCOREBOARD");printf("\033[0m");
		printf("\t\t\t\t   |\n\t\t|\t\t\t CIKIS\t\t\t\t\t   |\n");
		printf("\t\t|\t\t\t\t\t\t\t\t   |\n\t\t|\t\t\t\t\t\t\t\t   |\n");
		printf("\t\t|\t\t\t\t\t\t\t\t   |\n\t\t|\t\t\t\t\t\t\t\t   |\n");
		printf("\t\t|\t\t\t\t\t\t\t\t   |\n\t\t|\t\t\t\t\t\t\t\t   |\n");
		printf("\t\t|\t\t\t\t\t\t\t\t   |\n\t\t|\t\t\t\t\t\t\t\t   |\n");
		printf("\t\t|__________________________________________________________________|\n");
	}
	//EN ALT SEÇENEK
	else if(key == 0)
	{
		system("CLS");
		printf("\n\n");
		printf("\t\t____________________________________________________________________\n");
		printf("\t\t|");printf("\t\t\t\t\t\t\t\t   |\n");
		printf("\t\t|\t\t\t\t\t\t\t\t   |\n\t\t|\t\t\t\t\t\t\t\t   |\n");
		printf("\t\t|\t\t\t      -2048-\t\t\t\t   |\n\t\t|\t\t\t\t\t\t\t\t   |\n");
		printf("\t\t|\t\t\tOYUNA HOSGELDINIZ\t\t\t   |\n\t\t|\t\t\t\t\t\t\t\t   |\n");
		printf("\t\t|\t\tBu oyun BBG dersi icin hazirlanmistir\t\t   |\n\t\t|\t\t\t\t\t\t\t\t   |\n");
		printf("\t\t|\t\t\t\t\t\t\t\t   |\n\t\t|\t\t\t\t\t\t\t\t   |\n");
		printf("\t\t|\t\t\t OYUN\t\t\t\t\t   |\n\t\t|\t\t\t KURALLAR\t\t\t\t   |\n");
		printf("\t\t|\t\t\t SCOREBOARD\t\t\t\t   |\n\t\t|\t\t\t- ");printf("\033[0;31m");printf("CIKIS");printf("\033[0m");
		printf("\t\t\t\t\t   |\n");
		printf("\t\t|\t\t\t\t\t\t\t\t   |\n\t\t|\t\t\t\t\t\t\t\t   |\n");
		printf("\t\t|\t\t\t\t\t\t\t\t   |\n\t\t|\t\t\t\t\t\t\t\t   |\n");
		printf("\t\t|\t\t\t\t\t\t\t\t   |\n\t\t|\t\t\t\t\t\t\t\t   |\n");
		printf("\t\t|\t\t\t\t\t\t\t\t   |\n\t\t|\t\t\t\t\t\t\t\t   |\n");
		printf("\t\t|__________________________________________________________________|\n");
	}
}



int main()
{
	system("color 06");
	int tablo[4][4], eskitablo[4][4],lastscore[50];
	int i,j,tmp,score,maxscore,a,b,cevap,sign,count,sum,poprandom,key,oldscore,joker,jsatir,jsutun;
	char ch;//oyun içindeki hareketlerimizin algılanması içi ch değişkeni oluşturdum, yapılan hareketlerin değeri buraya gidecek
	char isim_listesi[50][50];//oyun bittiğinde isimleri almamız için oluşturduğumuz matris
	j = 0;
    tmp = 0;//sürüklemeler sırasında geçici olarak j'yi atayacağımız değişken (ileride daha ayrıntılı açıklayacağım)
	sum = 0;score = 0;maxscore = 0;
	count = 0;//oyunu bitiren oyuncuları saymak için count değişkeni oluşturdum
    sign = 0;//oyunun devam edip etmeyeceğini belirlemek için sign değişkeni
    poprandom = 0;//hangi durumlarda random sayı üretileceğini belirlemek için poprandom
    joker = 0;
	srand(time(NULL));//rastgele sayı üretebilmesi ve bu rastgele sayının sürekli değişebilmesi için
	
	for(i = 0; i < 50; i++)
	{
		lastscore[i] = 0;//skor listesinin tamamına 0 değeri veriyorum
	}
	
	
	for(i = 0; i < 4; i++)
	{
		for(j = 0; j < 4; j++)
		{
			tablo[i][j] = 0;//tabloyu sıfırlıyorum
		}
	}
	
	for(i = 0; i < 2; i++)
	{
		//rastgele satır ve sütunlar belirliyorum
		a = rand() % 4;
		b = rand() % 4;
			
		//belirlenen satır ve sütundaki sayı sıfırdan farklı olana dek a ve b yi değiştiriyorum	
		while(tablo[a][b] != 0)
		{
			a = rand() % 4;
			b = rand() % 4;
		}
		//tablodaki hücreye oyunun kurallarına göre ya 4 ya da 2 değeri atanıyor	
		tablo[a][b] = rand() % 3 + 2;//2 ile 4 arasında rastgele sayı üretmesi için
		while(tablo[a][b] % 2 != 0)//rastgele atanan değerin 3 olmaması için
		{
			tablo[a][b] = rand() % 3 + 2;
		}
	}
	
	
	//MENÜ BÖLÜMÜ
	
	key = 3;//en ust secenekten baslasin
	
	print_menu(ch,key);//menüyü yazdır
	
	ch = getch();
	if(ch == -32 || ch == 0)//ilk taramada -32(her bilgisayarda farklı olabilir), ikincide ise istediğim değeri alabildiğim için if yerleştirdim
	{
		ch = getch();
	}
	
	/*
		KEY = 3 -> OYUNA GEÇ
		KEY = 2 -> KURALLARA BAK
		KEY = 1 -> SCOREBOARD'A BAK
		KEY = 0 -> OYUNDAN ÇIK
	*/ 
	
	while(ch != enter || (key != 0 && key != 3))//ÇIKIŞ ve OYUN seçeneğine gelip ENTER'a basmadığı sürece menüde kal 
	{
		ch = getch();
		if(ch == -32)
		{
			ch = getch();
		}
	
		if(ch == up && key < 3)//yukarı ok tuşuna bastığımızda, en üst seçenekte değilsek key'i bir arttırıyorum
		{
			key = key + 1;//bu sayede diğer seçeneklere geçebiliyoruz
		}
		else if(ch == down && key > 0)//aşağı ok tuşuna bastığımızda, en aşağıdaki seçeneğe gelmediysek
		{
			key = key - 1;//key'i bir azaltıyorum
		}
		
		print_menu(ch,key);
			
		if(ch == enter)//ENTER'a basarsak
		{
			//OYUN KURALLARI seçeneğine girersek
			if(key == 2)
			{
				system("CLS");
				
				print_rules();//kuralları yazdır
				
				while(ch != 'r' && ch != 'R')//menüye geri dönmek için 'r' ye basmadığımız sürece bu seçenekte kal
				{
					ch = getch();
					if(ch == -32)
					{
						ch = getch();
					}	
				}
				
			}
			//SCOREBOARD'A BAKMAK İSTERSEK
			else if(key == 1)
			{
				system("CLS");
				print_liste(isim_listesi,lastscore,count);
				printf("\n\n\t\tMENUYE DON = R\n");
				while(ch != 'r' && ch != 'R')
				{
					ch = getch();
					if(ch == -32)
					{
						ch = getch();
					}	
				}
			}
			//OYUNDAN ÇIKMAK İSTERSEK
			else if(key == 0)
			{
				sign = 1;
			}
			//OYUNA GİRMEK İSTERSEK
			else
				system("CLS");
		
		}
		
	}
	system("CLS");
	
	//OYUN BÖLÜMÜ

	while(sign == 0)//OYUN sign 1 olana dek devam edecek
	{
		if(score > maxscore) maxscore = score;
		
		poprandom = 0;//her el random sayı üretip üretemeyeceğimizi anlamak için kullandığımız poprandom'u sıfırlıyorum
		
		
		print_tablo(tablo,score,maxscore,a,b,joker);
	
		
		ch = getch();
		if(ch == -32 ||ch == 0)//Bazı bilgisayarlarda ilk değer sıfır olabiliyor
		{
			//-32'den sonra gelen kodu cekiyoruz
			ch = getch();
		}
		if(ch == 'r')//oyuncu r'ye bastığında eski hamleye geri dönebilmek için
		{
			for(i = 0; i < 4; i++)
			{
				for(j = 0; j < 4; j++)
				{
					tablo[i][j] = eskitablo[i][j];//kaydettiğimiz eski tabloyu, tabloma atıyorum
					
				}
			}
			if(score != oldscore)//geçen el skorda bir değişim olduysa
			{
				score = oldscore;//eski skora geri dön
				joker--;//kazandığımız joker hakkını sil
			}
			
			system("CLS");
		}
		for(i = 0; i < 4; i++)
		{
			for(j = 0; j < 4; j++)
			{
				eskitablo[i][j] = tablo[i][j];//tablonun yedeğini tutuyoruz
			}
		}
		oldscore = score;//skorun yedeği
		
		//Joker kullanmak istersek (10'dan fazla jokerimiz olmalı)
		if(ch == 'j' && joker >= 10)
		{
			printf("\n\t\tHangi hucreyi silmek istersiniz, once satirini sonra sutununu giriniz ?");
			scanf("%d",&jsatir);
			scanf("%d",&jsutun);
			
			tablo[jsatir-1][jsutun-1] = 0;
			
			joker = joker - 10;
		}
		
		//oyun içerisinde menüye girmek istersek
		if(ch == esc)
		{
			//Menüde kullandığımız kodlar
			
			print_menu(ch,key);
			
			while(ch != enter || (key != 0 && key != 3))  
			{
				ch = getch();
				if(ch == -32)
				{
					ch = getch();
				}
			
				if(ch == up && key < 3)
				{
					key = key + 1;
				}
				else if(ch == down && key > 0)
				{
					key = key - 1;
				}
				print_menu(ch,key);
					
				if(ch == enter)
				{
					if(key == 2)
					{
						system("CLS");
						print_rules();
						
						while(ch != 'r' && ch != 'R')
						{
							ch = getch();
							if(ch == -32)
							{
								ch = getch();
							}	
						}
						
					}
					else if(key == 1)
					{
						system("CLS");
						print_liste(isim_listesi,lastscore,count);
						printf("\n\n\t\tMENUYE DON = R\n");
						while(ch != 'r' && ch != 'R')
						{
							ch = getch();
							if(ch == -32)
							{
								ch = getch();
							}	
						}
					}
					else if(key == 0)
					{
						sign = 1;
					}
					else
						system("CLS");
				
				}
		
			}
			
			
			
			
			
		}
		
		
		
		
		
		if(ch == left)//sola kaydirmak istiyorsak
		{
			for(i = 0; i < 4; i++)
			{ 
				sum = 0;//kaydirma sirasinda, daha önce toplama yapilip yapilmadigini tespit etmek icin "sum" degiskenini her satırda 0 yapiyoruz
					
				for(j = 1; j < 4; j++)
				{
					if(tablo[i][j] != 0)
					{
						//hücre sıfırdan farklı olursa j'de oynama yapamayacağımızdan tmp adlı değişkene j'nin değerini atıyorum
						
						tmp = j;
							
						//geldiğimiz hücrenin gerisindeki hücre 0'dan farklı olana dek geri gidiyorum	
						while((tablo[i][tmp-1] == 0) && (tmp >= 1))
						{
							tmp--;
							
						}
						if(tmp != j) poprandom = 1;/*tmp'de değişiklik olmuşsa hücremizde hareketlilik olacaktır,
													 bu da yeni sayı üretebilmemiz için yeterlidir*/
						
					    if(tmp == 0)//gerideki hücrelerin tamamı sıfır ise
						{
							tablo[i][0] = tablo[i][j];//ilk hücreye kaldığımız hücrenin değerini ata
							tablo[i][j] = 0;//kaldığımız hücrenin değerini sıfırla
							
						}
						else if(tablo[i][tmp-1] == tablo[i][j])
						{
							if(tmp == j) poprandom = 1;//sayılar birbirine eşitse ve hücremizin tam arkasındaki hücreyse yeni sayı üretilebilir
							
							//iki sayı eşit olabilir ancak kendilerinden önce başka iki sayı toplanmışsa o sayıları toplayamayız
							//bunu anlayabilmek için sum değişkeni oluşturdum
							
							if(sum == 1)//daha önce toplama yapılmışsa
							{	
								tablo[i][tmp] = tablo[i][j];//değeri eşit olunan hücreden bir sonraki hücreye yerleş
								tablo[i][j] = 0;//bulunduğumuz hücreyi sıfırla
								sum = 0;
								
							}
							else if(sum == 0)//toplama yapılmamışsa
							{
								tablo[i][tmp-1] = tablo[i][j] + tablo[i][j];
								score = score + tablo[i][tmp-1];
								tablo[i][j] = 0;
								sum = 1;//sum'ı toplama yapıldığına dair işaretliyorum
								
							}
							
						}
						//ulaştığımız hücrenin değeri bulunduğumuz hücredekinden farklıysa ve hemen gerimizdeki hücre değilse
						else if(tablo[i][tmp-1] != tablo[i][j] && (tmp != j))
						{
							tablo[i][tmp] = tablo[i][j];//ulaştığımız hücreden sonraki boş hücreye yerleş
							sum = 0;
							tablo[i][j] = 0;//kaldığımız hücreyi sıfırla
							
							
						}
							
					}
					
				}			
	
			}	
		}
		else if(ch == right)//saga kaydirdigimizda (Aynı Yöntemi Uyguladım)
		{
			for(i = 0; i < 4; i++)
			{ 
				sum = 0;
					
				for(j = 3; j >= 1; j--)
				{
						
					if(tablo[i][j-1] != 0)//bulundugumuz hucredeki eleman sifirdan farkli olana dek geri git
					{
						tmp = j;
							
						while((tmp <=3) && (tablo[i][tmp] == 0))
						{
							tmp++;
						}
						if(tmp != j) poprandom = 1;//bir gerimizde olmayan hucre sifirdan farkliysa yeni sayi uretilebilir
						//bu kosulun amaci, sayimizin hareket edip etmeyecegini anlamaktir
						
						//sayimiz en saga kadar gidebiliyorsa
					    if(tmp == 4)
						{
							tablo[i][3] = tablo[i][j-1];
							tablo[i][j-1] = 0;
							
						}					
						else//ulastigimiz sayi hucredeki sayidan farkliysa ya da hucreye esitse 
						{
							
							if(sum == 1 || (tablo[i][tmp] != tablo[i][j-1]))//iki hucreyi toplamamaliysak veya hucreler birbirine esit degilse
							{
								sum = 0;
								tablo[i][tmp-1] = tablo[i][j-1];
								if(tmp != j)//hucre hareket etmisse yani sifirdan farkli olan hemen bir sagimizdaki hucre degilse
								{tablo[i][j-1] = 0;}
							}
							else//hucreler birbirine esitse ve toplanabilirse
							{
								if(tmp == j) poprandom = 1;
								tablo[i][tmp] = tablo[i][j-1] * 2;
								score = score + tablo[i][tmp];
								tablo[i][j-1] = 0;
								sum = 1;//bundan sonra esit olan hucreler toplanamayacak
							}
							
						}
							
					}
					
				}			
	
			}	
			
		}
		else if(ch == up)//yukari kaydirdigimizda
		{
			for(j = 0; j < 4; j++)
			{
				sum = 0;
				
				for(i = 1; i < 4; i++)
				{
					if(tablo[i][j] != 0)//yukaridan asagiya dogru sifirdan farkli olan ilk hucreyi bulalim
					{
						tmp = i;
						
						//tmp en asagidaki hucreye erisinceye dek veya sifirdan farkl, bir sayiyla karsilasana dek asagi inecek
						while(tmp >= 1 && (tablo[tmp-1][j] == 0))
						{
							tmp--;
						}
						if(tmp != i) {poprandom = 1;}//sayinin hareket edip edemeyecegine gore rastgele sayi uretecegiz
						
						if(tmp == 0)
						{
							tablo[0][j] = tablo[i][j];
							tablo[i][j] = 0;
							
						}
						else
						{
							if(sum == 1 || (tablo[tmp-1][j] != tablo[i][j]))//buldugumuz hucre, hucremize esit degilse veya toplama iznimiz yoksa
							{
								sum = 0;
								tablo[tmp][j] = tablo[i][j];//buldugumuz hucrenin bir altindaki hucreye yerles
								if(tmp != i)//hucre hareket etmisse yani sifirdan farkli olan hemen bir ustumuzdeki hucre degilse
								{tablo[i][j] = 0;}//bulundugumuz hucreyi sifirla
							}
							else
							{
								if(tmp == i) {poprandom = 1;}//toplama islemi yapildiysa yeni sayi uretilebilir
								tablo[tmp-1][j] = tablo[i][j] * 2;
								score = score + tablo[tmp-1][j];
								tablo[i][j] = 0;
								sum = 1;//bundan sonraki hucreler esitse toplama islemine izin verme
							}
							
						}
					
					}
					
				
				}
			}
		}
		else if(ch == down)//asagi kaydirdigimizda
		{
			for(j = 0; j < 4; j++)
			{
				sum = 0;
				
				for(i = 3; i >= 0; i--)
				{
					if(tablo[i][j] != 0 && i != 3)//asagidan yukariya giderken, ustumuzdeki bir hucre sifirdan farkliysa
					{
						tmp = i;
						
						while(tmp < 4 && (tablo[tmp+1][j] == 0))//hucre sifir olmayana dek veya en uste ulasana dek yukari cik
						{
							tmp++;
						}
						if(tmp != i) poprandom = 1;//hareket edilmisse rastgele sayi uret
						
						if(tmp == 4)//en ustteki hucreye kadar, tum hucreler sifirsa
						{
							tablo[3][j] = tablo[i][j];//en ustteki hucreye yerles
							tablo[i][j] = 0;
							
						}
						else
						{
							if(sum == 1 || (tablo[tmp+1][j] != tablo[i][j]))//buldugumuz hucre, hucremize esit degilse veya toplama iznimiz yoksa
							{
								sum = 0;
								tablo[tmp][j] = tablo[i][j];
								if(tmp != i)//hucre hareket etmisse yani sifirdan farkli olan hemen bir altimizdaki hucre degilse
								{tablo[i][j] = 0;}
							}
							else
							{
								if(tmp == i) poprandom = 1;//sayilar birbirine esitse ve toplanabiliyorsa yeni sayi uretilebilir
								tablo[tmp+1][j] = tablo[i][j] * 2;
								score = score + tablo[tmp+1][j];
								tablo[i][j] = 0;
								sum = 1;//bundan sonraki toplamaya izin yok
								
							}
							
						}
					
					}
					
				
				}
			}
		}
		else if(ch == 'x' || ch == 'X')//Oyunu yeniden başlatmak istersek 
		{
			for(i = 0; i < 4; i++)
			{
				for(j = 0; j < 4; j++)
				{
					tablo[i][j] = 0;//tablodaki tüm değerleri sıfırla
					eskitablo[i][j] = 0;
				}	
			}
			//yeniden random sayı üret
			a = rand() % 4;
			b = rand() % 4;
			
			while(tablo[a][b] != 0)
			{
				a = rand() % 4;
				b = rand() % 4;
			}
			joker = 0;
			oldscore = 0;
			//skoru sıfırla
			score = 0;
			
			tablo[a][b] = rand() % 3 + 2;
			while(tablo[a][b] % 2 != 0)
			{
				tablo[a][b] = rand() % 3 + 2;
			}
			
		}
		else if(ch == 'e' || ch == 'E')//oyunu bitirmek istersek
		{
			sign = 1;
		}
		
		i = 0;
		while(i < 4 && tablo[i][j] != 2048)//tablodaki elemanlardan biri 2048 mi diye kontrol ediyoruz
		{
			j = 0;
			while(j < 4 && tablo[i][j] != 2048 )
				{j++;}
			if(j == 4)
			{
				i++;
			}
		}
		if(tablo[i][j] == 2048) sign = 1;//2048 olursa oyunu bitir
		
		
		//sayılardan biri 2048 ise tabloyu kontrol etmemize gerek yok
		if(sign == 0){
			i = 0; j = 4; 
			while(i < 4 && j == 4)//tablodaki tüm elemanlar 0'dan farklı mı diye kontrol ediyoruz
			{
				j = 0;
				while(j < 4 && tablo[i][j] != 0 )
					{j++;}
				i++;
			}
		}
		
		
		if(j == 4) //j = 4 ise tablodaki tüm elemanlar sıfırdan farklıdır 
		{
			i = 0; j = 3;
			while(i < 4 && j == 3)//yataydaki elemanların toplanıp toplanamayacağını kontrol ediyoruz
			{
				j = 0;
				while(j < 3 && tablo[i][j] != tablo[i][j+1])//tablodaki arka arkaya gelen iki sayi esit olmadigi surece dongu devam eder
					{j++;}
				i++;
			}
			
			if(j == 3)//yataydaki elemanlar toplanammıyorsa dikeydeki elemanların toplanabilirliğine bakıyoruz
			{
				j = 0; i = 3;
				while(j < 4 && i== 3)
				{
					i = 0;
					while(i < 3 && tablo[i][j] != tablo[i+1][j])//dikeyde ust uste elemanlar birbirine esit olmadigi surece
						{i++;}
					j++;
				}
				
				if( i == 3 )//dikeydeki elemanlar da toplanamıyorsa oyunun bittigini belirtiyoruz
				{
					sign = 1;//bu yuzden sign'ı sifirdan bire ceviriyoruz
				}
				else
					system("CLS");
					
				
			}
			else
				system("CLS");
				
			
		}
		else if((ch != 'r' || ch != 'R') && poprandom == 1)//eski tabloya geri donmediysek ve random sayi uretmemize izin verildiyse
		{
			a = rand() % 4;
			b = rand() % 4;
				
			while(tablo[a][b] != 0)
			{
				a = rand() % 4;
				b = rand() % 4;
			}
				
			tablo[a][b] = rand() % 3 + 2;
			while(tablo[a][b] % 2 != 0)
			{
				tablo[a][b] = rand() % 3 + 2;
			}
				
			poprandom = 0;
		
			
			system("CLS");	
		}
		else//sayi uretemiyorsak ya da R'ye basilmissa
			system("CLS");	
		
		if(sign == 1 && key != 0)//oyun bittiyse
		{
			system("CLS");
			
			print_tablo(tablo,score,maxscore,a,b,joker);//tablonun son halini ve skorlari yazdir
			
			printf("\n\t\t========================= GAME OVER ==============================\n\n");
			
			printf("\t\tLutfen isminizi giriniz : ");scanf("%s",isim_listesi[count]);//oynayan kisinin ismini alip listenin ilk satirina kaydediyoruz
			
			lastscore[count] = score;//oynayan kisinin skorunu ayri bir diziye kaydediyoruz
			count++;//oyun yeniden oynanmak isterse listenin 2. elemanina geciyoruz
			
			printf("\t\tOynayanlarin skorlarini gormek isterseniz 1'i\n\t\t");
			printf("Oyundan cikmak isterseniz 2'yi\n\t\tOyunu yeniden baslatmak isterseniz 3'u tuslayiniz\n\n");
			scanf("%d",&cevap);
			if(cevap == 1)//scoreboard'u aciyoruz
			{
				//printf("\n\n")
				system("CLS");
				
				print_liste(isim_listesi,lastscore,count);//scoreboard' yazdirma fonksiyonu
				
				printf("\n\n\t\tOYUNA DONUS = X\n\t\tOYUNDAN CIK = E\n");
				
				ch = getch();
				if(ch == -32)
				{
					ch = getch();
				}
				
				if(ch == 'x')//scoreboard uzerinden oyunu yeniden baslatmak istersek
				{
					system("CLS");
					
					for(i = 0; i < 4; i++)//tum elemanlari sifirliyoruz
					{
						for(j = 0; j < 4; j++)
						{
							tablo[i][j] = 0;
							eskitablo[i][j] = 0;
						}
					}
					//skoru ve sign'i sifirliyoruz ki oyun devam edebilsin
					score = 0;
					oldscore = 0;
					sign = 0;
					joker = 0;
					//baslangic olarak yine bos iki yere rastgele 2 ya da 4 sayisi atiyoruz
					for(i = 0; i < 2; i++)
					{
						a = rand() % 4;
						b = rand() % 4;
							
						while(tablo[a][b] != 0)
						{
							a = rand() % 4;
							b = rand() % 4;
						}
							
						tablo[a][b] = rand() % 3 + 2;
						while(tablo[a][b] % 2 != 0)
						{
							tablo[a][b] = rand() % 3 + 2;
						}
					}
				}
			}
			else if(cevap == 3)//oyun bittikten sonra direkt yeniden baslatmak istersek
			{
				for(i = 0; i < 4; i++)
				{
					for(j = 0; j < 4; j++)
					{
						tablo[i][j] = 0;
						eskitablo[i][j] = 0;
					}
				}
				joker = 0;
				oldscore = 0;
				score = 0;
				sign = 0;
				for(i = 0; i < 2; i++)
				{
					a = rand() % 4;
					b = rand() % 4;
						
					while(tablo[a][b] != 0)
					{
						a = rand() % 4;
						b = rand() % 4;
					}
						
					tablo[a][b] = rand() % 3 + 2;
					while(tablo[a][b] % 2 != 0)
					{
						tablo[a][b] = rand() % 3 + 2;
					}
				}
				system("CLS");
						
			}
			else
				system("CLS");
			
		}
		
		if(score != oldscore) joker++;//skorda değişim olmuşsa joker'i arttır
		
		
		
		
	}
	
	return 0;
}		
	
	

