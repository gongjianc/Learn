#include <stdio.h>
#include <strings.h>
#include <stdlib.h>
#include "webmail_attach.h"
#include "webmail_parser.h"

 /**
 * @brief c_malloc get the room to store the data dynamiclly
 *
 * @param obj
 * @param len
 */
int c_malloc(void **obj,int len)
{
    //  log_printf(ZLOG_LEVEL_DEBUG, "c_malloc start ....");
    *obj=(char*)malloc(len);
    if(*obj==NULL){
        return -1;
    }
    memset(*obj,'\0',len);
    //  log_printf(ZLOG_LEVEL_DEBUG, "c_malloc end ....");
    return 0;
}

/**
 * @brief c_free free the space which has been apply;
 *
 * @param obj
 */
void c_free(char**obj)
{
    //  log_printf(ZLOG_LEVEL_DEBUG, "c_free start ....");
    if(*obj!=NULL){
        free (*obj);
        *obj=NULL;
    }
    //  log_printf(ZLOG_LEVEL_DEBUG, "c_free end ....");
}



/*用于存储webmail程序中长度与内容的结构体*/
typedef struct DLP_WEBMAIL_PARSER_STRING_AND_STRLEN {
    int len;
    char *string;
} webmail_string_and_len;


/**
 * @brief 初始化结构体webmail_string_and_len
 *
 * @return*如果未成功申请空间，则返回为空，如果申请成功则返回所申请的var变量
 */
webmail_string_and_len *init_webmail_string_and_len()
{
    webmail_string_and_len *var ;
    c_malloc((void**)&var,sizeof(webmail_string_and_len));

    if(var != NULL)
    {
        var->len = 0;
        var->string = NULL;
        return var;
    }
    else{
        log_printf(ZLOG_LEVEL_ERROR, "init_webmail_string_and_len error ....");
        return NULL;
    }
}


/*用于存放提取的webmail信息的结构体*/
typedef struct mail{
    enum
    {
       no_jansson,/*不需要生成jansson串*/
       build_jansson,/*需要生成jansson串*/
       mail_end,/*提取大附件结束的标志，也许要生成jansson串*/
       mail_ing,/*大附件还没结束，不需要生成jansson串*/
    }state;/*是否需要生成jansson串的状态标志*/
    int pro_id;/*协议号，webmail协议号码是157，http的协议号是7*/
    char Time[MAX_TIME_LEN]; /*邮件发送时间*/
    char boundary[MAX_BOUNDARY_LEN];/*mime格式的boundary*/
    char uuid[UUID_LENGTH];/*文件的存储名uuid*/
    char host_ori_ref[HTTP_FILE_HOST_MAX+HTTP_FILE_ORIGIN_MAX+HTTP_FILE_REFERER_MAX];/*将http头中的host、origin、referer放到一个数组里*/
    char *From; /*发件人*/
    char *To;/*收件人*/
    char *Subject;/*邮件主题*/
    char *Content;/*邮件正文*/
    struct att_node *filenode; /*附件链表*/
}dlp_webmail_info;


/**
 * @brief webmail_to_parser
 * 将经过解码后的包含多个发件人的字符串进行分析，提取出所有的发件人到一个字符串内
 *
 * @param var
 * 待提取的字符串
 * @param sto
 * 存储结果
 *
 * @return
 */
int webmail_to_parser(webmail_string_and_len *var , char *sto )
{

    if(sto == NULL || var == NULL)return -1;
    /*正则所需变量的声明*/
    const char *pattern = "([a-zA-Z0-9_\\.\\-])+\\@(([a-zA-Z0-9\\-])+\\.)+([a-zA-Z0-9]{2,4})+";
    pcre *re;
    const char *error = NULL;
    int erroffset;
    int ovector[OVECCOUNT];
    int rc = 0;
    int Rloca = 0;
    char *substring_start = NULL;
    int substring_length = 0;
    char sto1[MAX_TO_LEN]={};

    /*循环将所要操作的对象中的所有邮箱提取到sto中，并各个邮箱以逗号作为分割*/
    re = pcre_compile(pattern, 0, &error, &erroffset, NULL);
    if (re == NULL) return -1;
    do{
        rc = 0;
        rc = pcre_exec(re, NULL, var->string, var->len, Rloca, 0, ovector, OVECCOUNT);
        if (rc < 0) break;
        if(Rloca>0||strlen(sto) !=0 ){
            strcat(sto , ",");
        }
        Rloca = ovector[1];
        substring_start = var->string + ovector[0];
        substring_length = ovector[1] - ovector[0];
        memcpy(sto1 , substring_start , substring_length);
        if(strlen(sto) + substring_length >MAX_TO_LEN){
            break;
        }
        strcat(sto , sto1);
        Rloca = ovector[1];
        memset(ovector , 0 , OVECCOUNT);
        memset(sto1 , 0 , MAX_TO_LEN);
    }
    while(rc>0) ;

    pcre_free(re);
    return 0;

}


/**
 * @brief time_convert  将时间戳转换为标准格式的时间
 *
 * @param time1         时间戳
 * @param s             放置转换后的时间
 *
 * @return
 */
int time_convert(char* time1,char*s)
{
    if(time1){
        char time[10]={};
        memcpy(time,time1,10);
        int t1;
        t1=atoi(time);
        time_t t;
        struct tm *p;
        t=(time_t)t1;
        p=localtime(&t);
        strftime(s, 20, "%Y-%m-%d %H:%M:%S", p);
        return 0;
    }
    return 0;
}



/**
 * @brief hex2num 十六进制转换到10进制
 *
 * @param c
 *
 * @return
 */
int hex2num(char c)
{
    if (c>='0' && c<='9') return c - '0';
    if (c>='a' && c<='z') return c - 'a' + 10;
    if (c>='A' && c<='Z') return c - 'A' + 10;

    return NON_NUM;
}

/**
 * @brief URLDecode 解码 例如：%0A%0D转换为对应的ASCII字符回车换行 web_urldecode 是将UTF8进行转码 即将“%40” 转换为 “@”的形式
 *
 * @param str  被转换的字符字符串
 * @param strSize   被转换字符串对应长度
 * @param result decoded string 存放转换后的字符串
 * @param resultSize 存放转换后字符串长度
 *
 * @return
 */
