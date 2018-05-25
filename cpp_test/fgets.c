#include <stdio.h>
#include <strings.h>
#include <stdlib.h>

int main()
{
    FILE *fp = fopen("squid.conf", "r");
    if(NULL == fp){
        printf("fopen error\n");
        return -1;
    }
    char *s = (char *)malloc(1024);
    while(bzero(s, sizeof(s)), fgets(s, 1024, fp)){
        printf("s is %s", s);
    }
    return 0;
}
