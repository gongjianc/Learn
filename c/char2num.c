#include <stdio.h>
#include <string.h>

int mi(int base, int times)
{
    int res = 1;
    for(int i = 1; i <= times; i++)
        res *= base;
    return res;
}

int char2num(char *str)
{
    int length = strlen(str);
    printf("str is %s, len is %d\n", str, length);
    /* log_printf(ZLOG_LEVEL_DEBUG, "str is %d len", length); */
    int num = 0;

    int i;

    /* num = str[length - 1] - '0'; */
    /* printf("num is %d\n", num); */
    for(i = length - 1; i >= 0; i--){
        num += (str[i] - '0') * mi(10, length - 1 - i);
        printf("num is %d\n", num);
    }
    return num;
}

int main()
{
    char *test = "30";
    int num = char2num(test);
    printf("num is %d\n", num);
    return 0;

}
