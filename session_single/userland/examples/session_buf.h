
#ifndef  SESSION_BUF_H
#define  SESSION_BUF_H
#include <stdio.h>
#include <pcap.h>
#include <semaphore.h>
#include <pthread.h>
#define MAX_SRING_BUFFER_COUNT 50

struct four_tuple {
        uint32_t ip_src;
        uint32_t ip_dst;    
        uint16_t port_src;
        uint16_t port_dst;
};
typedef struct four_tuple four_tuple_t;


/** 
 * Structure of Each node for a ring buffer 
 * 五元组信息 
 *
 * **/
typedef struct five_attr {
	uint32_t ip_src;	//原IP
	uint32_t ip_dst;	//目的IP
	uint16_t port_src;	//源端口
	uint16_t port_dst;	//目的端口
	uint32_t protocol;	//本文件网络传输的协议
} session_ft;			//五元组信息

/* 
 *ftp协议解析信息
 *
 * */
typedef struct session_buffer_node_tag {
	session_ft session_five_attr;	//五元组信息
	char strname[128];	//本地存储文件0名
	char strpath[128];	//文件路径
	char orgname[128];	//网络上传输的原始文件名
	FILE *old_file;
	void *attr;
} SESSION_BUFFER_NODE;

/**Decleration of Header Structure**/

struct session_buffer_holder {
	SESSION_BUFFER_NODE *rb_base;
	int irb_qlength;
	int irb_head_index;
	int irb_tail_index;
	int irb_buffer_end_index;
	sem_t empty_buffer;
	sem_t full_buffer;
	pthread_mutex_t mutex;
	pthread_mutex_t pmutex;
} sb_holder[MAX_SRING_BUFFER_COUNT];

int sbuff_create(int irb_qlength);
int sbuff_putdata(int iringbuffer_ID, SESSION_BUFFER_NODE ftt);
int sbuff_getdata(int iringbuffer_ID, SESSION_BUFFER_NODE * ftt);
int sbuff_destroy(int iringbuffer_ID);

#endif
