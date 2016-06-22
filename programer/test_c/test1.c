#include <stdio.h>

int main(void)
{
    printf("char is %c\n", '\101');
    const char ch3[6] = "Daniel";
    printf("ch3 is %s, sizeof ch3 is %d\n", ch3, (int)sizeof(ch3));
    
    return 0;
}
