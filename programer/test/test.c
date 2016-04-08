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
    return 0;
}
