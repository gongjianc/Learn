#include <stdio.h>
#include <iostream>

using std::cout;


int strToNum(const char *str)
{
    int ret;
    const char *p = str;
    while(*p){
        if(*p >= '0' && *p <= '9'){
            ret = ret * 10 + (*p - '0');
            ++p;
        }else{
            printf("illegle\n");
            return -1;
        }
    }
    return ret;
}

int main(void)
{
    char src[128];
    printf("enter the string : ");
    scanf("%s", src);
    printf("the result is : %d", strToNum(src));
    return 0;
}
    
