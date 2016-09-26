#ifndef __BEAP_HASH
#define __BEAP_HASH

#define MAX_HASH_SIZE	16384
#define UUID_MAX_LENGTH 512
typedef signed char    SCHAR8;
typedef unsigned char      UCHAR8;
typedef char               CHAR8;
#include<stdio.h>
#include<string.h>

typedef unsigned int(* t_hash_func)(void *,int) ;
typedef char(* t_comp_func)(void *, void *,int) ;

struct stru_hash_bucket;

typedef struct{
struct stru_hash_bucket * prev_hash;
struct stru_hash_bucket * next_hash;
}hash_ptr;

typedef struct stru_hash_bucket{
void *		key;
void *		content;
hash_ptr	ptr;
}hash_bucket;

typedef struct{
hash_bucket  	**hashtable;
int 		nr_entries; //记录数
int		tbl_size;//表大小
t_hash_func	hash_func;
t_comp_func	comp_func;
}hash_table;

hash_table * create_hash(int , t_hash_func , t_comp_func );
void delete_hash(hash_table *);
void delete_hash_keep_content(hash_table *);
void insert_hash(hash_table * , hash_bucket * ,int a);
void remove_hash(hash_table * , hash_bucket *,int a);
hash_bucket * find_hash(hash_table * , void*,int a );


///////////////////////////////////////////////////////////////
#define LS_MEM_MALLOC(Ptr,Type)      Ptr =  (Type *)malloc(sizeof(Type))

#define LS_MEM_FREE(p)\
    if(p){\
        free(p);\
        p = NULL;\
    }
	
unsigned int session_hash(void *key,int len);
SCHAR8 session_comp(void *key1, void *key2,int len);
typedef struct
{
    unsigned long sip;
    unsigned long dip;
    unsigned short sport;
    unsigned short dport;
}
sessionID;

struct tuple4
{
  unsigned short source;
   unsigned short dest;
   unsigned int saddr;
   unsigned int daddr;
};

struct tuple5
{
  unsigned short source;
   unsigned short dest;
   unsigned int saddr;
   unsigned int daddr;
   unsigned int pro;
};

typedef struct {
    struct tuple5  key;
    int                  flag;
    char    uuid[UUID_MAX_LENGTH];
    char*   content;
}WebmailSession;

typedef struct https
{
     struct tuple4  key;
     int                  flag;
     int                  len;
     FILE*             old_file;
     char               old_name[128];
     char                uuid[512];
     void             *http;
}
HttpSession;



/*定义五元组，在飞信离线文件传输时生成哈希地址*/
struct http_IM_tuple5
{
	unsigned short IM_src;	//源端口
	unsigned short IM_dst;	//目的端口
	unsigned int   IM_sad;	//源IP
	unsigned int   IM_dad;	//目的IP
	char           post_id[128];	//自定义的字符串
};
/*飞信离线文件传输时的hash桶内容*/
typedef struct _http_IM_session
{
	unsigned short			block;	//传输文件时的分块数量
	unsigned long			range;	//Message Body中文件数据在所传输文件的位置偏移量
	unsigned long			fs;		//文件大小
	char					ts[64];	//文件开始传输的时间
	char					uid[70];//存放原始文件的UUID
	struct http_IM_tuple5	key;	//五元组
	char					fn[256];//原始文件名
//	char					url[1024];
}http_IM_session;
struct http_feition_tuple2
{
    char s[64];
    char host[256];
};

typedef struct _http_feition_session
{
    struct http_feition_tuple2      key;
//  char                            url[1024];
    char                            fn[256];
    char                            uid[70];
    char                            ts[64];
    unsigned long                   fs;
}http_feition_session;

#endif
// __BEAP_HASH