int web_URLDecode(const char* str, const int strSize, char* result, const int resultSize)
{
    char ch,ch1,ch2;
    int i;
    int j = 0;//record result index

    if ((str==NULL) || (result==NULL) || (strSize<=0) || (resultSize<=0)) {
        return 0;
    }

    for (i=0; (i<strSize) && (j<resultSize); ++i) {
        ch = str[i];
        switch (ch) {
            case '+':
                result[j++] = ' ';
                break;
            case '%':
                if (i+2<strSize) {
                    ch1 = hex2num(str[i+1]);
                    ch2 = hex2num(str[i+2]);

                    //#define NON_NUM '0'
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





/**
 * @brief mch       通过pcre正则引擎匹配字符串
 *
 * @param var       待检测的字符串
 * @param Sto2      将被匹配到的字符串放置到Sto里面
 * @param pattern   所需要的正则表达式
 *
 * @return
 */
int mch( webmail_string_and_len ** var, webmail_string_and_len ** Sto,const char *pattern)
{
    /*初始化正则匹配所需要的变量*/
    /*
     * re       放置编译后的正则串
     * error    放置出错信息
     * erroffset放置出错的位置
     * ovector  放置所匹配到的字符串的位置，ovetor【0】放置开始位置，ovector【1】放置结束位置
     * substring_start 放置匹配串在内存中的开始位置
     * substring_length 繁殖匹配串的长度
     */
    pcre *re; //<pcre.h>
    const char *error;
    int erroffset;
    int ovector[OVECCOUNT];
    int rc = 0;
    char *substring_start = NULL;
    int substring_length = 0;

    re = pcre_compile(pattern, 0, &error, &erroffset, NULL);
    if (re == NULL) {
        return -1;
    }

    /*执行正则匹配，如果匹配出错返回小于0的值，如果成功则返回大于零的值*/
    rc = pcre_exec(re, NULL, (*var)->string, (*var)->len, 0, 0, ovector, OVECCOUNT);
    if (rc < 0) {
        if (rc == PCRE_ERROR_NOMATCH){
            goto mch_fail;
        }
        else{
            log_printf(ZLOG_LEVEL_ERROR, "Matching error %d\n", rc);
            goto mch_fail;
        }
    }

    substring_start = (*var)->string + ovector[0];
    substring_length = ovector[1] - ovector[0];

    /*将匹配串放置到Sto->string中,并将匹配串的长度放置在Sto1->len中*/
    if(Sto!=NULL){
        (*Sto)->len = substring_length;
        if(c_malloc((void**)&(*Sto)->string,substring_length+1) ==-1) goto mch_fail; //lj
        memset((*Sto)->string,'\0', substring_length+1);
        memcpy((*Sto)->string, substring_start, substring_length);
    }

    /*释放对应资源*/
    pcre_free(re);
    re=NULL;
    return EXIT_SUCCESS; //<stdlib.h>

mch_fail:
    pcre_free(re);
    re=NULL;
    return EXIT_FAILURE;
}





/**
 * @brief wymail    提取通过网易邮箱发送的主题、正文、发送者、接收者等信息
 *
 * @param StoMB  待分析的消息体 Message Body
 * @param webmail   存储邮件的相应信息
 */
int wymail(webmail_string_and_len *StoMB, dlp_webmail_info *webmail)
{
    log_printf(ZLOG_LEVEL_DEBUG, "wymail start ....");
    if(StoMB==NULL || webmail ==NULL)
        return -1;

    /*初始化obj和sto
     * obj  存放解码后的数据体
     * obj 通过URLDecode
     * sto  则存放提取后的信息
     * */
    webmail_string_and_len *obj=init_webmail_string_and_len();

    webmail_string_and_len *sto=init_webmail_string_and_len();
    webmail_string_and_len *sto1=init_webmail_string_and_len();

    if(obj == NULL||sto == NULL||sto1 == NULL) 
        goto wymail_fail;

    unsigned int ln = StoMB->len;

    if(c_malloc((void**)&(obj->string),ln) == -1)
        goto wymail_fail;//lj

    /*web_urldecode 是将UTF8进行转码 即将“%40” 转换为 “@”的形式*/
    web_URLDecode(StoMB->string, ln, obj->string, ln);
    obj->len=StoMB->len;

    /*通过正则表达式提取发送人，并存入webmail->From中，便于之后产生jansson串*/
    /*此正则表达式的意义时提取出以account、n>开头，以</string>结尾的中间部分的字符串*/
    const char pattern2_1[PATTERN_LENGTH] = "(?<=account\">)[\\w\\W]*?(?=</string>)";
    const char pattern2_2[PATTERN_LENGTH] = "(?<=account\":\")[\\w\\W]*?(?=\",)";/*此正则表达式的意义时提取出以account、n>开头，以</string>结尾的中间部分的字符串*/

    const char pattern_mail[128] = "([a-zA-Z0-9_\\.\\-])+\\@(([a-zA-Z0-9\\-])+\\.)+([a-zA-Z0-9]{2,4})+";

    /*首先判断webmail中没有东西，并且能够提取到账户，进行下一步，否则跳出 ， 经过mch（）函数后，已经将账户提取到了sto结构体中*/
    if(webmail->From==NULL && (mch(&obj,&sto1, pattern2_1)==EXIT_SUCCESS) || (mch(&obj,&sto1, pattern2_2)==EXIT_SUCCESS)  ){
        if(sto1->len){/*判断sto里面的东西长度大于0，表明确实提取到了*/
            if(mch(&sto1,&sto, pattern_mail)==EXIT_SUCCESS){
                if(c_malloc((void**)&(webmail->From),sto->len+1) == -1) 
                    goto wymail_fail;//lj
                memcpy(webmail->From,sto->string,sto->len);
                c_free(&(sto->string));/*用过的结构体清空，便于下次使用*/
                c_free(&(sto1->string));
            }
        }
    }

    /*to收件人、cc抄送、bcc密送都认为时收件人并通过to_len来将三者拼接到一起*/
    /*3、4、5 pattern 都是提取收件人*/

    /*通过正则表达式提取收件人，并存入webmail->to中，便于之后产生jansson串*/
    const char pattern3_1[PATTERN_LENGTH] = "(?<=\"to\"><string>)[\\w\\W]*?(?=</array>)";
    const char pattern3_2[PATTERN_LENGTH] = "(?<=\"to\":\")[\\w\\W]*?(?=\",)";
    if(mch(&obj,&sto, pattern3_1)==EXIT_SUCCESS || mch(&obj,&sto, pattern3_2)==EXIT_SUCCESS){
        if(sto->len){
            webmail_to_parser(sto , webmail->To);
        }
        if (sto)
        {
            c_free(&(sto->string));
        }
    }

    //同上
    /*通过正则表达式提取收件人，并存入webmail->to中，便于之后产生jansson串*/
    const char pattern4_1[PATTERN_LENGTH] = "(?<=\"cc\"><string>)[\\w\\W]*?(?=<array)";
    const char pattern4_2[PATTERN_LENGTH] = "(?<=\"cc\":\")[\\w\\W]*?(?=\",)";
    if(mch(&obj,&sto, pattern4_1)==EXIT_SUCCESS || mch(&obj,&sto, pattern4_2)==EXIT_SUCCESS){
        if(sto->len){
            webmail_to_parser(sto , webmail->To);
        }
        if (sto)
        {
            c_free(&(sto->string));
        }
    }

    //同上
    /*通过正则表达式提取收件人，并存入webmail->to中，便于之后产生jansson串*/
    const char pattern5_1[PATTERN_LENGTH] = "(?<=\"bcc\"><string>)[\\w\\W]*?(?=<string name=\"subject\">)";
    const char pattern5_2[PATTERN_LENGTH] = "(?<=\"bcc\":\")[\\w\\W]*?(?=\",)";
    if(mch(&obj,&sto, pattern5_1)==EXIT_SUCCESS || mch(&obj,&sto, pattern5_2)==EXIT_SUCCESS){
        if(sto->len){
            webmail_to_parser(sto , webmail->To);
        }
        if (sto)
        {
            c_free(&(sto->string));
        }
    }

    /*提取发送时间*/
    const char pattern8[PATTERN_LENGTH] = "(?<=\"id\">c:)[\\w\\W]*?(?=</string>)";
    if(mch(&obj,&sto, pattern8)==EXIT_SUCCESS){
        if(sto->len){
            char s[MAX_TIME_LEN];
            memset(s,'\0',sizeof(s));
            /*将时间转换为规定的格式*/
            time_convert(sto->string,s);
            memcpy(webmail->Time,s,strlen(s));
        }
        if (sto)
        {
            c_free(&(sto->string));
        }
    }

    /*提取主题，并对应存储*/
    const char pattern6_1[PATTERN_LENGTH] = "(?<=subject\">)[\\w\\W]*?(?=</string>)";
    const char pattern6_2[PATTERN_LENGTH] = "(?<=subject\":\")[\\w\\W]*?(?=\",)";
    if(webmail->Subject==NULL&&mch(&obj,&sto, pattern6_1)==EXIT_SUCCESS || mch(&obj,&sto, pattern6_2)==EXIT_SUCCESS){
        if(sto->len){
            if(c_malloc((void**)&webmail->Subject,sto->len+1) == -1 ) goto wymail_fail;//lj
            memcpy(webmail->Subject, sto->string, sto->len);
        }
        if (sto)
        {
            c_free(&(sto->string));
        }
    }

    /*提取内容，并对应存储*/
    const char pattern7_1[PATTERN_LENGTH] = "(?<=content\">)[\\w\\W]*?(?=</string>)";
    const char pattern7_2[PATTERN_LENGTH] = "(?<=content\":\")[\\w\\W]*?(?=\",)";
    if(webmail->Content==NULL&&mch(&obj,&sto, pattern7_1)==EXIT_SUCCESS  || mch(&obj,&sto, pattern7_2)==EXIT_SUCCESS){
        if(sto->len){
            if(c_malloc((void**)&webmail->Content,sto->len+1) == -1 ) goto wymail_fail;//lj
            memcpy(webmail->Content,sto->string,sto->len);
        }
        if (sto)
        {
            c_free(&(sto->string));
        }
    }
    if(sto){
        free(sto);
        sto=NULL;
    }
    if(sto1){
        free(sto1);
        sto1=NULL;
    }
    if(obj){
        c_free(&(obj->string));
        free(obj);
        obj=NULL;
    }
    log_printf(ZLOG_LEVEL_DEBUG, "wymail end ....");
    return 0;

wymail_fail:
    if(sto){
        c_free(&(sto->string));
        free(sto);
        sto = NULL;
    }
    if(sto1){
        c_free(&(sto1->string));
        free(sto1);
        sto1=NULL;
    }
    if(obj){
        c_free(&(obj->string));
        free(obj);
        obj = NULL;
    }
    log_printf(ZLOG_LEVEL_ERROR, "wymail fail end ....");
    return -1;
}


/**
 * @brief wyattach      提取通过网易邮箱发送的文件
 *
 * @param dlp_http      经提取后的http头
 * @param StoPT         URL
 * @param StoMB         待分析数据体
 * @param webmail       存储邮件的相应信息
 */

int wyattach(HttpSession* dlp_http,webmail_string_and_len *StoPT,webmail_string_and_len *StoMB,dlp_webmail_info *webmail)
{
    log_printf(ZLOG_LEVEL_DEBUG, "wyattach start ....");
    if(dlp_http==NULL || StoPT==NULL || StoMB==NULL || webmail ==NULL)
        return -1;

    /*通过dlp_http 中的dlp_http_post_head类型的dlp_http来提取相应http的头信息*/
    dlp_http_post_head *http_session;
    http_session = (dlp_http_post_head *)dlp_http->http;

    /*通过四元组来生成hash值，来作为文件名来保存还未拼接完整的文件*/
    /*最终将拼接好的四元组放置在tuple_name中*/
    char tmp[128]= {};
    char tuple_name[128] = {};
    char filename[FILE_NAME_LENGTH]={0};
    char pathname[FILE_NAME_LENGTH]={0};
    get_tuple_name(dlp_http , tuple_name);
    sprintf(filename , "%s%s%s%s", session_stream , "/" , tuple_name , ".dlp");

    /*初始化所需要的变量*/
    /*
     *  now_length          现在已经拼接好的长度
     *  mail_upload_name    上传的文件的名字
     *  mail_upload_size    上传的文件的大小
     *  sto                 临时存储所提取信息
     */
    int now_length = 0;
    int mail_upload_size = 0;
    char mail_upload_name[FILE_NAME_LENGTH] = {};
    memcpy(mail_upload_name, http_session->file_name, FILE_NAME_LENGTH);
    mail_upload_size = http_session->up_load_size;
    webmail_string_and_len *sto=init_webmail_string_and_len();
    webmail_string_and_len *sto1=init_webmail_string_and_len();
    if(sto == NULL || sto1 == NULL) goto wyattach_fail; //lj

    /*如果已经存在有tuple_name命名的文件则打开这个文件，并在其之后继续拼接文件，如果不存在则建立这个文件*/
    FILE *fp=NULL;
    if(access(filename, F_OK) < 0){
        fp = fopen(filename, "w+");
        if(fp == NULL)
        {
            log_printf(ZLOG_LEVEL_ERROR, "wyattach file open is failing!!\n");
            return -1;
        }
        fclose(fp);
        fp=NULL;
    }
    fp = fopen(filename, "a+");
    if(fp == NULL)
    {
        log_printf(ZLOG_LEVEL_ERROR, "wyattach file open is failing!!\n");
        return -1;
    }

    /*获得现在该文件的大小*/
    fseek(fp, 0, SEEK_END );
    now_length = ftell(fp);
    /*将文件进行拼接*/
    fwrite(StoMB->string, http_session->content_length, 1,  fp);
    //printf("\n now the wyfile length is %d\n",now_length);

    /*如果现在文件的大小等于文件头部的文件大小，说明文件已经组合完整*/
    if (now_length+ http_session->content_length == mail_upload_size && mail_upload_size != 0) {
        webmail->state = mail_end;/*告知生成jansson串*/

        /*提取邮件的发件人，并对应存储便于之后生成jansson串*/
        const char pattern_mail[128] = "([a-zA-Z0-9_\\.\\-])+\\@(([a-zA-Z0-9\\-])+\\.)+([a-zA-Z0-9]{2,4})+";
        const char pattern3[PATTERN_LENGTH] = "(?<=uid=)[\\w\\W]*?(?=&cid)";
        if(webmail->From==NULL&&mch(&StoPT,&sto1,pattern3)==EXIT_SUCCESS){
            if(sto1->len){
                if(mch(&sto1,&sto, pattern_mail)==EXIT_SUCCESS){
                    if(c_malloc((void**)&(webmail->From),sto->len+1) == -1 ) goto wyattach_fail; //lj
                    memcpy(webmail->From,sto->string,sto->len);
                }
                c_free(&(sto1->string));
                c_free(&(sto->string));
            }
        }

        /*提取时间并将其转换成标准格式，其存在两种形式*/
        const char pattern5[PATTERN_LENGTH] = "(?<=cid=c:)[\\w\\W]*?(?=&)";
        const char pattern6[PATTERN_LENGTH] = "(?<=cid=)[\\w\\W]*?(?=&)";
        if(mch(&StoPT,&sto,pattern5)==EXIT_SUCCESS){
            if(sto->len){
                char s[MAX_TIME_LEN];
                memset(s,'\0',sizeof(s));
                time_convert(sto->string,s);
                memcpy(webmail->Time,s,strlen(s));
            }
            if (sto)
            {
                c_free(&(sto->string));
            }
        }
        else if (mch(&StoPT,&sto,pattern6)==EXIT_SUCCESS){
            if(sto->len){
                char s[MAX_TIME_LEN];
                memset(s,'\0',sizeof(s));
                time_convert(sto->string,s);
                memcpy(webmail->Time,s,strlen(s));
            }
            if (sto)
            {
                c_free(&(sto->string));
            }
        }

        /*用UUID作为文件名存储，之后将文件的原本名字、大小信息加入链表，便于之后生成jansson串*/
        add_att_nodelist(webmail->filenode, mail_upload_size, mail_upload_name, NULL);
        char uuid[UUID_LENGTH]={};
        uuid_t  wy_randNumber;
        uuid_generate(wy_randNumber);
        uuid_unparse(wy_randNumber, uuid);
        sprintf(pathname, "%s%s%s", session_stream , "/" , uuid);
        rename(filename , pathname);
        memcpy(webmail->uuid,uuid, UUID_LENGTH);
    }

    /*邮件还没有拼接完成*/
    else{
        webmail->state = mail_ing;/*告知不生成jansson串，邮件还为拼接完成*/
    }

    if(sto){
        free(sto);
        sto=NULL;
    }
    if(sto1){
        free(sto1);
        sto1=NULL;
    }
    if (fp)
    {
        fclose(fp);
        fp = NULL;
    }

    return 0;
wyattach_fail://lj
    if(sto)
    {
        c_free(&(sto->string));
        free(sto);
        sto = NULL;
    }
    if(sto1){
        c_free(&(sto1->string));
        free(sto1);
        sto1=NULL;
    }
    if (fp)
    {
        fclose(fp);
        fp = NULL;
    }
    log_printf(ZLOG_LEVEL_ERROR, "wyattach fail end ....");
    return -1;

}
/**
 * @brief wyattach1 提取通过网易网盘上传的文件
 *      此种是通过MIME格式传递文件的方式 则可以直接调用mch_att函数
 * @param StoBY
 * @param StoMB
 * @param webmail
 */
int wyattach1(webmail_string_and_len *StoBY,webmail_string_and_len *StoMB,dlp_webmail_info *webmail)
{
    log_printf(ZLOG_LEVEL_DEBUG, "wyattach1 start ....");
    if(StoBY==NULL || StoMB==NULL || webmail ==NULL)
        return -1;
    memcpy(webmail->boundary,StoBY->string,MAX_BOUNDARY_LEN);
    mch_att(&StoMB,webmail);
    return 0;

}





/**
 * @brief jdmail_att    判断具体为何种邮箱的何种流
 *
 * @param dlp_http      经提取后的http头
 * @param webmail       存储邮件的相应信息
 * @param host_ori_ref  包含host、origin、referer信息
 * @param webmailjson   存放生成的jansson串
 * @param sbuff_putdata
 * @param iringbuffer_ID
 *
 * @return
 */
int jdmail_att(HttpSession* dlp_http, dlp_webmail_info *webmail, char* host_ori_ref, char**webmailjson, http_sbuffputdata sbuff_putdata, const int iringbuffer_ID)
{
    //log_printf(ZLOG_LEVEL_DEBUG, "jdmail_att start ....");
    /*通过dlp_http 中的dlp_http_post_head类型的dlp_http来提取相应http的头信息*/
    dlp_http_post_head *http_session = NULL;
    http_session = (dlp_http_post_head *)dlp_http->http;


    /*初始化所需要的变量*/
    /*
     * StoPT        URL
     * StoBY        MIME格式中的boundary
     * StoCK        cookie信息
     * StoMB        待检测的数据体
     */
    webmail_string_and_len *StoPT = NULL;
    webmail_string_and_len *StoBY = NULL;
    webmail_string_and_len *StoCK = NULL;
    webmail_string_and_len *StoMB = NULL;

    /*将从上层提取的信息放置到上面申请的变量中*/
    FILE *pr = fopen(http_session->new_name,"r");
    if(pr == NULL){
        log_printf(ZLOG_LEVEL_ERROR, "When jdmail_att is processed,the open file is failed!!\n\n");
        //goto jdmail_att_fail;
        return -1;
    }

    StoPT= init_webmail_string_and_len();
    if(StoPT == NULL){
        log_printf(ZLOG_LEVEL_ERROR, "When we enter into jdmail_att,alloc memory StoPT failed!!\n");
        goto jdmail_att_fail;
    }

    StoBY = init_webmail_string_and_len();
    if(StoBY == NULL){
        log_printf(ZLOG_LEVEL_ERROR, "When we enter into jdmail_att,alloc memory StoBY failed!!\n");
        goto jdmail_att_fail;
    }

    StoCK = init_webmail_string_and_len();
    if(StoCK == NULL){
        log_printf(ZLOG_LEVEL_ERROR, "When we enter into jdmail_att,alloc memory StoCK failed!!\n");
        goto jdmail_att_fail;
    }

    StoMB = init_webmail_string_and_len();
    if(StoMB == NULL){
        log_printf(ZLOG_LEVEL_ERROR, "When we enter into jdmail_att,alloc memory StoMB failed!!\n");
        goto jdmail_att_fail;
    }

    StoMB->len=http_session->content_length;
    if(c_malloc((void**)&StoMB->string,StoMB->len+1) ==-1)
    {
        goto jdmail_att_fail;
    }
    fseek(pr,0,SEEK_SET);
    fread(StoMB->string,1,StoMB->len,pr);
    if (pr)
    {
        fclose(pr);
        pr = NULL;
    }

    //      if(remove(http_session->new_name))
    //              printf("the file can't delte %s \n",http_session->new_name );

    if(http_session->url){
        if(c_malloc((void**)&StoPT->string,HTTP_FILE_URL_MAX)==-1)goto jdmail_att_fail;
        memcpy(StoPT->string,http_session->url,HTTP_FILE_URL_MAX);
        StoPT->len=HTTP_FILE_URL_MAX;
    }
    if(strstr(http_session->content_type, "boundary")){
        if(c_malloc((void**)&StoBY->string,MAX_BOUNDARY_LEN)==-1)goto jdmail_att_fail;
        memcpy(StoBY->string,http_session->boundary,MAX_BOUNDARY_LEN);
        StoBY->len=MAX_BOUNDARY_LEN;
    }
    if(http_session->cookie){
        if(c_malloc((void**)&StoCK->string,strlen(http_session->cookie)+1) == -1) goto jdmail_att_fail;
        memcpy(StoCK->string,http_session->cookie,strlen(http_session->cookie));
        StoCK->len=strlen(http_session->cookie);
    }

    /*开始通过正则表达式，与字符串匹配的方法匹配url、origin、referer的信息来判断具体为何种邮箱的何种流，并进入到对应的函数中进行处理*/
    switch(jdmail_type(host_ori_ref)){
        case WYMAIL:
            {
                char *webmail_wy_mail_pattern1 = "&func=mbox:compose";
                char *webmail_wy_mail_pattern2 = "action=deliver";
                char *webmail_wy_mail_pattern3 = "compose/compose.do?action=DELIVER";
                char *webmail_wy_att_pattern = "/upxmail/upload";
                char *webmail_wy_att_pattern1 = "/compose/upload";
                //if (strstr(StoPT->string, webmail_wy_mail_pattern1)) {
                ///*下面判断的意思是：在URL 中如果找到了“&func=mbox:compose”,
                //并且同时找到了"action=deliver"则进入wymail（）函数，如果进入到wymail函数中出错了，则goto
                //释放资源，做出错处理*/
                if ((strstr(StoPT->string, webmail_wy_mail_pattern1) && strstr(StoPT->string, webmail_wy_mail_pattern2)) || strstr(StoPT->string , webmail_wy_mail_pattern3)) {
                    if(wymail(StoMB,webmail) ==-1) goto jdmail_att_fail;
                    webmail->state = build_jansson;
                }
                /*如果URL中没有找到相应的字符串，则开始判断是否里面包含"/upxmail/upload",同时头部项中有up_load_size项，则进入到wyattach函数进行处理*/
                else if (strstr(StoPT->string, webmail_wy_att_pattern)  && http_session->up_load_size) {
                    if(wyattach(dlp_http, StoPT, StoMB, webmail)==-1)goto jdmail_att_fail;
                }
                /*如果上面两项都不满足，继续判断里面是否有"/upxmail/upload",同时头部项中的content—type为MIME形式的，则进入到wyattach1函数进行处理*/
                else if ((strstr(StoPT->string, webmail_wy_att_pattern)||strstr(StoPT->string, webmail_wy_att_pattern1) ) && strstr(http_session->content_type, "boundary")) {
                    if(wyattach1(StoBY,StoMB,webmail)==-1)goto jdmail_att_fail;
                    webmail->state = build_jansson;
                }
                break;
            }
                case QMAIL:
            {
                char *webmail_qq_mail_pattern1 = "/cgi-bin/compose_send";
                char *webmail_qq_mail_pattern2 = "/cgi-bin/groupmail_send";//qq发送群邮件
                char *webmail_qq_att_pattern1 = "/ftn_handler";
                char *webmail_qq_att_pattern2 = "/cgi-bin/upload";
                if (strstr(StoPT->string, webmail_qq_mail_pattern1)) {
                    if(QQmail(StoMB,webmail)==-1)goto jdmail_att_fail;
                    webmail->state = build_jansson;
                }
                else if (strstr(StoPT->string, webmail_qq_mail_pattern2)) {
                    if(QQmail_qun(StoCK,StoMB,webmail)==-1)goto jdmail_att_fail;
                    webmail->state = build_jansson;
                }
                else if (strstr(StoPT->string, webmail_qq_att_pattern1)) {
                    if(QQattach(dlp_http, StoPT, StoBY, StoMB, webmail)==-1)goto jdmail_att_fail;
                }
                else if(strstr(StoPT->string,webmail_qq_att_pattern2)){
                    char *webmail_qq_att_pattern3 = "/cgi-bin/uploadfile";//分会话分post无乱码情况
                    char pattern_filesize[PATTERN_LENGTH]="(?<=size=)[\\W\\w]*?(?=&)";
                    char pattern_id[PATTERN_LENGTH]="(?<=storeid=)[\\W\\w]*?(?=&)";
                    /*下面这个式子的意思时首先检查URL中是否有"/cgi-bin/uploadfile"，并且能够在URL中正则匹配到pattern——filesize"(?<=size=)[\\W\\w]*?(?=&)"，再同时匹配到"(?<=storeid=)[\\W\\w]*?(?=&)",则进入QQattach2进行处理*/
                    if(strstr(StoPT->string,webmail_qq_att_pattern3) && mch(&StoPT,NULL,pattern_filesize)==EXIT_SUCCESS
                            &&mch(&StoPT,NULL,pattern_id)==EXIT_SUCCESS){
                        //printf("We will enter into QQattach2 process function!!\n\n");
                        if( QQattach2(dlp_http,StoPT,StoMB,webmail)==-1)goto jdmail_att_fail;
                    }
                    else
                        if( QQattachF(dlp_http,StoCK,StoMB,webmail)==-1)goto jdmail_att_fail;
                }
                break;
            }
                case CNPCMAIL:
            {
                char *webmail_cnpc_mail_pattern="/exchange.*";
                if(mch(&StoPT,NULL,webmail_cnpc_mail_pattern)==EXIT_SUCCESS){
                    char *webmail_cnpc_att_pattern="/exchange.*Cmd=addattach";
                    if(mch(&StoPT,NULL,webmail_cnpc_att_pattern)==EXIT_SUCCESS && strstr(http_session->content_type, "boundary")){
                        if(cnpc_attach(StoPT,StoBY,StoMB,webmail)==-1)goto jdmail_att_fail;
                        webmail->state = build_jansson;
                    }
                    else if(mch(&StoPT,NULL,webmail_cnpc_att_pattern)==EXIT_FAILURE){
                        if(cnpc_mail(StoPT,StoMB,webmail)==-1)goto jdmail_att_fail;
                        webmail->state = build_jansson;
                    }
                }
                break;
            }
                case SINAMAIL:
            {
                char *webmail_sina_mail_pattern = "/classic/send";
                char *webmail_sina_att_pattern1 = "/classic/uploadatt";
                //                char *webmail_sina_att_pattern1 = "upload";
                //              char *webmail_sina_att_pattern2 = "(?<=Host: |Host:)(\\w+\\.){0,2}upload-vdisk\\.\\w+\\.(com|cn|net){0,2}";
                char *webmail_sina_att_pattern2 = "/\\d*/files/basic/";
                char *webmail_sina_att_pattern3 = "diskupload";//linux网盘

                if(strstr(StoPT->string,webmail_sina_mail_pattern)&& strstr(http_session->content_type, "boundary")){
                    if(sina_mail(StoPT,StoBY,StoMB,webmail)==-1)goto jdmail_att_fail;
                    webmail->state = build_jansson;
                }
                else if(strstr(StoPT->string,webmail_sina_att_pattern1)||mch(&StoPT,NULL,webmail_sina_att_pattern2)==EXIT_SUCCESS||strstr(StoPT->string,webmail_sina_att_pattern3)){
                    if(sina_attach(StoPT,StoBY,StoMB,webmail)==-1)goto jdmail_att_fail;
                    webmail->state = build_jansson;
                }
                break;
            }
                case SOHUMAIL:
            {
                char *webmail_sohu_mail_pattern="/bapp/\\d*/mail";
                char *webmail_sohu_att_pattern="/bapp/\\d*/mail/att";
                char *webmail_sohu_att_pattern1="/flashUploadAction.action";//批量上传到优盘
                const char pattern_id[PATTERN_LENGTH]="(?<=xid)[\\W\\w]*?(?=&)";
                //printf("This is sohu function!!!!!!!!!!!\n");
                if(mch(&StoPT,NULL,webmail_sohu_mail_pattern)==EXIT_SUCCESS){
                    if(mch(&StoPT,NULL,webmail_sohu_att_pattern)==EXIT_SUCCESS && strstr(http_session->content_type, "boundary")){
                        if(sohu_attach(StoPT,StoBY,StoMB,webmail)==-1)goto jdmail_att_fail;
                        webmail->state = build_jansson;
                    }
                    else if(mch(&StoPT,NULL,webmail_sohu_att_pattern)==EXIT_FAILURE){
                        if(sohu_mail(StoMB,webmail)==-1)goto jdmail_att_fail;
                        webmail->state = build_jansson;
                    }
                }
                else if(strstr(StoPT->string,webmail_sohu_att_pattern1) && mch(&StoPT,NULL,pattern_id)==EXIT_SUCCESS){
                    //printf("This is sohu_attach2 function!!!!!!!!!!!\n");
                    if(sohu_attach2(dlp_http, StoPT, StoMB, webmail)==-1)goto jdmail_att_fail;
                }
                break;
            }
                case TOMMAIL:
            {
                char *webmail_tom_mail_pattern="/webmail/writemail/sendmail";
                char *webmail_tom_att_pattern="/webmail/writemail/.*uploadAttachment";
                if(strstr(StoPT->string,webmail_tom_mail_pattern)){
                    if(tom_mail(StoMB,webmail)==-1)goto jdmail_att_fail;
                    webmail->state = build_jansson;
                }
                else if(mch(&StoPT,NULL,webmail_tom_att_pattern)==EXIT_SUCCESS && strstr(http_session->content_type, "boundary")){
                    if(tom_attach(StoBY,StoMB,webmail)==-1)goto jdmail_att_fail;
                    webmail->state = build_jansson;
                }
                break;
            }
                case CNMAIL:
            {
                char *webmail_21cn_mail_pattern="/webmail/sendMail";
                char *webmail_21cn_att_pattern="/webmail/upload";
                if(strstr(StoPT->string,webmail_21cn_mail_pattern)){
                    if(cn_mail(StoMB,webmail)==-1)goto jdmail_att_fail;
                    webmail->state = build_jansson;
                }
                else if(strstr(StoPT->string,webmail_21cn_att_pattern)&& strstr(http_session->content_type, "boundary")){
                    if(cn_attach(StoBY,StoMB,webmail)==-1)goto jdmail_att_fail;
                    webmail->state = build_jansson;
                }
                break;
            }
                case MAIL139:
            {
                char *webmail_139_mail_pattern=".*func=mbox:compose";
                char *webmail_139_att_pattern1=".*func=attach:upload";
                char *webmail_139_att_pattern2="/new-fcgi-bin/";
                char *webmail_139_att_pattern3="/storageWeb/servlet/pcUploadFile";//网盘附件上传
                if(mch(&StoPT,NULL,webmail_139_mail_pattern)==EXIT_SUCCESS){
                    if(mail_139(StoCK,StoMB,webmail)==-1)goto jdmail_att_fail;
                    webmail->state = build_jansson;
                }
                else if(mch(&StoPT,NULL,webmail_139_att_pattern2)==EXIT_SUCCESS){
                    if(attach_139(StoBY,StoMB,webmail)==-1)goto jdmail_att_fail;
                }
                else if(mch(&StoPT,NULL,webmail_139_att_pattern1)==EXIT_SUCCESS && strstr(http_session->content_type, "boundary")){
                    if(attach2_139(dlp_http, StoPT, StoMB, StoBY, webmail)==-1)goto jdmail_att_fail;
                }
                else if(mch(&StoPT,NULL,webmail_139_att_pattern3)==EXIT_SUCCESS && http_session->contentSize){
                    if(attach3_139(dlp_http,StoMB,webmail)==-1)goto jdmail_att_fail;
                }
                break;
            }
                case MAIL189:
            {
                char *webmail_189_mail_pattern=".*/mail/sendMail";
                char *webmail_189_att_pattern1=".*/mail/upload";
                char *webmail_189_att_pattern2=".*WebUploadSmallFileAction";
                if(mch(&StoPT,NULL,webmail_189_mail_pattern)==EXIT_SUCCESS){
                    if(mail_189(StoMB,webmail)==-1)goto jdmail_att_fail;
                    webmail->state = build_jansson;
                }
                else if(mch(&StoPT,NULL,webmail_189_att_pattern1)==EXIT_SUCCESS||mch(&StoPT,NULL,webmail_189_att_pattern2)==EXIT_SUCCESS && strstr(http_session->content_type, "boundary")){
                    if(attach_189(StoPT,StoBY,StoMB,webmail)==-1)goto jdmail_att_fail;
                    webmail->state = build_jansson;
                }
                break;
            }
                case CNTVMAIL:
            {
                char *webmail_cntv_mail_pattern="/mail/mailOperate/";
                char *webmail_cntv_att_pattern="/FileUploadServlet";
                if(strstr(StoPT->string,webmail_cntv_mail_pattern)){
                    if(cntv_mail(StoMB,webmail)==-1)goto jdmail_att_fail;
                    webmail->state = build_jansson;
                }
                else if(strstr(StoPT->string,webmail_cntv_att_pattern) && strstr(http_session->content_type, "boundary")){
                    if(cntv_attach(StoCK,StoBY,StoMB,webmail)==-1)goto jdmail_att_fail;
                    webmail->state = build_jansson;
                }
                break;
            }
                default:webmail->state = no_jansson;
                        break;
            }
            if(webmail->state == build_jansson || webmail->state == mail_end){
                webmail_pro_jsonstr(dlp_http, webmail, webmailjson, sbuff_putdata, iringbuffer_ID);
            }

            /*资源释放*/
            if(StoPT){
                c_free(&StoPT->string);
                free(StoPT);
                StoPT = NULL;
            }
            if(StoMB){
                c_free(&StoMB->string);
                free(StoMB);
                StoMB = NULL;
            }
            if(StoBY){
                c_free(&StoBY->string);
                free(StoBY);
                StoBY = NULL;
            }
            if(StoCK){
                c_free(&StoCK->string);
                free(StoCK);
                StoCK = NULL;
            }
            //  log_printf(ZLOG_LEVEL_DEBUG, "jdmail_att end ....");
            return 0;
jdmail_att_fail:
            log_printf(ZLOG_LEVEL_ERROR, "jdmail_att fail end ....");
            if(pr){
                fclose(pr);
                pr = NULL;
            }
            if(StoPT){
                c_free(&StoPT->string);
                free(StoPT);
                StoPT = NULL;
            }
            if(StoMB){
                c_free(&StoMB->string);
                free(StoMB);
                StoMB = NULL;
            }
            if(StoBY){
                c_free(&StoBY->string);
                free(StoBY);
                StoBY = NULL;
            }
            if(StoCK){
                c_free(&StoCK->string);
                free(StoCK);
                StoCK = NULL;
            }
            return -1;
    }

    /**
     * @brief jdmail_type   判断是何种邮箱
     *
     * @param host_ori_ref  包含host、origin、referer信息
     *
     * @return
     */
    int jdmail_type(char*host_ori_ref)
    {
        log_printf(ZLOG_LEVEL_DEBUG, "jdmail_type start ....");
        char*wypat1="163.com";
        char*wypat2="126.com";
        char*wypat3="yeah.net";
        char*qqpat="qq.com";
        char*cnpcpat="cnpc";
        char*sinapat1="sina.com";
        char*sinapat2="sina.cn";
        char*sohu_sougopat1="sohu.com";
        char*sohu_sougopat2="sogou.com";
        char*sohu_sougopat3="chinaren.com";
        char*tompat="tom.com";
        char*cnpat="21cn.com";
        char*pat139="10086.cn";
        char*pat189="189.cn";
        char*cntvpat="cntv.cn";
        if(strstr(host_ori_ref,wypat1)||strstr(host_ori_ref,wypat2)||strstr(host_ori_ref,wypat3)){
            return WYMAIL;
        }
        else if(strstr(host_ori_ref,qqpat)){
            return QMAIL;
        }
        else if(strstr(host_ori_ref,cnpcpat)){
            return CNPCMAIL;
        }
        else if(strstr(host_ori_ref,sinapat1)||strstr(host_ori_ref,sinapat2)){
            return SINAMAIL;
        }
        else if(strstr(host_ori_ref,sohu_sougopat1)||strstr(host_ori_ref,sohu_sougopat2)||strstr(host_ori_ref,sohu_sougopat3)){
            return SOHUMAIL;
        }
        else if(strstr(host_ori_ref,tompat)){
            return TOMMAIL;
        }
        else if(strstr(host_ori_ref,cnpat)){
            return CNMAIL;
        }
        else if(strstr(host_ori_ref,pat139)){
            return MAIL139;
        }
        else if(strstr(host_ori_ref,pat189)){
            return MAIL189;
        }
        else if(strstr(host_ori_ref,cntvpat)){
            return CNTVMAIL;
        }
        //  log_printf(ZLOG_LEVEL_DEBUG, "jdmail_type end ....");
        return 0;
    }

    /**
     * @brief jdmail 判断是否为webmail
     *
     * @param host_ori_ref  host + origin + referer;
     *
     * @return
     */
    int jdmail(char* host_ori_ref)
    {
        if (strstr(host_ori_ref, "mail")){
            return EXIT_SUCCESS;
        }
        return EXIT_FAILURE;
    }

  
    /**
     * @brief webmail_          生成jansson串的函数
     *
     * @param dlp_http      经提取后的http头
     * @param webmail       存储邮件的相应信息
     * @param webmailjson   存放生成的jansson串
     * @param sbuff_putdata
     * @param iringbuffer_ID
     *
     * @return
     */
    int webmail_pro_jsonstr(HttpSession* dlp_http,dlp_webmail_info *webmail, char **webmailjson, http_sbuffputdata sbuff_putdata, const int iringbuffer_ID)
    {
        log_printf(ZLOG_LEVEL_DEBUG, " webmail_pro_jsonstr start ....");

        /*通过dlp_http 中的dlp_http_post_head类型的dlp_http来提取相应http的头信息*/
        dlp_http_post_head* http_session = NULL;
        http_session = (dlp_http_post_head *)dlp_http->http;

        /*初始化所需要的变量*/
        /*
         * fp           文件指针
         * nowtime      系统运行时间
         * result       存放jansson串的结果
         * tmp          存放临时节点
         * url          存放拼接后的完整url
         */
        FILE *fp = NULL;//lj
        char *nowtime = NULL;
        if(c_malloc((void**)&nowtime,256) == -1)  goto webmail_pro_jsonstr_fail;
        system_time(nowtime);
        json_t *objectmsg = NULL;
        json_t *objectall = NULL;
        json_t *myfile = NULL;
        json_t *filelist = NULL;
        json_t *tolist = NULL;
        char *result = NULL;
        struct att_node *tmp=webmail->filenode;
        SESSION_BUFFER_NODE ft;
        char url[HTTP_FILE_URL_MAX] = {};
        strcat(url, http_session->host);
        strcat(url, http_session->url);

        objectmsg = json_object();
        objectall = json_object();
        filelist= json_array();
        tolist= json_array();

        /*将源地址ip转换为标准格式，存入jansson串*/
        char * ipaddr1=NULL;
        char addr1[20]= {};
        struct in_addr inaddr1;
        inaddr1.s_addr=dlp_http->key.saddr;
        ipaddr1 = inet_ntoa(inaddr1);
        strcpy(addr1,ipaddr1);
        json_object_set_new (objectall, "sip", json_string(addr1));

        /*将目的地址ip转换为标准格式，存入jansson串*/
        char * ipaddr2=NULL;
        char addr2[20]={};
        struct in_addr inaddr2;
        inaddr2.s_addr=dlp_http->key.daddr;
        ipaddr2 = inet_ntoa(inaddr2);
        strcpy(addr2,ipaddr2);
        json_object_set_new (objectall, "dip", json_string(addr2));

        /*如果不存在收件人则协议号改为7*/
        if(strlen(webmail->To)==0)
            webmail->pro_id=7;
        json_object_set_new (objectall, "prt", json_integer(webmail->pro_id));
        json_object_set_new (objectall, "spt", json_integer(dlp_http->key.source));
        json_object_set_new (objectall, "dpt", json_integer(dlp_http->key.dest));
        json_object_set_new (objectall, "ts", json_string(nowtime));
        json_object_set_new (objectmsg, "url", json_string(url));
        json_object_set_new (objectmsg, "ts", json_string(webmail->Time));

        /*生成发件人，收件人，主题等信息*/
        if(webmail->From)
            json_object_set_new (objectmsg, "from", json_string(webmail->From));
        else
            json_object_set_new (objectmsg, "from", json_string(""));

        if(strlen(webmail->To)!=0){
            to_json(tolist , webmail->To);
        }
        json_object_set_new (objectmsg, "to", tolist);

        if(webmail->Subject){
            json_object_set_new (objectmsg, "sbj", json_string(webmail->Subject));
        }
        else{
            json_object_set_new (objectmsg, "sbj", json_string(""));
        }

        /*创建正文存储的路径*/
        char foldername[FILE_NAME_LENGTH]={};
#if 0 //当前路径
        getcwd(foldername, FILE_NAME_LENGTH);
        strcat(foldername,"/");
#endif
        sprintf(foldername , "%s%s", session_stream , "/" );
        char uuid_buf[UUID_LENGTH] = {};

        /*正文存储，并相应生成jansson串*/
        if(webmail->Content){
            /*创建正文的存储名，并存储到当前目录*/
            char filename[FILE_NAME_LENGTH]={};/*存放路径+文件名*/
            uuid_t randNumber;
            uuid_generate(randNumber);
            uuid_unparse(randNumber, uuid_buf);
            strcpy(filename,foldername);
            strcat(filename,uuid_buf);
            strcat(filename,".txt");
            json_object_set_new (objectmsg, "plain", json_string(filename));
            /*存储正文到当前目录*/
            fp = fopen(filename , "w+");
            if(fp == NULL){
                log_printf(ZLOG_LEVEL_ERROR, " the pro_jansson cant open the file!");
                goto webmail_pro_jsonstr_fail;
            }
            fwrite( webmail->Content , strlen(webmail->Content) , 1 , fp);
            fclose(fp);
            fp=NULL;
        }
        /*如果没有正文则输出为空*/
        else
            json_object_set_new (objectmsg, "plain", json_string(""));

        json_object_set_new (objectall, "pAttr", objectmsg);

        /*生成附件相关jansson串*/
        json_t *copy = NULL;
        /*从附件链表中循环取出一个或多个附件*/
        while(tmp->next!=NULL){
            myfile = json_object();
            tmp=tmp->next;
            json_object_set_new (myfile, "ft", json_string(""));
         if(tmp->RawAttName==NULL) json_object_set_new (myfile, "fn", json_string("file_no_name"));//0115
            else json_object_set_new (myfile, "fn", json_string(tmp->RawAttName));
            json_object_set_new (myfile, "fs", json_integer(tmp->attlen/1024));
            json_object_set_new (myfile, "au", json_string(""));
            char uuid_buf1[UUID_LENGTH]={};
            char filename2[FILE_NAME_LENGTH]={};/*放置路径+文件名*/
            if(webmail->state == build_jansson){
                memcpy(uuid_buf1,tmp->att_uuid,UUID_LENGTH);
            }
            if(webmail->state == mail_end){
                memcpy(uuid_buf1,webmail->uuid,UUID_LENGTH);
            }
            strcpy(filename2,foldername);
            strcat(filename2,uuid_buf1);
            json_object_set_new (myfile, "fp", json_string(filename2));
            json_object_set_new (myfile, "ftp", json_string(""));
            copy = json_deep_copy(myfile);
            json_decref(myfile);
            myfile=NULL;
            json_array_append_new(filelist, copy);
        }
        json_object_set_new (objectall, "flist", filelist);

        /* 转换成字符串 */
        result = json_dumps(objectall, JSON_PRESERVE_ORDER);
        if(c_malloc((void**)webmailjson,strlen(result)+1) ==-1) goto webmail_pro_jsonstr_fail;
        memcpy(*webmailjson, result,strlen(result));

        /* 释放json_dumps调用后的结果 */
        c_free(&result);

        /*释放对应资源*/
        json_decref(objectall);
        c_free(&nowtime);
        if((&(**webmailjson)) != NULL)
        {
            ft.session_five_attr.ip_dst = dlp_http->key.daddr;
            ft.session_five_attr.ip_src = dlp_http->key.saddr;
            ft.session_five_attr.port_src = dlp_http->key.source;
            ft.session_five_attr.port_dst = dlp_http->key.dest;
            ft.attr = &(**webmailjson);
            ft.session_five_attr.protocol = webmail->pro_id;
            sbuff_putdata(iringbuffer_ID, ft);
        }
        if(fp){
            fclose(fp);
            fp =NULL;
        }
        return 0;
webmail_pro_jsonstr_fail:
        json_decref(objectall);
        c_free(&nowtime);
        c_free(&result);
        if(fp){
            fclose(fp);
            fp =NULL;
        }
        log_printf(ZLOG_LEVEL_ERROR, " webmail_pro_jsonstr fail end ....");
        return -1;

    }
    /**
     * @brief webmail_free_HttpSession  free struct of "HttpSession"
     *
     * @param dlp_http
     *
     * @return
     */
    int webmail_free_HttpSession (HttpSession* dlp_http){
       if(dlp_http ==NULL)return -1;
        dlp_http_post_head *http_session=NULL;
        http_session = (dlp_http_post_head*)dlp_http->http;
        if (http_session == NULL)
        {
            log_printf(ZLOG_LEVEL_ERROR, "http_session is null, %s__%d", __FILE__, __LINE__);
            return -1;
        }
        if (http_session->cookie)
        {
            c_free(&(http_session->cookie));
        }

        if(http_session != NULL){
            free(http_session);
            http_session = NULL;
        }
        return 0;
    }

    /**
     * @brief webmail_mail  the entrance of the  main fuction and initiate the
     * related data ;
     *
     * @param dlp_http
     * @param webmailjson
     * @param sbuff_putdata
     * @param iringbuffer_ID
     *
     * @return
     */
    int webmail_mail(HttpSession* dlp_http,char **webmailjson, http_sbuffputdata sbuff_putdata, const int iringbuffer_ID)
    {
        //  log_printf(ZLOG_LEVEL_DEBUG, " webmail_mail start ....");
        /*通过dlp_http 中的dlp_http_post_head类型的dlp_http来提取相应http的头信息*/
        dlp_http_post_head *http_session = NULL;
        http_session = (dlp_http_post_head* )dlp_http->http;
        webmail_string_and_len *StoMB = init_webmail_string_and_len();
        if(StoMB == NULL)goto webmail_mail_fail;//lj
        FILE *pr=NULL;
        /*初始化存放解析后的邮件信息*/
        dlp_webmail_info *webmail = NULL;
        if(c_malloc((void **)&webmail,sizeof(dlp_webmail_info)) ==-1) goto webmail_mail_fail;//lj
        webmail->filenode = init_att_nodelist();
        memset(webmail->Time, '\0', sizeof(webmail->Time));
        memset(webmail->boundary,'\0',sizeof (webmail->boundary));
        memset(webmail->uuid,'\0', sizeof(webmail->uuid));
        webmail->From=NULL;
        webmail->Subject=NULL;
        webmail->Content=NULL;
        webmail->state = no_jansson;
        if(c_malloc((void**)&(webmail->To),MAX_TO_LEN) == -1 ) goto webmail_mail_fail;//lj

        /*存放host、origin、regerer信息，便于之后判断时否为webmail以及时何种邮箱流*/
        char host_ori_ref[HTTP_FILE_HOST_MAX+HTTP_FILE_ORIGIN_MAX+HTTP_FILE_REFERER_MAX] = {0};//存放提取前五个post的host、origine、referer放到一个指针里
        if(http_session == NULL){
            log_printf(ZLOG_LEVEL_ERROR, "%s__%d\n", __FILE__, __LINE__);
            goto webmail_mail_fail;
        }
        strcat(host_ori_ref, http_session->host);
        strcat(host_ori_ref, http_session->origin);
        strcat(host_ori_ref, http_session->referer);

        /*判断是否为webmail，如果是在判断是何种邮箱发出的何种流（附件或者单纯的邮件）*/
        if(jdmail(host_ori_ref)==EXIT_SUCCESS){
            if(http_session->content_length != 0){
                webmail->pro_id=157;
                if(jdmail_att(dlp_http, webmail, host_ori_ref, webmailjson, sbuff_putdata, iringbuffer_ID)==-1){
                    log_printf(ZLOG_LEVEL_ERROR, "%s__%d\n", __FILE__, __LINE__);
                    goto webmail_mail_fail;
                }
            }

        }
        /*如果均不是，则判断是否为MIME格式的http*/
        else if(strstr(http_session->content_type,"boundary")){
            char new_name[FILE_NAME_LENGTH];
            strcpy(new_name,http_session->new_name);
            //printf("the new name is %s\n",new_name);
            FILE *pr = fopen(new_name,"r");
            if(pr == NULL){
                log_printf(ZLOG_LEVEL_ERROR, "When webmail_mail is processed,the open file is failed!!\n\n");
                goto webmail_mail_fail;
            }
            //fseek(pr,0,SEEK_END);
            StoMB->len = http_session->content_length;
            if(c_malloc((void**)&StoMB->string,StoMB->len+1) == -1){
                goto webmail_mail_fail;//lj
            }
            fseek(pr,0,SEEK_SET);
            fread(StoMB->string,1,StoMB->len,pr);
            fclose(pr);
            pr=NULL;
            webmail->pro_id=7;
            memcpy(webmail->boundary,http_session->boundary,MAX_BOUNDARY_LEN);
            /*提取MIME格式传输的附件*/
            mch_att(&StoMB,webmail);
            if((webmail->filenode)->next!=NULL){
                webmail->state = build_jansson;
                webmail_pro_jsonstr(dlp_http,webmail, webmailjson,sbuff_putdata, iringbuffer_ID);
            }
        }
        else
        {
#if 1
            http_IM(dlp_http, webmailjson, sbuff_putdata, iringbuffer_ID);
            goto webmail_mail_fail;//lj
#endif
        }
        if(StoMB){
            c_free(&StoMB->string);
            free(StoMB);
            StoMB = NULL;
        }
        if(webmail != NULL){
            free_att_nodelist (webmail->filenode);
            c_free(&webmail->Subject);
            c_free(&webmail->From);
            c_free(&webmail->To);
            c_free(&webmail->Content);
            free(webmail);
            webmail = NULL;
        }
        webmail_free_HttpSession(dlp_http);
        //  log_printf(ZLOG_LEVEL_DEBUG, " webmail_mail end ....");

        return 0;
webmail_mail_fail:
        if(StoMB){
            c_free(&StoMB->string);
            free(StoMB);
            StoMB = NULL;
        }
        if(webmail){
            free_att_nodelist (webmail->filenode);
            c_free(&webmail->Subject);
            c_free(&webmail->From);
            c_free(&webmail->To);
            c_free(&webmail->Content);
            free(webmail);
            webmail = NULL;
        }
        if(pr){
            fclose(pr);
            pr =NULL;
        }
        webmail_free_HttpSession(dlp_http);
        //log_printf(ZLOG_LEVEL_ERROR, " webmail_mail fail end ....");
        return -1;
    }
    


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
