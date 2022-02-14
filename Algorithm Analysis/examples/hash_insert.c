#include <stdio.h>
#include <stdlib.h>

int insert(int* hash, int key, int m) {
	int i = 0;
	int adr = key%m;
	while(i < m && hash[adr] != NULL && hash[adr] != key) {
		i++;
		adr = ((adr+1)%m);
	}
	if(i == m)	return 0;
	else {
		if(hash[adr] == NULL) {
			hash[adr] = key;
			return 1;
		} else {
			return 0;
		}
	}
}

int main()
{
	int key1 = 5, key2 = 12;
	int i;
	int* hash = (int*)malloc(sizeof(int)*7);
	for(i = 0; i < 7; i++)
		hash[i] = NULL;
	if(insert(hash,key1,7) && insert(hash,key2,7) && insert(hash,19,7))
		printf("Yerlestirme Basarili!..\n");
	
	
	printf("\n");
	
	for(i = 0; i < 7; i++) {
		if(hash[i] == NULL)
			printf("0\n");		
		else
			printf("%d\n",hash[i]);
		
	}
	
	return 0;
}
