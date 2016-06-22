#include <stdio.h>
#include <string.h>

int main(void)
{
    char ca1[] = {'C', '+', '+'};
    char ca2[] = "C++";
    printf("sizeof of ca1 is %d\n", (int)sizeof(ca1));
    printf("strlen of ca1 is %d\n", (int)strlen(ca1));
    printf("sizeof of ca2 is %d\n", (int)sizeof(ca2));
    printf("strlen of ca2 is %d\n", (int)strlen(ca2));
    printf("-------\n");
    
    char a[2][2][3] = {{{1, 6, 3}, {5, 4, 15}}, {{3, 5, 33}, {23, 12, 7}}};
    for(int i = 0; i < 12; i++)
        printf("%d\n", *((int *)a + i));

    printf("-------\n");
    char x = '\0';
    printf("%d\n", x);

    return 0;
}
