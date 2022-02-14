#include <stdio.h>
#include <stdlib.h>
#define MAX 100
typedef struct STACK{
	int items[MAX];
	int top;
}STACK;

int isEmpty(STACK* s)
{
	if(s->top == 0)
		return 1;
	else
		return 0;
	
}

int isFull(STACK* s)
{
	if(s->top == MAX)
		return 1;
	else
		return 0;
}

void initStack(STACK* s)
{
	s->top = 0;
}

int pushStack(STACK* s, int item)
{
	if(isFull(s))
		return 0;
	else
	{
		s->items[(s->top)++] = item;
		return 1;
	}
		
}

int popStack(STACK *s,int* x)
{
	if(isEmpty(s))
		return 0;
	else
	{
		--s->top;
		*x = s->items[s->top];
	}
}

int peak(STACK* s, int* x)
{
	if(isEmpty(s))
		return 0;
	else
	{
		*x = s->items[(s->top)-1];
		return 1;
	}
}

int main()
{
	STACK *stek = malloc(sizeof(STACK));
	initStack(stek);
	
	int x;
	pushStack(stek,5);
	pushStack(stek,7);
	peak(stek,&x);
	printf("%d",x);
	pushStack(stek,1);
	pushStack(stek,6);
	peak(stek,&x);
	printf("\n%d",x);
	pushStack(stek,8);
	peak(stek,&x);
	printf("\n%d",x);	
	
	
}

