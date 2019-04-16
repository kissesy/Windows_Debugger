#include <stdio.h>

//#pragma pack(push, 1)

typedef struct _Point
{
	short a; 
	long b; 
	short c;
	long d; 	
}Point; 

//a의 자료형을 기준으로 크기가 증가함! 

//#pragma pack(pop) 

int main(void)
{
	int offset[5] = {0,2,4,6,8};
   	int offset1[5] = {0,1,2,3,4};	
	Point point = {1,2,3,4};
	Point* ptr = &point;
	//long* pointer = (long*)&pointi;
	printf("%d\n", *ptr);
	printf("%p %p %p %p\n",&ptr->a, &ptr->b, &ptr->c, &ptr->d);
	for(int i=0;i<4;i++)
	{
		printf("%p %d\n",((ptr+offset1[i]) ,*((ptr+offset1[i])))); 
	}	
	return 0;
}
