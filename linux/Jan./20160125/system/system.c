#include <stdio.h>
#include <stdlib.h>

int main(void)
{
	printf("Running ls with system\n");
	system("ls -l");
	printf("I am main process\n");
	return 0;
}
