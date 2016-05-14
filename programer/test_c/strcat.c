#include <stdio.h>

char *myStrcat(char *strDest, const char *strSrc)
{
    char *address = strDest;
    while(*strDest)
        strDest++;
    while((*strDest++ = *strSrc++));
    return address;
}

int main(void)
{
    char b[12];
    char a[] = "hello,world";
    myStrcat(b, a);
    printf("%s\n", b);
    return 0;
}
