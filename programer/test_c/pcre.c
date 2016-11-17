#include <stdio.h>
#include <pcre.h>
#include <string.h>
#include <strings.h>

#define OVECCOUNT 36

char ** IM_Match_Pcre(const char *src, const char *pattern, unsigned int n)
{
    pcre		 *re = NULL; //被编译好的正则表达式的pcre内部表示结构 
    const char	 *error;     //输出参数，用来输出错误信息
    int			 erroffset;   //输出参数，pattern中出错位置的偏移量 
    int			 ovector[OVECCOUNT]; //输出参数，用来返回匹配位置偏移量的数组
    int			 rc;                 //匹配成功返回非负数，没有匹配返回负数 
    const char   *stringptr = NULL;         //指向那个提取到的子串的指针
    unsigned int i;
    char 		 **result = NULL;
    /*正则表达式*/
    //char pattern[] = ".*&filekey=(.*)&filesize=(.*)&bmd5=.*&range=(.*)";
    re = pcre_compile(pattern,     // pattern, 输入参数，将要被编译的字符串形式的正则表达式 
            0,             // options, 输入参数，用来指定编译时的一些选项 
            &error,         // errptr, 输出参数，用来输出错误信息 
            &erroffset,     // erroffset, 输出参数，pattern中出错位置的偏移量 
            NULL);         // tableptr, 输入参数，用来指定字符表，一般情况用NULL 
    // 返回值：被编译好的正则表达式的pcre内部表示结构 
    if (re == NULL)
    {//如果编译失败，返回错误信息 
        printf("PCRE compilation failed at offset %d: %s in -----%s------%s------%d", erroffset, error,__FILE__, __func__, __LINE__); 
        return NULL; 
    } 
    rc = pcre_exec(re,    // code, 输入参数，用pcre_compile编译好的正则表达结构的指针 
            NULL,  // extra, 输入参数，用来向pcre_exec传一些额外的数据信息的结构的指针 
            src,   // subject, 输入参数，要被用来匹配的字符串 
            strlen(src),  // length, 输入参数，要被用来匹配的字符串的指针 
            0,     // startoffset, 输入参数，用来指定subject从什么位置开始被匹配的偏移量 
            0,     // options, 输入参数，用来指定匹配过程中的一些选项 
            ovector,        // ovector, 输出参数，用来返回匹配位置偏移量的数组 
            OVECCOUNT);    // ovecsize, 输入参数， 用来返回匹配位置偏移量的数组的最大大小 
    // 返回值：匹配成功返回非负数，没有匹配返回负数 
    if (rc < 0)
    {//如果没有匹配，返回错误信息 
        printf("pcre_exec error\n");
        pcre_free(re);    
        return NULL; 
    }
    if( NULL == (result = (char **)malloc( n * sizeof(char *))))
    {
        printf("Insufficient_memory: -----%s------%s------%d\n",__FILE__, __func__, __LINE__);
        pcre_free(re);
        return NULL;
    }
    memset(result, '\0', n * sizeof(char *));
    for(i = 0; i < n; i++)
    {
        pcre_get_substring(src, ovector, rc, i + 1, &stringptr);
        if( NULL == (result[i] = (char *)malloc( sizeof(char) * (strlen(stringptr) + 1))))
        {
            printf("Insufficient_memory: -----%s------%s------%d\n",__FILE__, __func__, __LINE__);
            pcre_free(re);
            return NULL;
        }
        memset(result[i], '\0', sizeof(char) * (strlen(stringptr) + 1));
        memcpy(result[i], stringptr, strlen(stringptr));
        pcre_free_substring(stringptr);
    } 
    pcre_free(re);   // 释放编译正则表达式re的内存
    return result;
}

