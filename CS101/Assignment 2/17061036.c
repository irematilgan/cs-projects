#include <stdio.h>
#include <stdlib.h>

int main()
{
	int d[100][100], counterPlace[50], rangestop[50], countstop[50], identical[50], place[50];
	
	int i,j,k, max, min, M, N, tmp, mostid;
	
	srand(time(NULL));//Her seferinde rasgele sayý üretilebilmesi için yazdým

	
	printf("Number of bus lines : ");scanf("%d",&N);
	printf("Number of stops : ");scanf("%d",&M);
	
	//	Eðer kullanici bilgileri girecekse kullanilacak kod (Ben her seferinde farklý elemanlar atanmasý için random fonk. kullandým)
	/*
	for(i = 0; i < N; i++)
	{
		countstop[i] = 0;
		counterPlace[i] = i;
		for(j = 0; j < M; j++)
		{
			rangestop[j] = 0;
			printf("%d. otobus guzergahi %d. duraktan gecer mi?\nEvet ise 1'i Hayir ise 0'i tuslayiniz\n",i+1,j+1);
			scanf("%d",&d[i][j]);
			while(d[i][j] != 1 && d[i][j] != 0)
			{
				printf("Lutfen tekrar deneyiniz\n");
				scanf("%d",&d[i][j]);
			}
			
		}
	}

	
	for(i = 0; i < N; i++)
	{
		for(j = 0; j < M; j++)
		{
			printf("%d\t",d[i][j]);
			if(d[i][j] == 1)
			{
				countstop[i]++;
				rangestop[j]++;
			}

		}
			printf("-- stops = %d",countstop[i]);
			printf("\n");
		
	}
	*/
	
	
	
	for(i = 0; i < N; i++)
	{
		for(j = 0; j < M; j++)
		{
			d[i][j] = rand()%2 ; // matrisin elemanlarini atayarak vakit kaybetmektense 0-1 aralýðýnda rastgele eleman atanmasýný saðladým
			rangestop[j] = 0; //rangestop dizisi duraklarýn yoðunluðunu bulmak için oluþturduðumuz bir dizi
		}
		
		countstop[i] = 0; // countstop, her bir bus line'ýn durak sayisini saklamak için oluþturduðumuz bir dizi
	}
	
	
	for(i = 0; i < N; i++)
	{
		counterPlace[i] = i; // durak sayýlarýnýn hangi bus line' a ait olduðunu tutabilmek için counterPlace dizisini açtým
		for(j = 0; j < M; j++)
		{
			
			printf("%d\t",d[i][j]);
			
			if(d[i][j] == 1)
			{
				countstop[i]++;
				rangestop[j]++; //yeni bir döngü oluþturup dizinin elemanýný arttýrmak yerine bu yolu kullandým
			}
			
		}
		printf("-- stops = %d",countstop[i]);
		printf("\n");
	}
	
	
	printf("\n");
	
	//aþaðýdaki döngüde selection sort'tan yararlanarak stop dizisini büyükten küçüðe olacak þekilde düzenledik
	for(i = 0; i < N; i++)
	{
		max = i;
		
		
		for(j = i+1; j < N; j++ )
		{
			if(countstop[max] < countstop[j]) //tüm bus line'lar arasýnda hangisinin en fazla duraktan geçtiðini bulmak için kýyaslama yaptým
			{
				max = j;
			}
		}
		
		tmp = countstop[i];
		countstop[i] = countstop[max];
		countstop[max] = tmp;
		
		//yeni bir tmp deðiþkeni oluþturmak yerine ayný deðiþkeni kullanarak tasarruf etmeye çalýþtým
		//sayýlarýn yeri deðiþirken eski konumlarýnýn yerini de koruyabilmek için counterPlace'i de swapledim
		tmp = counterPlace[max];
 		counterPlace[max] = counterPlace[i];
		counterPlace[i] = tmp;
		
		
		printf("Bus Line %d : %d stops\n",counterPlace[i]+1,countstop[i]);
	}
	
	printf("\n");
	
	//en yoðun ve en az yoðun duraðý bulabilmek için max ve min deðiþkenleri oluþturdum
	max = rangestop[0];
	min = rangestop[0];
	
	for(i = 1; i < M; i++)
	{
		if(max < rangestop[i])
		{
			max = rangestop[i];
		}
		else if(min > rangestop[i])
		{
			min = rangestop[i];
		}
	}
	//en yoðun/en az yoðun duraklar birden fazla olabileceði için min ve max deðerlerini veren tüm i'leri yazdýrdým
	printf("The most busy stop : ");
	for(i = 0; i < M; i++)
	{
		if(rangestop[i] == max)
		{
			printf("%d ",i+1);
		}
	}
	printf("\nThe least busy stop : ");
	for(i = 0; i < M; i++)
	{
		if(rangestop[i] == min)
		{
			printf("%d ",i+1);
		}
	}
	
	printf("\n");
	
	//en çok benzeyen 2 bus line'ý bulabilmek için 3'lü döngü açtým
	//böylece her satýr kendinden sonraki satýrlarla kýyaslanýp benzerlik sayýsý tmp'de tutulacak
	for(i = 0; i < N-1; i++)
	{
		max = 0, tmp = 0;
		identical[i] = 0;
		
		for(j = i+1; j < N; j++)
		{
			for(k = 0; k < M; k++)
			{
				if(d[i][k] == 1 && d[j][k] == 1 )
				{
					tmp++;
				}
			}
			if(tmp > max)//her satýrýn en çok hangi satýrla benzediðini bulabilmek için tmp'yi maksimum benzerlik sayýsýyla kýyaslarýz
			{
				//eðer tmp'de daha çok benzerlik varsa o satýrýn maksimum benzerlik sayýsý tmp olur
				identical[i] = tmp;
				max = tmp;
				place[i] = j;//i satýrýnýn hangi j satýrýyla benzediðini saklayabilmek için place dizisini kullanýrýz
			}
			tmp = 0;
		}
	}
		
	mostid = 0;// most identical
	//"the most identical" ý bulabilmek için yeni bir döngü açarýz
	for(i = 1; i < N-1; i++)
	{
		if(identical[mostid] < identical[i])
		{
			mostid = i;
		}
	}
	printf("The most identical bus lines  : %d and %d", place[mostid]+1, mostid+1);//birbirine en çok benzeyen bus line'larýn yerini yazdýrýyorum
		
	return 0;
	
	
	
	
}
