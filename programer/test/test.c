#include <stdio.h>
#include <strings.h>
#include <stdlib.h>

int main(void)
{
    FILE *fp = fopen("163mail.txt", "r");
    if(NULL == fp){
        perror("fopen");
        return -1;
    }else
        printf("success \n");

    char *str = (char *)malloc(4096);
    bzero(str, sizeof(str));

    //读取文件
    fread(str, 1024, 4, fp);
    printf("%s", str);

    webmail_string_and_len *StoMB = init_webmail_string_and_len();


    dlp_webmail_info *webmail = NULL;
    
    

    free(str);
    fclose(fp);
    return 0;
}

