#include<stdio.h>
#include<sys/types.h>
#include<regex.h>
#include<memory.h>
#include<string.h>
#include<stdlib.h>
int  smtpmatch(char *ch1,char *ch2)
{
	char bematch[2048];
	char pattern[128];
    	char errbuf[2048];
	char match[100];
	int err,nm = 10;
	int flag;
	regex_t reg;
	regmatch_t pmatch[nm];

	strcpy(bematch,ch1);
	strcpy(pattern,ch2);
	if(regcomp(&reg,pattern,REG_EXTENDED)<0)
	{
		regerror(err,&reg,errbuf,sizeof(errbuf));
		printf("err:%s\n",errbuf);
		flag = 0;
	}
	err =regexec(&reg,bematch,nm,pmatch,0);
	if(err == REG_NOMATCH)
	{
//		printf("buf is no matched\n");
		flag = 0;
//		exit(-1);
	}
	else if(err)
	{
		regerror(err,&reg,errbuf,sizeof(errbuf));
		printf("err:%s\n",errbuf);
		flag = 0;
//		exit(-1);
	}
	if (err == 0)
	{
//	 printf("be matched\n");
	    flag = 1;
	} 
	regfree(&reg);
	return flag;
}

int main()

{

    	FILE *fpr;
	int flag_1 , flag_2,i=0;
	char buf_1[2048];
//	char buf_2[2048];
	char *regexp_1 = "^220.*USER.*";
	char *regexp_2 = "^331.*PASS.*";
	char *file = "liu.txt";

	if((fpr=fopen(file,"r"))==NULL)
	    printf("cant't open this file!\n");
	else
	fseek(fpr,0,SEEK_SET);

	while((!feof(fpr))&&i<2048)
       {buf_1[i]=fgetc(fpr);
         i++;}

//fgets(buf_2,128,fpr);
//	printf("%s\n",buf_1);
//	printf("%s\n",buf_2);
  	flag_1 = smtpmatch(buf_1,regexp_1);
//	flag_2 = smtpmatch(buf_1,regexp_2);
//	printf("%d\n",flag_1);
//	printf("%d\n",flag_2);
//	if(flag_1 == 1 && flag_2 == 1)
//	    printf("ftp is matched\n");
//	else
//	    printf("ftp is no matched\n");
	if(flag_1 == 1)
	printf("ftp is matched, it is kongzhi liu \n");
        else
	    printf("ftp is not  matched\n");
	fclose(fpr);
	return(0);		
}

