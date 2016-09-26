//this version can extract the post request line and the header.
//based on the ver 4.0, this can all POST message.
#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<pcre.h> 
#define OVECCOUNT 30    /* should be a multiple of 3 */
#define EBUFLEN 128
#define BUFLEN 1024 
#define NON_NUM '0'
int mch(char var[200000],char*pattern)
{
        pcre *re;
        const char *error;
        int erroffset;
        int ovector[OVECCOUNT];
        int rc, i=0;
	int c=0;	/*to record the offset location each match*/
	//int flag=1;
	char *buf;
	buf=(char *)malloc(200000);
	char *src;
	src=(char *)malloc(200000);
	strcpy(src,var);
	int		len=strlen(src);	
       // char *pattern="POST[\\w\\W]*?(?=\\r\\n\\r\\n)";
			
//        printf("String : %s\n", src);
//        printf("Pattern: %s\n", pattern);
        re = pcre_compile(pattern, 0, &error, &erroffset, NULL);
        if (re == NULL) {
                printf("PCRE compilation failed at offset %d: %s\n", erroffset, error);
                return 1;
        }
/*********************************************************************************************/
	do 
	{
		/*clean ovector */
		memset(ovector,0,OVECCOUNT*sizeof(int));	
		rc=0;
		rc = pcre_exec(re, NULL, src, len,c, 0, ovector, OVECCOUNT);
		if (rc < 0) {
		        if (rc != PCRE_ERROR_NOMATCH) printf("Sorry, no more match ...\n");
		        //else    
			//printf("Matching error %d\n", rc);
		        return 1;
		}
//		printf("\nOK, has matched ...\n\n");
	       for (i = 0; i < rc; i++)
	       {
		        char *substring_start = src + ovector[2*i];
		        int substring_length = ovector[2*i+1] - ovector[2*i];
		       printf("\n%.*s\n", substring_length, substring_start);
			//flag++;
		}
		c=ovector[((rc-1)*2+1)];
	}while(rc>0);	
        free(re);
        return 0;
}
int hex2num(char c)
{
    if (c>='0' && c<='9') return c - '0';
    if (c>='a' && c<='z') return c - 'a' + 10;//ÕâÀï+10µÄÔ­ÒòÊÇ:±ÈÈç16œøÖÆµÄaÖµÎª10
    if (c>='A' && c<='Z') return c - 'A' + 10;
    
    //printf("unexpected char: %c", c);
    return NON_NUM;
}

/**
 * @brief URLDecode 
 *
 * @param str Ô­×Ö·ûŽ®
 * @param strSize Ô­×Ö·ûŽ®ŽóÐ¡£š²»°üÀš×îºóµÄ\0£©
 * @param result œá¹û×Ö·ûŽ®»ºŽæÇø
 * @param resultSize œá¹ûµØÖ·µÄ»º³åÇøŽóÐ¡(°üÀš×îºóµÄ\0)
 *
 * @return: >0 result ÀïÊµŒÊÓÐÐ§µÄ×Ö·ûŽ®³€¶È
 *            0 œâÂëÊ§°Ü
 */
int URLDecode(const char* str, const int strSize, char* result, const int resultSize)
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
                if (i+2<strSize) {
                    ch1 = hex2num(str[i+1]);//žß4Î»
                    ch2 = hex2num(str[i+2]);//µÍ4Î»
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
/******************163mail***********************************/
void wymail(char var[200000])
{
//	printf("%s","this is a 163mail without attach");			
	printf("%s","from:");
	char *pattern1="(?<=account\">\")[\\w\\W]*?(?=&gt)";
	mch(var,pattern1); 
	printf("%s","to:");
	char *pattern2="(?<=to\"><string>)[\\w\\W]*?(?=</string>)";
	mch(var,pattern2);
	printf("%s","Cc:");
	char *pattern3="(?<=\"cc\"><string>)[\\w\\W]*?(?=</string>)";
	mch(var,pattern3);
	printf("%s","Bcc:");
	char *pattern4="(?<=bcc\"><string>)[\\w\\W]*?(?=</string>)";
	mch(var,pattern4);
	printf("%s","subject:");
	char *pattern5="(?<=subject\">)[\\w\\W]*?(?=</string>)";
	mch(var,pattern5);
	printf("%s","content:");
	//char *pattern6="(<=content\">)*(?<=&gt;)[\\w\\W]*?(?=&lt)";
	//mch(obj,pattern6);
	printf("%s","Time:");
	char *pattern7="Date+[\\w\\W]*?\\n";
	mch(var,pattern7);
}
/********************163attach***********************************/
void wyattach(char var[20000])
{
//	printf("%s","this is a 163mail with attach!\n");
	printf("%s","attachname:");
	char *pattern1="(?<=Mail-Upload-name:)[\\w\\W]*?(?<=\\r\\n)";
	mch(var,pattern1);
	printf("%s","attachsize:");
	char *pattern2="(?<=Mail-Upload-size:)[\\w\\W]*?(?<=\\r\\n)";
	mch(var,pattern2);
	printf("%s","user_mail:");
	char *pattern3="(?<=uid=)[\\w\\W]*?(?=&cid)";
	mch(var,pattern3);
	printf("%s","Time:");
	char *pattern4="Date+[\\w\\W]*?\\n";
	mch(var,pattern4);
}
	

int main(int argc,char** argv) 
{  	
	FILE *fpr;
	char buf[1000]={};
	char var[100000]={};   
	if((fpr=fopen(argv[1],"r"))==NULL)  
	{  
		printf("open file  failed.\n");  
		return ;  
	}
	else
	{
		fseek(fpr,0,SEEK_SET);
		while((fgets(buf,1000,fpr))!=NULL)		
		{		
			strcat(var,buf);
		}		
	}
	/*system("clear");
//	printf("%s",var);
	char *s;
	s=(char *)malloc(100000);
	strcpy(s,var);
	char*match1="Host: twebmail.mail.163.com";
	char*match2="Host: webmail.mail.163.com";*/
	//char*match="Host:.*?/.mail/..*?.(com|cn|net)";
	char obj[10000] = {0};
    	unsigned int len = strlen(var);
   	int resultSize = URLDecode(var, len, obj, 100000); 
	char*match1="&func=mbox:compose";
	if(strstr(obj,match1)) 
	{
		wymail(obj);
	}
	char*match2="POST /upxmail/upload";
	if(strstr(obj,match2)) 
	{	
		wyattach(obj);
	}
		
	else
	{printf("It is not a webmail");}
	fclose(fpr);	
	return (0);

} 

		 

