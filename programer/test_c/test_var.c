#include <stdio.h>

int main(void)
{
    int i;
    for(i = 0; i < 6; i++)
        printf("i = %d\n", i);

    while(i++ < 6)
        printf("i = %d\n", i);

    printf("finish\n");
    return 0;
}
