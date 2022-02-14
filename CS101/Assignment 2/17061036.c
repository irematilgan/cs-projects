#include <stdio.h>
#include <stdlib.h>

int main()
{
	int d[100][100], counterPlace[50], rangestop[50], countstop[50], identical[50], place[50];
	
	int i,j,k, max, min, M, N, tmp, mostid;
	
	srand(time(NULL));//Her seferinde rasgele say� �retilebilmesi i�in yazd�m

	
	printf("Number of bus lines : ");scanf("%d",&N);
	printf("Number of stops : ");scanf("%d",&M);
	
	//	E�er kullanici bilgileri girecekse kullanilacak kod (Ben her seferinde farkl� elemanlar atanmas� i�in random fonk. kulland�m)
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
			d[i][j] = rand()%2 ; // matrisin elemanlarini atayarak vakit kaybetmektense 0-1 aral���nda rastgele eleman atanmas�n� sa�lad�m
			rangestop[j] = 0; //rangestop dizisi duraklar�n yo�unlu�unu bulmak i�in olu�turdu�umuz bir dizi
		}
		
		countstop[i] = 0; // countstop, her bir bus line'�n durak sayisini saklamak i�in olu�turdu�umuz bir dizi
	}
	
	
	for(i = 0; i < N; i++)
	{
		counterPlace[i] = i; // durak say�lar�n�n hangi bus line' a ait oldu�unu tutabilmek i�in counterPlace dizisini a�t�m
		for(j = 0; j < M; j++)
		{
			
			printf("%d\t",d[i][j]);
			
			if(d[i][j] == 1)
			{
				countstop[i]++;
				rangestop[j]++; //yeni bir d�ng� olu�turup dizinin eleman�n� artt�rmak yerine bu yolu kulland�m
			}
			
		}
		printf("-- stops = %d",countstop[i]);
		printf("\n");
	}
	
	
	printf("\n");
	
	//a�a��daki d�ng�de selection sort'tan yararlanarak stop dizisini b�y�kten k����e olacak �ekilde d�zenledik
	for(i = 0; i < N; i++)
	{
		max = i;
		
		
		for(j = i+1; j < N; j++ )
		{
			if(countstop[max] < countstop[j]) //t�m bus line'lar aras�nda hangisinin en fazla duraktan ge�ti�ini bulmak i�in k�yaslama yapt�m
			{
				max = j;
			}
		}
		
		tmp = countstop[i];
		countstop[i] = countstop[max];
		countstop[max] = tmp;
		
		//yeni bir tmp de�i�keni olu�turmak yerine ayn� de�i�keni kullanarak tasarruf etmeye �al��t�m
		//say�lar�n yeri de�i�irken eski konumlar�n�n yerini de koruyabilmek i�in counterPlace'i de swapledim
		tmp = counterPlace[max];
 		counterPlace[max] = counterPlace[i];
		counterPlace[i] = tmp;
		
		
		printf("Bus Line %d : %d stops\n",counterPlace[i]+1,countstop[i]);
	}
	
	printf("\n");
	
	//en yo�un ve en az yo�un dura�� bulabilmek i�in max ve min de�i�kenleri olu�turdum
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
	//en yo�un/en az yo�un duraklar birden fazla olabilece�i i�in min ve max de�erlerini veren t�m i'leri yazd�rd�m
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
	
	//en �ok benzeyen 2 bus line'� bulabilmek i�in 3'l� d�ng� a�t�m
	//b�ylece her sat�r kendinden sonraki sat�rlarla k�yaslan�p benzerlik say�s� tmp'de tutulacak
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
			if(tmp > max)//her sat�r�n en �ok hangi sat�rla benzedi�ini bulabilmek i�in tmp'yi maksimum benzerlik say�s�yla k�yaslar�z
			{
				//e�er tmp'de daha �ok benzerlik varsa o sat�r�n maksimum benzerlik say�s� tmp olur
				identical[i] = tmp;
				max = tmp;
				place[i] = j;//i sat�r�n�n hangi j sat�r�yla benzedi�ini saklayabilmek i�in place dizisini kullan�r�z
			}
			tmp = 0;
		}
	}
		
	mostid = 0;// most identical
	//"the most identical" � bulabilmek i�in yeni bir d�ng� a�ar�z
	for(i = 1; i < N-1; i++)
	{
		if(identical[mostid] < identical[i])
		{
			mostid = i;
		}
	}
	printf("The most identical bus lines  : %d and %d", place[mostid]+1, mostid+1);//birbirine en �ok benzeyen bus line'lar�n yerini yazd�r�yorum
		
	return 0;
	
	
	
	
}
