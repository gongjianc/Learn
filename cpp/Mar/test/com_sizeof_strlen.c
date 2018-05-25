#include <string.h>
#include <stdio.h>

int main(void)
{
    char buf[1024] = "hello,world";
    printf("sizeof(buf) = %d\n", (int)sizeof(buf));
    printf("strlen(buf) = %d\n", (int)strlen(buf));
    return 0;
}
