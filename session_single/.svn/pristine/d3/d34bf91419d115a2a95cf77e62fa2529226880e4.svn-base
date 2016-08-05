#ifndef __BEAP_HASH
#define __BEAP_HASH

#define MAX_HASH_SIZE	100001
#define UUID_MAX_LENGTH	512
typedef signed char    SCHAR8;



typedef unsigned int(* t_hash_func)(void *, int) ;
typedef char(* t_comp_func)(void *, void *, int) ;

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
int 		nr_entries; //?????
int		tbl_size;//???С
t_hash_func	hash_func;
t_comp_func	comp_func;
}hash_table;

hash_table * create_hash(int , t_hash_func , t_comp_func );
void delete_hash(hash_table *);
void delete_hash_keep_content(hash_table *);
void insert_hash(hash_table * , hash_bucket * ,int);
void remove_hash(hash_table * , hash_bucket * ,int);
hash_bucket * find_hash(hash_table * , void* ,int);


///////////////////////////////////////////////////////////////
#define LS_MEM_MALLOC(Ptr,Type)      Ptr =  (Type *)malloc(sizeof(Type))




unsigned int session_hash(void *key, int);
unsigned char session_comp(void *key1, void *key2, int);

typedef struct
{
   unsigned  char   flag;
   struct tuple4    key;
   unsigned char    file_name[128];
   unsigned char    store_name[220];
}FtpdataSession;


typedef struct
{
	unsigned char flag;
	unsigned char flag2;
	struct tuple4 key;
    unsigned char file_name[128];
    unsigned char store_name[220];
	uint32_t      port;
	uint32_t      ip;
}FtpSession;

typedef struct {
    struct  tuple5  key;
    char    uuid[UUID_MAX_LENGTH];
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
/*********************************/
struct http_IM_tuple5
{
    unsigned short IM_src;
    unsigned short IM_dst;
    unsigned int   IM_sad;
    unsigned int   IM_dad;
    char           post_id[256];
};

typedef struct _http_IM_session
{
    struct http_IM_tuple5   key;
    unsigned long           fs; 
    char                    uid[70];
    char                    url[1024];
    char                    ts[64];
    char                    fn[256];
    unsigned long           range;                          
    unsigned short          block;                          
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

/////////////SMTP///////////SMTP/////////SMTP//////////////SMTP////////////////////////

#if 1 
/*存放email数据包的链表*/
typedef struct SMTP_EML
{
	char			*p_eml;            //邮件数据包
	unsigned short  len;
	struct SMTP_EML	*next;
}SMTP_EML;

typedef struct EML_TO
{
    char    *p_to;
	unsigned short  len;
	struct  EML_TO  *next;
}EML_TO;
/**********************************************************************************/
typedef struct
{
	EML_TO          *to_list;		  //存放email 的链表
    char            uid[70];
	unsigned short  chunk_flag;		  //计算发件人所在数据包的大小
	unsigned long   eml_to_len;		  //计算发件人所在数据包的大小
	unsigned short  find_chunk;		  //用来标记邮件部分的状态
	unsigned long   Find_RCPT;		  //用来标记邮件部分的状态
	unsigned long   linelen;		  //用来标记邮件部分的状态
	unsigned long   Find_MAil;		  //用来标记邮件部分的状态
	unsigned long   BDAT_len;		  //用来标记邮件部分的状态
	unsigned long   BDAT;		  //用来标记邮件部分的状态
	unsigned long   DATA;		  //用来标记邮件部分的状态
	unsigned short  eml_flag;		  //用来标记邮件部分的状态
	unsigned short  seen_eml;		  //用来标记邮件部分的状态
	unsigned short  crlf_seen;		  //
	unsigned short  smtp_state;		  //标记处理数据包的状态
	unsigned short  STARTTLS;		  //标记邮件是否加密的中间状态
	unsigned short  flag;			  //是否读桶数据的标记，为1时，需要读桶
	char            eml_from[64];		  //存放发件人
	char            *smtp_json;		  //包含EML属性的JSON串 ，需要提取
	struct tuple4	key;
}SmtpSession;
/////////////////////////////////////////////////////////////////////////////////////////////////
#endif

#endif
// __BEAP_HASH
