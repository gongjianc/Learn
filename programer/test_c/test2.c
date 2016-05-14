#include <stdio.h>
#include <string.h>
#include <stdlib.h>

void getMemory(char *p)
{
    p = (char *)malloc(11);
}

int main(void)
{
    char *str = "hello";
    getMemory(str);
    strcpy(str, "hello world");
    printf("%s\n", str);
    return 0;
}
