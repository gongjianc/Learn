#include <stdio.h>

int main(void)
{
    int a = 2;
    printf("sizeof(a) = %d\n", (int)sizeof(a++));
    printf("a = %d\n", a);
    return 0;
}
