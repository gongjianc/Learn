#include <stdio.h>

int main(void)
{
	int a = 10;
	int *iptr = &a;
	printf("Size of Interger Pointer : %d Bytes\n",(int)sizeof(iptr));

	return 0;
}

