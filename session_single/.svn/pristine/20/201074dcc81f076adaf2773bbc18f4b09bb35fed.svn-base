
//this version can extract the post request line and the header.
//based on the ver 4.0, this  can get  all POST message.
//this ver 5.5 should  exract not only the POST RL and Header but also the  message body.
#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<pcre.h> 
#define OVECCOUNT 30    /* should be a multiple of 3 */
#define EBUFLEN 128
#define BUFLEN 1024 
static	char 		StoPT[100][40000] = {};
static	char		StoMB[100][40000] = {};
static	int		StoCL[100]={0};	
int mch(char var[20000],char * pattern)
{
        pcre 		*re1;
        const char 	*error;
        int 		erroffset;
        int 		ovector[OVECCOUNT];
        int 		rc, i;
	int 		Rloca=0;	/*to record the offset location each match*/
	int 		f=0;		//flag for record the 
static	char 		StoPT[100][40000] = {};
static	char		StoMB[100][40000] = {};
static	int		StoCL[100]={0};
	int		len=strlen(var);		
        re1 = pcre_compile(pattern, 0, &error, &erroffset, NULL);
        if (re1== NULL) 
	{
                printf("PCRE compilation failed at offset %d: %s\n", erroffset, error);
                return -1;
        }
/*****************************************************************************************************/
/****************************************Extract POST RL HDR******************************************/
	do 
	{
		/*clean ovector */
		memset(ovector,0,OVECCOUNT*sizeof(int));	
		rc=0;
		rc = pcre_exec(re1, NULL, var, len,Rloca, 0, ovector, OVECCOUNT);
		if (rc < 0) 
		{
		        if (rc == PCRE_ERROR_NOMATCH) printf("Sorry, no more match ...\n");
		        else    printf("Matching error %d\n", rc);
                        re1 = NULL;
		        return -1;
		}
		else
		{
			Rloca=ovector[1];
			char *substring_start = var + ovector[0];
		        int substring_length = ovector[1] - ovector[0];
			sprintf(StoPT[f],"%.*s",substring_length, substring_start); //just first string
			StoCL[f]=Post_Con_Len(StoPT[f],f);
			sprintf(StoMB[f],"%.*s",StoCL[f],var+ovector[1]+4);
			printf("StoMB[%d]=%s  \n ",f,StoMB[f]);
			f++;
		}
	}while(rc>0);
        return 0;
}
/************************************************************************************************/
/************************************************************************************************/
int Post_Con_Len(char *StoPT ,int f)
{
	char 		pattern[100] = "(?<=Content-Length:|Content-Length: )\\d*?(?=\\r\\n)";
        pcre 		*re2;
        const char 	*error;
        int 		erroffset;
        int 		ovector[OVECCOUNT];
        int 		rc, i;
	int		x;
	char		tem[30];
	int		len=strlen(StoPT);
	re2	= 	pcre_compile(pattern, 0, &error, &erroffset, NULL); 
	if 		(re2 == NULL) 
	{
                printf("PCRE compilation failed at offset %d: %s\n", erroffset, error);
                return -1;
        }
	memset(ovector,0,OVECCOUNT*sizeof(int));	
	rc=0;
	rc = pcre_exec(re2, NULL, StoPT, len,0, 0, ovector, OVECCOUNT);
	if (rc < 0) 
	{
	        if (rc == PCRE_ERROR_NOMATCH)
		{
			printf("Sorry, no more match ...\n");
			return	-1;
		}
	        else
		{
	    		printf("Matching error %d\n", rc);
	        	return	-1;
		}
	}
        char *substring_start = StoPT+ ovector[0];
        int substring_length = ovector[1] - ovector[0];
	sprintf(tem,"%.*s",substring_length, substring_start); //just first string
	x=atoi(tem);
	printf("%d\n",x);
	return(x);
}

/************************************************************************************************/
/************************************************************************************************/


/************************************************************************************************/
/************************************************************************************************/
int main(int argc,char** argv) 
{  	
	FILE *fpr;
	char buf[10000]={};
static	char var[200000]={};   
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
	printf("size=%d\n", strlen(var));
	system("clear");
//	printf("%s",var);
	char		*pat1;
	pat1	=	(char*)malloc(200);	
	pat1="POST[\\w\\W]*?(?=\\r\\n\\r\\n)";	
    	mch(var,pat1); 
	fclose(fpr);	
	return (0);
} 

		 

