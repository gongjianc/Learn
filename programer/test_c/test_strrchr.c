#include <string.h>
#include <stdio.h>

int main(void)
{
    char *str = "Hello, world";
    char *ch = strrchr(str, ',');
    printf("ch is :%s\n", ch);
    return 0;
}
