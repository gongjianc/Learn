#include <stdio.h>

int main()
{
				printf("hello world\n");
				FILE* fp;
				
				fp = fopen("test","r+");
				perror("fopen");
				return 0;

}
