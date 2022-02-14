#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

FILE* openFile(char[]);
void scanFile(FILE*,char[]);
void findwBMH(char[],char[],char[],int,int*);
int replace(char[],char[],int,int);
float timedifference_msec(struct timeval,struct timeval);


int main()
{
	char word[100];
	char newWord[100];
	char fname[20];
	char c;
	int i = 0;
	int counter = 0;//count replacements
	struct timeval t0,t1;
	float elapsed;
	
	char csensitive;//case sensitiveness indicator
	
	char *text = (char*)calloc(5000,sizeof(char)); 

	
	
	printf("\nFind : "); 
	while((word[i] = getchar()) != '\n'){
		i++;
	}
	word[i] = '\0';
	printf("Replace : ");
	i = 0;
	while((newWord[i] = getchar()) != '\n'){
		i++;
	}
	newWord[i] = '\0';
	
	printf("Case sensitive or not [y/n] : ");
	
	csensitive = getchar();
	
	FILE *fp = openFile(fname);
	scanFile(fp,text);
	
	
	if(strlen(text) < strlen(word)){
		printf("Input word is larger than text!..");
		exit(1);
	}
	
	if(csensitive == 'y')
		csensitive = 1;
	else
		csensitive = 0;
		
		
	printf("\nText : %s",text);
	
	gettimeofday(&t0,0);
	
	findwBMH(word,text,newWord,csensitive,&counter);//Find with Boyer-Moore Horspool Algorithm
	
	gettimeofday(&t1,0);
	

	elapsed = timedifference_msec(t0, t1);
	
	printf("\nNew Text : %s",text);
	printf("\nWord Length : %d",strlen(word));
	printf("\nReplaced Word Length : %d",strlen(newWord));
	printf("\n\nSearching has completed!..Total change of words = %d",counter);
	printf("\nTotal Time : %lf ms",elapsed);
	
	freopen(fname,"w",fp);//To write the text, we have to delete existing string from the file
	fputs(text,fp);
	
	fclose(fp);
	free(text);
	
	return 0;
}

float timedifference_msec(struct timeval t0, struct timeval t1)
{
    return (t1.tv_sec - t0.tv_sec) * 1000.0f + (t1.tv_usec - t0.tv_usec) / 1000.0f;
}



FILE* openFile(char fname[]){
	
	FILE* fp;
	
	printf("\nFile name (txt): "); scanf("%s",fname);
	strcat(fname,".txt");
	
	fp = fopen(fname,"r");
	
	return fp;
}

void scanFile(FILE* fp,char text[]){
	
	while(!feof(fp)){
		fgets(text,sizeof(char)*5000,fp);
	}
	
	
}

void findwBMH(char word[],char text[],char newWord[],int cs,int *counter){
	
	int length = strlen(word);//the word we will be searching for
	
	int badmatch[94];//Badmatch table takes words and punctuation marks
	
	
	
	int i,j = length-1;
	
	for(i = 0; i < 94; i++){
		badmatch[i] = length;	//If the letter is not in the word we are looking for,
								//we have to shift the letters at the amount of the length of the word
	}
	
	int offset = 32; //Space character is the offset
	
	for(i = 0; i < length-1;i++){
		
		badmatch[word[i]-offset] = length-i-1; 
		if(cs == 0){											//If It is not case sensitive, we have to fill both of the indexes
			if(word[i] >= 'A' && word[i] <= 'Z')					//If the letter is 'a', we have to fill the index of 'A' and 'a' otherwise
				badmatch[word[i]-offset+'a'-'A'] = length-i-1;
			else if(word[i] >= 'a' && word[i] <= 'z')
				badmatch[word[i]-offset+'A'-'a'] = length-i-1;
		}
	}
	

	badmatch[word[i]-offset] = length;	//Last letter of the word should be the length of the word
	i = length-1;
	while(j >= 0 && j < strlen(text)){
		
		
		if(text[j] == word[i]){//If two letters are identical
			
			j--;
			i--;
		}
		else{
			
			//If the are not identical, look if the user wants case sensitiveness and in the case they don't
			//Compare two letters (Lower Case -> Upper Case OR Upper Case -> Lower Case)
			
			if((cs == 0) && ((text[j] - 'A') == (word[i] - 'a') || (text[j] - 'a') == (word[i] - 'A'))){
				i--;
				j--;
				
			}
			else{
				//If the letters do not match
				
				i = length-1;//Take index to the end of the pattern
				j += badmatch[text[j]-offset];//Shift letters due to the badmatch table
			}
		}
		
		if(i < 0){//If we find the word
			
			struct timeval t0,t1;
			gettimeofday(&t0,0);
			
			j = replace(text,newWord,j+1,length);
			
			gettimeofday(&t1,0);
			float elapsed = timedifference_msec(t0,t1);
			printf("\nTotal Time for Replacing : %lf ms",elapsed);
			
			(*counter)++;
			i = length-1;	//In the case of we haven't finished the text,
							//We should keep looking
			
		}	
	}
	
}


int replace(char text[], char newWord[],int text_index,int len_word){
	
	
	
	int len_text = strlen(text);
	int len_replace = strlen(newWord);
	
	int k = 0, word_index = 0;
	int dif = len_replace-len_word;						//find the difference between lengths
	int returnval = text_index + len_replace + len_word;//the index we will continue after replacement process
	
	
	while(k < len_word && word_index < len_replace){	//Replace letters until we reach one of the word's end
		text[text_index++] = newWord[word_index++];		
		k++;
	}
	
	dif = abs(dif);	
	
	if(word_index < len_replace){//Case 1: Word we will replace has longer length than the word will be replaced
	
		text = (char*)realloc(text,(len_text+dif+1)*sizeof(char));	//create space for the new letters
		
		for(k = len_text+dif-1;k >= text_index+dif; k--){
			text[k] = text[k-dif];									//Shift other letters in the text to the end
		}
		text[len_text+dif] = '\0';									//Put '\0' at the end of the text to finish
		for(k = 0; k < dif; k++){
			text[text_index] = newWord[word_index];					//Add remaining letters to the end of the replaced word
			
			text_index++;
			word_index++;
			
		}
	}
	else if(len_replace  == word_index && k != len_word){//Case 2: Word we will replace has shorter length than the word in the text
		
		text_index += dif;								
		while(text_index < strlen(text)){				//Shift letters backwards
			
			*(text+text_index-dif) = text[text_index];	
			text_index++;
		}
		text[text_index-dif] = '\0';
	}
		
	return returnval;	//After we make replacement, we return the index where we will keep searching from
	
}