int main(void)
{
#if 0
    char src[] = "/rest/2.0/pcs/superfile2?\
                  method=upload&app_id=250528&channel=chunlei&clienttype=0\
                  &web=1&BDUSS=pansec_DCb740ccc5511e5e8fedcff06b081203-nDu7\
                  5NGxQ4A57Bm%2BN5qcksghCdzS%2BmY0rBZaZ%2B4wvr1b2fHtB%2FJSh\
                  A90%2BOqvi2DAEM%2BlgAIPbyKXjeTHn1OjG0Df9N07mZFyFYmovwMVMub\
                  JtHyqENptMrblimyEpGC0Oc1Pl8DgeiEhd4dp6BBOrxq6eLFSVnVKo9mz\
                  3%2FeRBX4IEQAmX7%2FRUXI5aAssXHUv0R2p9qsY0PTR9eIxMqQwlVnJg\
                  O8ob8hSh5%2F5CgkNcdjf0sXL4xOpt8rEjhAlsv9EdMLABwWnMQgGqGlUDve6eA%3D%3D\
                  &logid=MTQ3OTE5NTY2NzQwNjAuMjE0ODk0NDg3OTMyMjkyMjc=\
                  &path=%2Ftoday-2016-11-15-15.txt\
                  &uploadid=N1-MTIzLjEzOS4yOS4xNjQ6MTQ3OTE5NTc1MTo3NDEzNjkxMTE5OTk4ODc0Mzg2\
                  &partseq=0";

    const char *DataUrlReg = "^/rest/2.0/pcs/superfile2.+&logid=(.+)&path=.+&partseq=(.+)";
    /* const char *DataUrlReg = "^/rest/2.0/pcs/superfile2.+&partseq=(.+)&client_ip=.+&logid=(.+)"; */
    /* const char *DataUrlReg = "^/rest/2.0/pcs/superfile2?(.+)"; //.+&partseq=";//(.+)&client_ip=.+&logid=(.+)"; */
    char **UrlInformation = NULL;

    UrlInformation = IM_Match_Pcre(src, DataUrlReg, 2);

    if(NULL != UrlInformation){
        printf("UrlInformation[0]: %s\n", UrlInformation[0]);
        printf("UrlInformation[1]: %s\n", UrlInformation[1]);
    }else{
        printf("error\n");
    }
#endif

    pcre *re = NULL;
    const char *error;
    int	erroffset;
    int	ovector[OVECCOUNT];
    int	rc;
    char boundary[] = "---------------------------19972116427593";
    char pattern[1024] = {0};
    FILE *fmsg = NULL;

    sprintf(pattern, "(^--%s\\r\\nContent-Disposition:.+\\r\\nContent-Type:.+\\r\\n\\r\\n)([\\s\\S]*)(\\r\\n--%s--\\r\\n)", boundary, boundary);

    fmsg = fopen("baidu.txt", "r");
    if(NULL == fmsg){
        perror("fopen");
        return -1;
    }

    char *src = (char *)malloc(sizeof(char) * 1254);
    if(NULL == src){
        perror("malloc");
        return -2;
    }
    bzero(src, sizeof(src));
    fread(src, 1254, 1, fmsg);
    fclose(fmsg);
    fmsg = NULL;

    re = pcre_compile(pattern,     // pattern, 输入参数，将要被编译的字符串形式的正则表达式 
            0,             // options, 输入参数，用来指定编译时的一些选项 
            &error,         // errptr, 输出参数，用来输出错误信息 
            &erroffset,     // erroffset, 输出参数，pattern中出错位置的偏移量 
            NULL);         // tableptr, 输入参数，用来指定字符表，一般情况用NULL 
    // 返回值：被编译好的正则表达式的pcre内部表示结构 
    if (re == NULL)
    {//如果编译失败，返回错误信息 
        printf("PCRE compilation failed at offset %d: %s in -----%s------%s------%d", erroffset, error,__FILE__, __func__, __LINE__); 
        return -3; 
    } 

    rc = pcre_exec(re,    // code, 输入参数，用pcre_compile编译好的正则表达结构的指针 
            NULL,  // extra, 输入参数，用来向pcre_exec传一些额外的数据信息的结构的指针 
            src,   // subject, 输入参数，要被用来匹配的字符串 
            239,  // length, 输入参数，要被用来匹配的字符串的指针 
            0,     // startoffset, 输入参数，用来指定subject从什么位置开始被匹配的偏移量 
            0,     // options, 输入参数，用来指定匹配过程中的一些选项 
            ovector,        // ovector, 输出参数，用来返回匹配位置偏移量的数组 
            OVECCOUNT);    // ovecsize, 输入参数， 用来返回匹配位置偏移量的数组的最大大小 
    // 返回值：匹配成功返回非负数，没有匹配返回负数 
    
    /* 如果没有匹配，返回错误信息  */
    if (rc < 0){
        printf("pcre_exec: NO RESULT\n");
        pcre_free(re);    
        return -3; 
    }
    int i;
    for(i = 0; i < 8; i++){
        printf("ovector[%d] is %d\n", i, ovector[i]);
    }

    return 0;
}
