#include <stdio.h>

int main(void)
{
    int a = 2;
    int *pa = &a;
    printf("sizeof(a) = %d\n", (int)sizeof(a++));
    printf("sizeof(pa) = %d\n", (int)sizeof(pa));
    printf("a = %d\n", a);
    return 0;
}
