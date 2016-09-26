#include <stdio.h>
#include <stdlib.h>
#include <strings.h>
#include <string.h>

#define NON_NUM '0'

int hex2num(char c)
{
    if (c>='0' && c<='9') return c - '0';
    if (c>='a' && c<='z') return c - 'a' + 10;
    if (c>='A' && c<='Z') return c - 'A' + 10;

    return NON_NUM;
}

int web_URLDecode(const char* str, const int strSize, char* result, const int resultSize)
{
    char ch,ch1,ch2;
    int i;
    int j = 0;//record result index

    if ((str==NULL) || (result==NULL) || (strSize<=0) || (resultSize<=0)) {
        return 0;
    }

    for ( i=0; (i<strSize) && (j<resultSize); ++i) {
        ch = str[i];
        switch (ch) {
            case '+':
                result[j++] = ' ';
                break;
            case '%':
                if (i+2 < strSize) {
                    ch1 = hex2num(str[i+1]);
                    ch2 = hex2num(str[i+2]);
                    if ((ch1!=NON_NUM) && (ch2!=NON_NUM))
                        result[j++] = (char)((ch1<<4) | ch2);
                    i += 2;
                    break;
                } else {
                    break;
                }
            default:
                result[j++] = ch;
                break;
        }
    }

    result[j] = 0;
    return j;
}


int main(void)
{
    FILE *fp = fopen("QQmail.txt", "r");
    if(NULL == fp){
        perror("fopen");
        return -1;
    }else
        printf("success \n");

    char *str = (char *)malloc(4096);
    bzero(str, sizeof(str));

    //读取文件
    fread(str, 1024, 4, fp);
    printf("%s\n", str);

    char *strResult = (char *)malloc(4096);
    bzero(strResult, sizeof(strResult));

    printf("strSize = %d, resSize = %d \n", sizeof(str), sizeof(strResult));

    int res1 = web_URLDecode(str, strlen(str), strResult, strlen(str));

    //printf("res1 = %d\n", res1);

    printf("\n\nThe result is :\n%s\n", strResult);
    
    
    free(str);
    free(strResult);
    fclose(fp);
    return 0;
}
