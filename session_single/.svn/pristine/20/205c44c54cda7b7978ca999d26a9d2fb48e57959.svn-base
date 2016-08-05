#ifndef HTTP_EXTRACT_H_
#define HTTP_EXTRACT_H_
#define COPY_STRING(dst, src, src_len) do{memcpy((dst), (src), (src_len)); dst[(src_len)]='\0';}while(0);

#include <assert.h>
#include <stddef.h>
#include <ctype.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include <stdio.h>
#include <time.h>
#include <stdarg.h>
#include <unistd.h>
#include <pcre.h>
#include <jansson.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/time.h>
#include "http_parser.h"

/* callback function */
typedef int (http_sbuffputdata) (int iringbuffer_ID, SESSION_BUFFER_NODE ft);

/***************************声明********************************************/
typedef struct {
	int used_flag;		/* 1:using, 0:not using */
	char url[2048];
	char host[256];
	char content_type[1024];
	char boundary[100];
	char up_load_name[1024];
	char up_load_modtime[16];
	char the_file_name[1024];	/* 272 = 256+16 */
	unsigned int up_load_size;
} dlp_http_file_node;

typedef struct DLP_HTTP_PARSER_NODE {
//      int   ID_OF_THE_STRING;
//      int   STRING_LENGTH;
//      char  *FILE_NAME;
//      char  *STRING;
	int id;			//0:Sto 1:文件名和文件 2：只有文件名
	int l;
	char *name;
	char *str;
	struct DLP_HTTP_PARSER_NODE *next;
} dlp_http_parser_node;

struct DLP_HTTP_PARSER {
	char dir[256];
	char url[2048];
	char content_type[100];
	char request_type[10];
	int file_length;
	char boundary[100];
	char filename[256];
	char *cookie;
	dlp_http_parser_node *node_file;
	dlp_http_parser_node *node_Sto;
} testhttp;

#ifndef testhttp_string_and_len
typedef struct DLP_HTTP_PARSER_STRING_AND_STRLEN {
	int len;
	char *string;
} testhttp_string_and_len;
#endif
/*****************************file function*********************************/
int mch_file(testhttp_string_and_len ** var, dlp_http_parser_node * node_file);
/*****************************初始化string_and_len***************************/
testhttp_string_and_len *init_testhttp_string_and_len();

/*************************获取一个16位的时间************************************/

int SYSTEM_TIME(char *all);

 /*************************正则匹配函数************************************/

int mch(testhttp_string_and_len ** var, testhttp_string_and_len ** Sto, const char pattern[200]);

int mch_file(testhttp_string_and_len ** var, dlp_http_parser_node * node);

int mch_boundary(testhttp_string_and_len * var, testhttp_string_and_len ** Sto, int Rloca, const char pattern[200]);

// int mch_Sto(testhttp_string_and_len * var, dlp_http_parser_node * node_Sto);

// int mch_Sto1(testhttp_string_and_len * var, dlp_http_parser_node * node_Sto);

// int Post_Con_Len(testhttp_string_and_len * StoPT);

 /*************************************链表*********************************/

dlp_http_parser_node *init_nodelist();

int add_nodelist(dlp_http_parser_node * nodelist, int id, int l, char *name, char *str);

int free_nodelist(dlp_http_parser_node * nodelist);

int show_nodelist(dlp_http_parser_node * nodelist);

/************设置http_paser的callback函数********************************/
int on_message_begin(http_parser * _);

int on_headers_complete(http_parser * _);

int on_message_complete(http_parser * _);

int on_url(http_parser * _, const char *at, size_t length);

int on_header_field(http_parser * _, const char *at, size_t length);

int on_header_value(http_parser * _, const char *at, size_t length);

int on_body(http_parser * _, const char *at, size_t length);

/*************http_paser*************************************/
/*功能：将data中的http进行解析
**参数：
**data：带解析字符串
**file_length：data长度
*/
int http_paser(testhttp_string_and_len ** Sto, http_parser * parser);

int http(char *file_path, hash_bucket * pBucket);
/* 获得文件大小 */
//unsigned long get_file_size(const char *path);

#endif
