#ifndef __BEAP_HASH
#define __BEAP_HASH

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#define MAX_HASH_SIZE	100001
//typedef signed char    SCHAR8;

typedef unsigned int uint32_t;
typedef unsigned long  uint64_t; 


typedef uint32_t(* t_hash_func)(void *, int) ;
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
    hash_bucket **hashtable;
    int 		nr_entries; //记录数
    int		    tbl_size;//表大小
    t_hash_func	hash_func;
    t_comp_func	comp_func;
}hash_table;

//hash_table  *ftp_session_tbl=NULL;
hash_table * create_hash(int , t_hash_func , t_comp_func );
void delete_hash(hash_table *);
void delete_hash_keep_content(hash_table *);
void insert_hash(hash_table * , hash_bucket * ,int);
void remove_hash(hash_table * , hash_bucket * ,int);
hash_bucket * find_hash(hash_table * , void* ,int);


///////////////////////////////////////////////////////////////
#define LS_MEM_MALLOC(Ptr,Type)      Ptr =  (Type *)malloc(sizeof(Type))

#if 1
struct tuple4
{
   u_short source;
   u_short dest;
   u_int saddr;
   u_int daddr;
};
#endif

uint32_t session_hash(void *key, int);
unsigned char session_comp(void *key1, void *key2, int);

#if 0
typedef struct
{
   uint32_t sip;
   uint32_t dip;
   uint32_t port;
}
datasession;

typedef struct
{
   unsigned  char   flag;
   struct tuple4    key;
   unsigned char file_name[128];
}FtpdataSession;


typedef struct
{
	unsigned char flag;
	unsigned char flag2;
	struct tuple4 key;
    unsigned char file_name[128];
	uint32_t      port;
	uint32_t      ip;
}FtpSession;
#endif

/////////////SMTP///////////SMTP/////////SMTP//////////////SMTP////////////////////////

/*瀛樻斁email鏁版嵁鍖呯殑閾捐〃*/
typedef struct SMTP_EML
{
	char			*p_eml;            //閭欢鏁版嵁鍖�
	struct SMTP_EML	*next;
}SMTP_EML;

typedef struct EML_TO
{
    char    *p_to;
	struct  EML_TO  *next;
}EML_TO;
#if 0
/**********************************************************************************/
typedef struct
{
	SMTP_EML        *eml_list;		  //瀛樻斁email 鐨勯摼琛�
	EML_TO          *to_list;		  //瀛樻斁email 鐨勯摼琛�
	unsigned long   eml_to_len;		  //璁＄畻鍙戜欢浜烘墍鍦ㄦ暟鎹寘鐨勫ぇ灏�
	unsigned long   eml_len;		  //鐢ㄦ潵鏍囪閭欢閮ㄥ垎鐨勭姸鎬�
	unsigned short  eml_flag;		  //鐢ㄦ潵鏍囪閭欢閮ㄥ垎鐨勭姸鎬�
	unsigned short  eml_end;		  //褰撳畠涓�1鏃讹紝璇存槑宸茬粡寰楀埌浜嗭紝閭欢鍘熷鏁版嵁
	unsigned short  crlf_seen;		  //
	unsigned short  smtp_state;		  //鏍囪澶勭悊鏁版嵁鍖呯殑鐘舵��
	unsigned short  STARTTLS;		  //鏍囪閭欢鏄惁鍔犲瘑鐨勪腑闂寸姸鎬�
	unsigned short  flag;			  //鏄惁璇绘《鏁版嵁鐨勬爣璁帮紝涓�1鏃讹紝闇�瑕佽妗�
	unsigned short  encypt;			  //閭欢鍔犲瘑鐨勬爣蹇楋紝鍔犲瘑鏃朵负1锛屽綋璇诲埌瀹冩槸1鐨勬椂鍊欓渶瑕佸叧闂細璇濓紝閲婃斁hash
	char            *eml_from;		  //瀛樻斁鍙戜欢浜�
//	char            *eml_to;		  //瀛樻斁鏀朵欢浜�
//	char			*eml;			  //瀛樻斁闇�瑕佽В鏋愮殑EMAIL
	char            *smtp_json;		  //鍖呭惈EML灞炴�х殑JSON涓� 锛岄渶瑕佹彁鍙�
	struct tuple4	key;
}SmtpSession;
/////////////////////////////////////////////////////////////////////////////////////////////////
#endif

#endif
// __BEAP_HASH
