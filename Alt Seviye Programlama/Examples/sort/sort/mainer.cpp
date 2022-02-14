#include <stdlib.h>
#include <stdio.h>
#include <time.h>

extern "C" void SelSortASM(int *, int);
void SelSortC(int *, int);
const int n = 100000;

int main(){
	int dizi[n];
	int i;
	double sure;
	clock_t bas, bit;

	for (i = 0; i < n; i++)
		dizi[i] = i + 1;
	
	printf("Bu program kucukten buyuge dogru siralanmis bir diziyi buyukten kucuge dogru siralamaktadir.\n\n");

	bas = clock();

	//SelSortASM(dizi, n);
	SelSortC(dizi, n);

	bit = clock();
	sure = (double)(bit - bas) / CLOCKS_PER_SEC;
	printf("Toplam sure: %.2f sn\n", sure);

	printf("Dizinin ilk elemani: %d\n Dizinin son elemani: %d\n", dizi[0], dizi[n - 1]);

	system("PAUSE");
	return 0;
}