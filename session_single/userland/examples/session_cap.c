/************************************************************
  Copyright (C), 2014, DLP FileName: session_cap.c 
Author: Dai Gang	
Version : V 1.0
Date: 2014-10-10 
Description: 
此程序为DLP监测器主程序，所支持的功能如下：
1）通过PF_ring抓取网络上传输的数据包		   
2）通过对抓取的数据包进行应用协议类型识别，得出每个数据包的协议号			   
3）将带有协议号的网络数据包经过网络数据包重组以及网络会话还原		   
4）还原后的数据包根据协议类型分发到不同的协议内容识别处理模块
5）经过协议识别后的提取的文件通过文件解析模块tika完成文件内容的提取
Version:V 1.0
Function List:	 
1. int runconf_table_conf(char *filename) 
功能：系统运行参数初始化函数，负责从配置文件中读出配置参数的值  
2. void dummyProcesssPacket(const struct pfring_pkthdr *h, const u_char *p, const u_char *user_bytes)	 
功能：pf_ring网络包抓取回调线程，负责对网络包的抓取	 
3. void* packet_consumer_thread(void* _id) 
功能：协议号识别线程，主要负责将抓取的网络数据包进过ndpi识别出具体的应用协议号	
4. void* session_process_thread(void* _id)  
功能：会话重组线程，主要完成将网络上采集的数据进行会话重组  
5. void* session_consumer_thread(void* _id)
功能：会话处理线程池，主要完成将重组后的会话分发到不同的协议识别处理模块进行会话内容提取  
6. void* http_file_thread(void* _id) 
功能：http协议类型处理线程池，主要负责完成通过http应用协议传输的内容识别与提取  
7. void* tika_consumer_thread(void* _id)	 
功能：文件识别与内容提取线程池，主要负责将上述功能模块识别出的文件通过tika提取相应的属性以及文件内容	 
8. void* WL_upload_thread(void* _id)  
功能：白名单以及动态参数配置线程，主要负责白名单的实时更新以及系统动态参数配置过程中参数的实时更新	 
9. static void pcap_packet_callback(u_char * args, const struct pcap_pkthdr *header, const u_char * packet, struct tuple5 *Five_attr)
功能：协议类型识别处理函数接口，主要负责对网络数据包的协议类型进行识别
10.static unsigned int ndpi_packet_processing(const u_int64_t time, \
const struct ndpi_iphdr *iph, \
struct ndpi_ip6_hdr *iph6, \
u_int16_t ip_offset, \
u_int16_t ipsize, u_int16_t rawsize, struct tuple5 *Five_attr)
功能：网络会话重组函数接口，主要负责将网络数据报文按照传输协议进行报文重组与会话还原，反应网络数据有序传输过程
11.void tcp_callback(struct tcp_stream *a_tcp, void **this_time_not_needed)
功能：会话还原回调函数接口，主要负责将会话重组后的网络数据包分发到不同的协议处理函数中进行传输内容识别
12.int rbuff_create (int irb_qlength)
功能：环形缓冲池创建函数接口，主要负责创建环形缓存池存放网络数据包
13.int rbuff_putdata(const int iringbuffer_ID, void *pData, struct pcap_pkthdr *hdr, struct tuple5 *session_five_attr)
功能：填充环形缓冲池函数接口，主要负责将捕获的网络数据包放入环形缓冲池
14.int rbuff_getdata(const int iringbuffer_ID, void *pData, struct pcap_pkthdr *hdr, struct tuple5 *session_five_attr)
功能：读取环形缓冲池函数接口，主要负责将环形缓冲池存放的网络数据包读取出来
15.int sbuff_create (int irb_qlength)
功能：事件处理信号池创建函数，主要负责创建信号池
16.int sbuff_putdata(const int iringbuffer_ID, SESSION_BUFFER_NODE ft)
功能：事件放入处理函数接口，主要负责将待处理的信号放入信号池等待处理
17.int sbuff_getdata(const int iringbuffer_ID, SESSION_BUFFER_NODE *hdr)
功能：事件提取处理函数，主要将事件池中的事件按照顺序提取出来激活就绪态的处理函数
18.void doftp(const char *packet_content, struct tuple4 *ftp_four_id)
功能：FTP协议控制流解析函数接口，主要负责FTP协议控制流内容的具体解析并生成相应的hash表
19.int do_smtp (const u_char *packet_content, struct tuple4 *key, unsigned int packlen, smtp_sbuffputdata sbuff_putdata, int sbuffer_id)
功能：SMTP协议解析函数，主要负责提取SMTP协议特征的解析以及传输内容的初步提取，所属文件：Smtp_dissect.c文件856行
20.int do_http_file(const char *packet_content, int len, struct tuple4 *key, http_sbuffputdata http_sbuff_putdata,const int iringbuffer_ID)
功能: HTTP协议初步解析函数，主要负责完成基于http应用协议的会话内容中POST信息的提取，所属文件：Parsertrace.c文件508行
21.int http( HttpSession *p_session)
功能：主要负责提取http协议传输过程中post头信息以及内容的提取，所属文件：Parsertrace.c文件488行
History: 
<author> <time> <version > <desc> 
Dai Gang  14/10/10 V 1.0 build this moudle  
 ***********************************************************/

#define _GNU_SOURCE
#include <signal.h>
#include <sched.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include <unistd.h>
#include <sys/mman.h>
#include <errno.h>
#include <sys/poll.h>
#include <netinet/in_systm.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <netinet/tcp.h>
#include <netinet/udp.h>
#include <netinet/ip6.h>
#include <net/ethernet.h>     
#include <sys/time.h>
#include <time.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <monetary.h>
#include <locale.h>

#include <syslog.h>
#include <alloca.h>
#include <config.h>
#if (HAVE_UNISTD_H)
#include <unistd.h>
#endif
#include <stdlib.h>
/*会话重组所需头文件*/
#include "checksum.h"
#include "ip_fragment.h"
#include "scan.h"
#include "tcp.h"
#include "util.h"
#include "nids.h"
/*会话重组所需头文件*/
#ifdef HAVE_LIBGTHREAD_2_0
#include <glib.h>
#endif
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>

#ifdef ENABLE_BPF
#include <pcap/pcap.h>
#include <pcap/bpf.h>
//#include <linux/filter.h>
#endif

#include "pfring.h"
#include "pfutils.c"

#include "ring_buffer.h"

#include "read_IM_config.h"
#include "third-party/sort.c"
#include "third-party/node.c"
#include "third-party/ahocorasick.c"
//#include "thread_pool.h"
//#include "session_hash.h"
#include "session_buf.h"

/*********/
#include <stdio.h>
#include <unistd.h>
#include <stdarg.h>
#include <search.h>
#include <signal.h>
/*协议识别模块头文件*/
#include "nDPI/config.h"
#include "linux_compat.h"
#include "ndpi_main.h"
/*协议识别模块头文件*/
/***********白名单部分使用头文件**************************/
#include "msgqueue.h"
#include <jansson.h>
#include "pfcountWL.h"
#include "plog.h"
/***********白名单部分使用头文件**************************/
/***********内存管理器代码**************************/
#define IMLOG_ENABLE
#define IMEM_DEBUG
#define IKMEM_NOFIB
#include "imembase.c"
#include "beap_hash.c"
/***********内存管理器代码**************************/
//#include "fileparse.h"

#if defined(__OpenBSD__)
#include <sys/socket.h>
#endif

/**********************/
//HTTP 协议解析头文件
/**********************/
#include "http_parser.h"

#include "http_IM_parser.h"
#include "webmail_parser.h"
/**********************/
//HTTP 协议解析头文件
/**********************/

/**********************/
//SMTP 协议解析头文件
/**********************/
#include "extractmain.h"

#include "gmime_smtp.h"
/**********************/
//SMTP 协议解析
/**********************/

/*******************************/
//抓包程序配置文件解析头文件
/*******************************/
#include<libxml/parser.h>
#include<libxml/tree.h>
/*******************************/
//抓包程序配置文件解析头文件
/*******************************/

#define CHAR_NUM              128

/******************************************/
#define big_version             0   //now release version
#define mid_version             0   //QA testing version
#define dlp_version             3   //developing version  
/******************************************/
#define ALARM_SLEEP            25
#define MAX_NUM_THREADS        64
#define DEFAULT_SNAPLEN     65536
#define DEFAULT_DEVICE     "eth0"

#define NAMELEN  512

#define int_ntoa(x)	inet_ntoa(*((struct in_addr *)&x))

#define  NDPI_ENABLE_DEBUG_MESSAGES  1
/****************抓包程序初始化参数定义*********************/
char *device = NULL;
unsigned int tika_pthreads = 0;
unsigned int http_pthreads = 0;
unsigned int session_pthreads = 0;
unsigned int max_cfilesize = 104857600;

/*******************************/
//Configration file parser parameter，预留
/*******************************/
typedef struct parser_protocol
{
    int  protocol_num;
    char parser_lib[CHAR_NUM];
    char parser_packet_func[CHAR_NUM];
    char parser__accessory_func[CHAR_NUM];
} parser_protocol_s;

parser_protocol_s parser_protocol[255];
/*******************************/
//Configration file parser parameter，预留
/*******************************/

/****************临时文件存储路径***************/
#define PATHLEN  128
char *session_stream;
char *session_json;
static int global_flag=-1;
/*******************************/
/****************抓包程序初始化参数定义*********************/
pfring  *pd;
int verbose = 0, num_threads = 8;
pfring_stat pfringStats;
void *automa = NULL;
static struct timeval startTime;
unsigned long long numPkts[MAX_NUM_THREADS] = { 0 }, numBytes[MAX_NUM_THREADS] = { 0 }, numStringMatches[MAX_NUM_THREADS] = { 0 };
int buffer_id, bbuffer_id, sbuffer_id, ssbuffer_id,http_buffer_id; //抓包程序所用程序缓冲池ID定义

#ifdef ENABLE_BPF
unsigned long long numPktsFiltered[MAX_NUM_THREADS] = { 0 };
struct bpf_program filter;
u_int8_t userspace_bpf = 0;
#endif

/*pf_ring初始化参数*/
u_int8_t wait_for_packet = 1, do_shutdown = 0, add_drop_rule = 0, show_crc = 0;
u_int8_t use_extended_pkt_header = 0, touch_payload = 0, enable_hw_timestamp = 0, dont_strip_timestamps = 0;
/*pf_ring初始化参数*/

//会话还原定义部分代码！！
#ifdef __linux__
extern int set_all_promisc();
#endif

#define int_ntoa(x)	inet_ntoa(*((struct in_addr *)&x))
extern int ip_options_compile(unsigned char *);
extern int raw_init();
static void nids_syslog(int, int, struct ip *, void *);
static int nids_ip_filter(struct ip *, int);

static struct proc_node *ip_frag_procs = NULL;
static struct proc_node *ip_procs = NULL;
static struct proc_node *udp_procs = NULL;

struct proc_node *tcp_procs = NULL;
static int       linktype = 0;
static pcap_t    *desc = NULL;

#ifdef HAVE_LIBGTHREAD_2_0

/* async queue for multiprocessing - mcree */
static GAsyncQueue *cap_queue = NULL;

/* items in the queue */
struct cap_queue_item {
    void *data;
    bpf_u_int32 caplen;
};

/* marks end of queue */
static struct cap_queue_item EOF_item;

/* error buffer for glib calls */
static GError *gerror = NULL;

#endif
/*********???????PF_RING?????Э?鸯???????????20140626*********/
pthread_t     my_thread[2];  //???????????????????
//#define       THD_NUM 1000 
//TpThreadPool  *pTp = NULL;
int           thread_num = 0;

/* ndpi??????? */
four_tuple_t ft;

struct nids_msgbuf
{
    uint32_t proto_type;
    four_tuple_t   addr;
    char mtext[NAMELEN];    /*  message data */
};

struct nids_with_proparse
{
    long mtype;        /*  message type, must be > 0 */
    struct nids_msgbuf nids_msg;
};

int    msgid = 0;
//ncc_t *ncc = NULL;
struct half_stream *hlf = NULL;

/******************/
char nids_errbuf[PCAP_ERRBUF_SIZE];
struct pcap_pkthdr * nids_last_pcap_header = NULL;
u_char *nids_last_pcap_data = NULL;
u_int nids_linkoffset = 0;

hash_table  *ftp_session_tbl=NULL;

char *nids_warnings[] = {
    "Murphy - you never should see this message !",
    "Oversized IP packet",
    "Invalid IP fragment list: fragment over size",
    "Overlapping IP fragments",
    "Invalid IP header",
    "Source routed IP frame",
    "Max number of TCP streams reached",
    "Invalid TCP header",
    "Too much data in TCP receive queue",
    "Invalid TCP flags"
};

/*会话还原部分初始化参数*/
struct nids_prm nids_params = {
    10400,			/* n_tcp_streams */
    256,			/* n_hosts */
    NULL,			/* device */
    NULL,			/* filename */
    168,			/* sk_buff_size */
    -1,				/* dev_addon */
    nids_syslog,		/* syslog() */
    LOG_ALERT,			/* syslog_level */
    256,			/* scan_num_hosts */
    3000,			/* scan_delay */
    10,				/* scan_num_ports */
    nids_no_mem,		/* no_mem() */
    nids_ip_filter,		/* ip_filter() */
    NULL,			/* pcap_filter */
    1,				/* promisc */
    0,				/* one_loop_less */
    1024,			/* pcap_timeout */
    0,				/* multiproc */
    20000,			/* queue_limit */
    0,				/* tcp_workarounds */
    NULL			/* pcap_desc */
};

/*协议识别处理模块参数定义域*/

/************白名单参数定义******************/
FilterInfo *WL_rules = NULL;
FilterInfo *WL_rules_copy = NULL;
/************白名单参数定义******************/

//程序绑定端口号初始化参数
int  bind_port = 60004;

/////////////////////////////////////////////////////1225////////////////////////////////////////////////////////////
/*会话还原部分初始化参数*/
/*************五元组结构体定义**************/
struct tuple5  *Five_attr = NULL;
/*协议识别处理模块参数定义域*/
#define NUM_ROOTS        16384
#define IDLE_SCAN_BUDGET         1024
#define MAX_NUM_READER_THREADS     16
static u_int8_t live_capture = 1;
#define IDLE_SCAN_PERIOD           10 /* msec (use detection_tick_resolution = 1000) */
#define MAX_IDLE_TIME           30000
static u_int32_t detection_tick_resolution = 1000;
#define	MAX_NDPI_FLOWS	2000000
static u_int8_t enable_protocol_guess = 1;
static time_t capture_until = 0;
static u_int16_t decode_tunnels = 0;
static u_int32_t size_id_struct = 0;
static u_int32_t size_flow_struct = 0;
#define GTP_U_V1_PORT   2152

#ifndef ETH_P_IP
#define ETH_P_IP 0x0800
#endif

typedef struct ndpi_flow {
    u_int32_t lower_ip;
    u_int32_t upper_ip;
    u_int16_t lower_port;
    u_int16_t upper_port;
    u_int8_t detection_completed, protocol;
    u_int16_t __padding;
    struct ndpi_flow_struct *ndpi_flow;
    char lower_name[32], upper_name[32];

    u_int64_t last_seen;

    u_int32_t packets, bytes;
    // result only, not used for flow identification
    u_int32_t detected_protocol;

    char host_server_name[256];

    struct {
        char client_certificate[48], server_certificate[48];
    } ssl;

    void *src_id, *dst_id;
} ndpi_flow_t;

struct thread_stats {
    u_int32_t guessed_flow_protocols;
    u_int64_t raw_packet_count;
    u_int64_t ip_packet_count;
    u_int64_t total_wire_bytes, total_ip_bytes, total_discarded_bytes;
    u_int64_t protocol_counter[NDPI_MAX_SUPPORTED_PROTOCOLS + NDPI_MAX_NUM_CUSTOM_PROTOCOLS + 1];
    u_int64_t protocol_counter_bytes[NDPI_MAX_SUPPORTED_PROTOCOLS + NDPI_MAX_NUM_CUSTOM_PROTOCOLS + 1];
    u_int32_t protocol_flows[NDPI_MAX_SUPPORTED_PROTOCOLS + NDPI_MAX_NUM_CUSTOM_PROTOCOLS + 1];
    u_int32_t ndpi_flow_count;
    u_int64_t tcp_count, udp_count;
    u_int64_t mpls_count, pppoe_count, vlan_count, fragmented_count;
    u_int64_t packet_len[6];
    u_int16_t max_packet_len;
};
struct reader_thread {//ndpi 线程结构体
    struct ndpi_detection_module_struct *ndpi_struct;
    void *ndpi_flows_root[NUM_ROOTS];//树
    char _pcap_error_buffer[PCAP_ERRBUF_SIZE];
    pcap_t *_pcap_handle;
    u_int64_t last_time;//当前时间
    u_int64_t last_idle_scan_time;//上次检查一遍的时间戳
    u_int32_t idle_scan_idx;//当前查询的树标号
    u_int32_t num_idle_flows;//记录当前树超时的节点数量
    pthread_t pthread;
    int _pcap_datalink_type;

    /* TODO Add barrier */
    struct thread_stats stats;

    struct ndpi_flow *idle_flows[IDLE_SCAN_BUDGET];
};
pthread_t ndpi_timeout_free;

static struct reader_thread ndpi_thread_info[MAX_NUM_READER_THREADS];

/*************************************************
Function: free_ndpi_flow()
Description: 释放已申请的数据流资源
Calls: 
Called By:ndpi_packet_processing
Table Accessed: 
Table Updated: 
Input: 已申请的数据流
Output: 无
Return: 无
Others: 
 *************************************************/
static void free_ndpi_flow(struct ndpi_flow *flow) {
    if(flow->ndpi_flow) { ndpi_free(flow->ndpi_flow); flow->ndpi_flow = NULL; }
    if(flow->src_id)    { ndpi_free(flow->src_id); flow->src_id = NULL;       }
    if(flow->dst_id)    { ndpi_free(flow->dst_id); flow->dst_id = NULL;       }
}
/*************************************************
Function: node_cmp()
Description: 协议识别过程中对节点的新来数据的判断。
Calls: 
Called By: ndpi_tsearch() -------> get_ndpi_flow()
Table Accessed: 
Table Updated: 
Input: 待比较的数据流
Output: 相比较的数据一致返回0，其余返回非0
Return: 相比较的数据一致返回0，其余返回非0
Others: 
 *************************************************/
static int node_cmp(const void *a, const void *b) {
    struct ndpi_flow *fa = (struct ndpi_flow*)a;
    struct ndpi_flow *fb = (struct ndpi_flow*)b;

    if(fa->lower_ip < fb->lower_ip) return(-1); else { if(fa->lower_ip > fb->lower_ip) return(1); }
    if(fa->lower_port < fb->lower_port) return(-1); else { if(fa->lower_port > fb->lower_port) return(1); }
    if(fa->upper_ip < fb->upper_ip) return(-1); else { if(fa->upper_ip > fb->upper_ip) return(1); }
    if(fa->upper_port < fb->upper_port) return(-1); else { if(fa->upper_port > fb->upper_port) return(1); }
    if(fa->protocol < fb->protocol) return(-1); else { if(fa->protocol > fb->protocol) return(1); }

    return(0);
}

/*************************************************
Function: malloc_wrapper()
Description: 协议识别过程中的存储空间申请函数。
Calls: 
Called By: ndpi_init_detection_module() -------> setupDetection()
Table Accessed: 
Table Updated: 
Input: 申请空间大小
Output: 申请成功的地址空间首地址
Return: 申请成功的地址空间首地址
Others: 
 *************************************************/
static void *malloc_wrapper(unsigned long size)
{
    return malloc(size);
}
/*************************************************
Function: free_wrapper()
Description: 协议识别过程中的已申请存储空间释放函数。
Calls: 
Called By: ndpi_init_detection_module() -------> setupDetection()
Table Accessed: 
Table Updated: 
Input: 已申请空间首地址
Output: 无
Return: 无
Others: 
 *************************************************/
static void free_wrapper(void *freeable)
{
    free(freeable);
}

static void debug_printf(u_int32_t protocol, void *id_struct,
        ndpi_log_level_t log_level,
        const char *format, ...) {

    //printf("[DEBUG]%s\n",format);

}

/*************************************************
Function: node_guess_undetected_protocol()
Description: 协议解析过程中，通过预设解析过程无法识别出协议号的数据流，经过此函数
进行一定的预测判断得出一个协议号。
Calls: 
Called By: static unsigned int ndpi_packet_processing
Table Accessed: 
Table Updated: 
Input: 网络传入数据流
Output: 推测得出的协议号
Return: 推测得出的协议号
Others: 
 *************************************************/
static unsigned int node_guess_undetected_protocol(u_int16_t thread_id,
        struct ndpi_flow *flow) {
    flow->detected_protocol = ndpi_guess_undetected_protocol(ndpi_thread_info[thread_id].ndpi_struct,
            flow->protocol,
            ntohl(flow->lower_ip),
            ntohs(flow->lower_port),
            ntohl(flow->upper_ip),
            ntohs(flow->upper_port));
    // printf("Guess state: %u\n", flow->detected_protocol);
    if(flow->detected_protocol != 0)
        ndpi_thread_info[thread_id].stats.guessed_flow_protocols++;

    return flow->detected_protocol;
}
/*************************************************
Function: node_proto_guess_walker()
Description: 遍历树
Calls: 
Called By: static unsigned int node_idle_scan_walker
Table Accessed: 
Table Updated: 

Others: 
 *************************************************/

static void node_proto_guess_walker(const void *node, ndpi_VISIT which, int depth, void *user_data) {
    struct ndpi_flow *flow = *(struct ndpi_flow **) node;
    u_int16_t thread_id = *((u_int16_t *) user_data);
    if((which == ndpi_preorder) || (which == ndpi_leaf)) { /* Avoid walking the same node multiple times */
        if(enable_protocol_guess) {
            if(flow->detected_protocol == 0 /* UNKNOWN */) {
                node_guess_undetected_protocol(thread_id, flow);
                // printFlow(thread_id, flow);
            }
        }

        ndpi_thread_info[thread_id].stats.protocol_counter[flow->detected_protocol]       += flow->packets;
        ndpi_thread_info[thread_id].stats.protocol_counter_bytes[flow->detected_protocol] += flow->bytes;
        ndpi_thread_info[thread_id].stats.protocol_flows[flow->detected_protocol]++;
    }
}
/*************************************************
Function: node_idle_scan_walker()
Description: 遍历空闲树的节点
Calls: 
Called By: ndpi_free_thread
Table Accessed: 
Table Updated: 

Others: 
 *************************************************/
static void node_idle_scan_walker(const void *node, ndpi_VISIT which, int depth, void *user_data) {
    struct ndpi_flow *flow = *(struct ndpi_flow **) node;
    u_int16_t thread_id = *((u_int16_t *) user_data);	
    if(ndpi_thread_info[thread_id].num_idle_flows == IDLE_SCAN_BUDGET) /* TODO optimise with a budget-based walk */
        return;
    if((which == ndpi_preorder) || (which == ndpi_leaf)) { /* Avoid walking the same node multiple times */
        if(flow->last_seen + MAX_IDLE_TIME < ndpi_thread_info[thread_id].last_time) {	
            /* update stats */
            node_proto_guess_walker(node, which, depth, user_data);
            free_ndpi_flow(flow);
            ndpi_thread_info[thread_id].stats.ndpi_flow_count--;

            /* adding to a queue (we can't delete it from the tree inline ) */
            ndpi_thread_info[thread_id].idle_flows[ndpi_thread_info[thread_id].num_idle_flows++] = flow;
        }
    }
}
/*************************************************
Function: setupDetection()
Description: ndpi检测协议的注册，以及参数设置
Calls: ndpi_init_detection_module，ndpi_set_protocol_detection_bitmask2，ndpi_detection_get_sizeof_ndpi_id_struct，ndpi_detection_get_sizeof_ndpi_flow_struct
Called By:main
Table Accessed: 
Table Updated: 
Input: 无
Output: 无
Return: 无
Others: 
 *************************************************/
static void setupDetection(u_int16_t thread_id) {
    NDPI_PROTOCOL_BITMASK all;

    memset(&ndpi_thread_info[thread_id], 0, sizeof(ndpi_thread_info[thread_id]));

    // init global detection structure
    ndpi_thread_info[thread_id].ndpi_struct = ndpi_init_detection_module(detection_tick_resolution, malloc_wrapper, free_wrapper, debug_printf);
    if(ndpi_thread_info[thread_id].ndpi_struct == NULL) {
        log_printf(ZLOG_LEVEL_FATAL, "Protocol anaysis function error: global structure initialization failed!!");
        //exit(-1);
    }

    // enable all protocols
    NDPI_BITMASK_SET_ALL(all);
    ndpi_set_protocol_detection_bitmask2(ndpi_thread_info[thread_id].ndpi_struct, &all);

    // allocate memory for id and flow tracking
    size_id_struct = ndpi_detection_get_sizeof_ndpi_id_struct();
    size_flow_struct = ndpi_detection_get_sizeof_ndpi_flow_struct();

    // clear memory for results
    memset(ndpi_thread_info[thread_id].stats.protocol_counter, 0, sizeof(ndpi_thread_info[thread_id].stats.protocol_counter));
    memset(ndpi_thread_info[thread_id].stats.protocol_counter_bytes, 0, sizeof(ndpi_thread_info[thread_id].stats.protocol_counter_bytes));
    memset(ndpi_thread_info[thread_id].stats.protocol_flows, 0, sizeof(ndpi_thread_info[thread_id].stats.protocol_flows));

}

/*************************************************
Function: get_ndpi_flow()
Description: 1、通过传输层拆包获得协议包的源和目的端口（tcp通过ndpi_tcphdr 、udp通过ndpi_udphdr分别进行拆包）
2、结合网络层和传输层的数据，通过源目的ip和端口分类会话
3、以ndpi_flows_root为hash数组，(lower_ip + upper_ip + iph->protocol + lower_port + upper_port) % NUM_ROOTS计算出会话对应的数组位置。然后对于数组的每个单元维护一个二叉查找链表。
4、通过ndpi_tfind函数对二叉树进行查找。
Calls: Called By:1、 ndpi_packet_processing() 2、 get_ndpi_flow6()
Table Accessed: 
Table Updated: 
Input: 
Output: 如果存在相对应的会话，则返回对应结果。如果不存在，则通过ndpi_tsearch把新的会话插入二叉树中。
Return: 如果存在相对应的会话，则返回对应结果。如果不存在，则通过ndpi_tsearch把新的会话插入二叉树中。
Others: 
 *************************************************/
static struct ndpi_flow *get_ndpi_flow(u_int16_t thread_id,
        const u_int8_t version,
        const struct ndpi_iphdr *iph,
        u_int16_t ip_offset,
        u_int16_t ipsize,
        u_int16_t l4_packet_len,
        struct ndpi_id_struct **src,
        struct ndpi_id_struct **dst,
        u_int8_t *proto,
        const struct ndpi_ip6_hdr *iph6)

{
    u_int32_t idx, l4_offset;
    struct ndpi_tcphdr *tcph = NULL;
    struct ndpi_udphdr *udph = NULL;
    u_int32_t lower_ip;
    u_int32_t upper_ip;
    u_int16_t lower_port;
    u_int16_t upper_port;
    struct ndpi_flow flow;
    void *ret;
    u_int8_t *l3;

    /*
Note: to keep things simple (ndpiReader is just a demo app)
we handle IPv6 a-la-IPv4.
*/
    if(version == 4) {
        if(ipsize < 20)
            return NULL;

        if((iph->ihl * 4) > ipsize || ipsize < ntohs(iph->tot_len)
                || (iph->frag_off & htons(0x1FFF)) != 0)
            return NULL;

        l4_offset = iph->ihl * 4;
        l3 = (u_int8_t*)iph;
    } else {
        l4_offset = sizeof(struct ndpi_ip6_hdr);
        l3 = (u_int8_t*)iph6;
    }

    if(l4_packet_len < 64)
        ndpi_thread_info[thread_id].stats.packet_len[0]++;
    else if(l4_packet_len >= 64 && l4_packet_len < 128)
        ndpi_thread_info[thread_id].stats.packet_len[1]++;
    else if(l4_packet_len >= 128 && l4_packet_len < 256)
        ndpi_thread_info[thread_id].stats.packet_len[2]++;
    else if(l4_packet_len >= 256 && l4_packet_len < 1024)
        ndpi_thread_info[thread_id].stats.packet_len[3]++;
    else if(l4_packet_len >= 1024 && l4_packet_len < 1500)
        ndpi_thread_info[thread_id].stats.packet_len[4]++;
    else if(l4_packet_len >= 1500)
        ndpi_thread_info[thread_id].stats.packet_len[5]++;

    if(l4_packet_len > ndpi_thread_info[thread_id].stats.max_packet_len)
        ndpi_thread_info[thread_id].stats.max_packet_len = l4_packet_len;

    if(iph->saddr < iph->daddr) {
        lower_ip = iph->saddr;
        upper_ip = iph->daddr;
    } else {
        lower_ip = iph->daddr;
        upper_ip = iph->saddr;
    }

    *proto = iph->protocol;
    if(iph->protocol == 6 && l4_packet_len >= 20) {
        ndpi_thread_info[thread_id].stats.tcp_count++;

        // tcp
        tcph = (struct ndpi_tcphdr *) ((u_int8_t *) l3 + l4_offset);
        if(iph->saddr < iph->daddr) {
            lower_port = tcph->source;
            upper_port = tcph->dest;
        } else {
            lower_port = tcph->dest;
            upper_port = tcph->source;

            if(iph->saddr == iph->daddr) {
                if(lower_port > upper_port) {
                    u_int16_t p = lower_port;

                    lower_port = upper_port;
                    upper_port = p;
                }
            }
        }
    }  else {
        // non tcp/udp protocols
        lower_port = 0;
        upper_port = 0;
        return (0);	
    }

    flow.protocol = iph->protocol;
    flow.lower_ip = lower_ip, flow.upper_ip = upper_ip;
    flow.lower_port = lower_port, flow.upper_port = upper_port;

    //if(0)
    //printf("[NDPI] [%u][%u:%u <-> %u:%u]\n",
    //iph->protocol, lower_ip, ntohs(lower_port), upper_ip, ntohs(upper_port));

    idx = (lower_ip + upper_ip + iph->protocol + lower_port + upper_port) % NUM_ROOTS;
    ret = ndpi_tfind(&flow, &ndpi_thread_info[thread_id].ndpi_flows_root[idx], node_cmp);

    if(ret == NULL) {
        if(ndpi_thread_info[thread_id].stats.ndpi_flow_count == MAX_NDPI_FLOWS) {
            //printf("ERROR: maximum flow count (%u) has been exceeded\n", MAX_NDPI_FLOWS);
            //exit(-1);
            log_printf(ZLOG_LEVEL_FATAL, "Protocol anaysis function error: stats.ndpi_flow_count == MAX_NDPI_FLOWS failed!!");
        } else {
            struct ndpi_flow *newflow = (struct ndpi_flow*)malloc(sizeof(struct ndpi_flow));

            if(newflow == NULL) {
                //printf("[NDPI] %s(1): not enough memory\n", __FUNCTION__);
                return(NULL);
            }

            memset(newflow, 0, sizeof(struct ndpi_flow));
            newflow->protocol = iph->protocol;
            newflow->lower_ip = lower_ip, newflow->upper_ip = upper_ip;
            newflow->lower_port = lower_port, newflow->upper_port = upper_port;

            if(version == 4) {
                inet_ntop(AF_INET, &lower_ip, newflow->lower_name, sizeof(newflow->lower_name));
                inet_ntop(AF_INET, &upper_ip, newflow->upper_name, sizeof(newflow->upper_name));
            } else {
                inet_ntop(AF_INET6, &iph6->ip6_src, newflow->lower_name, sizeof(newflow->lower_name));
                inet_ntop(AF_INET6, &iph6->ip6_dst, newflow->upper_name, sizeof(newflow->upper_name));
            }

            if((newflow->ndpi_flow = malloc_wrapper(size_flow_struct)) == NULL) {
                //printf("[NDPI] %s(2): not enough memory\n", __FUNCTION__);
                return(NULL);
            } else
                memset(newflow->ndpi_flow, 0, size_flow_struct);

            if((newflow->src_id = malloc_wrapper(size_id_struct)) == NULL) {
                //printf("[NDPI] %s(3): not enough memory\n", __FUNCTION__);
                return(NULL);
            } else
                memset(newflow->src_id, 0, size_id_struct);

            if((newflow->dst_id = malloc_wrapper(size_id_struct)) == NULL) {
                //printf("[NDPI] %s(4): not enough memory\n", __FUNCTION__);
                return(NULL);
            } else
                memset(newflow->dst_id, 0, size_id_struct);

            ndpi_tsearch(newflow, &ndpi_thread_info[thread_id].ndpi_flows_root[idx], node_cmp); /* Add */
            ndpi_thread_info[thread_id].stats.ndpi_flow_count++;

            *src = newflow->src_id, *dst = newflow->dst_id;

            // printFlow(thread_id, newflow);

            return(newflow);
        }
    } else {
        struct ndpi_flow *flow = *(struct ndpi_flow**)ret;

        if(flow->lower_ip == lower_ip && flow->upper_ip == upper_ip
                && flow->lower_port == lower_port && flow->upper_port == upper_port)
            *src = flow->src_id, *dst = flow->dst_id;
        else
            *src = flow->dst_id, *dst = flow->src_id;

        return flow;
    }
}

/*************************************************
Function: ndpi_tdelete2()
Description: 删除NDPI的超时的空闲节点，删除3个层次
Calls: 
Called By: ndpi_free_thread
Table Accessed: 
Table Updated: 

Others: 
 *************************************************/
    void *
ndpi_tdelete2(const void *vkey, void **vrootp,
        int (*compar)(const void *, const void *))
{
    ndpi_node **rootp = (ndpi_node **)vrootp;
    char *key = (char *)vkey;
    ndpi_node *p = (ndpi_node *)1;
    ndpi_node *q;
    ndpi_node *r;
    int cmp;

    if(rootp == (ndpi_node **)0 || *rootp == (ndpi_node *)0)
        return ((ndpi_node *)0);
    while ((cmp = (*compar)(key, (*rootp)->key)) != 0) {
        p = *rootp;
        rootp = (cmp < 0) ?
            &(*rootp)->left :		/* follow left branch */
            &(*rootp)->right;		/* follow right branch */
        if(*rootp == (ndpi_node *)0)
            return ((void *)0);		/* key not found */
    }
    r = (*rootp)->right;			/* D1: */
    if((q = (*rootp)->left) == (ndpi_node *)0)	/* Left (ndpi_node *)0? */
        q = r;
    else if(r != (ndpi_node *)0) {		/* Right link is null? */
        if(r->left == (ndpi_node *)0) {	/* D2: Find successor */
            r->left = q;
            q = r;
        } else {			/* D3: Find (ndpi_node *)0 link */
            for (q = r->left; q->left != (ndpi_node *)0; q = r->left)
                r = q;
            r->left = q->right;
            q->left = (*rootp)->left;
            q->right = (*rootp)->right;
        }
    }
    if((*rootp)->key!=NULL)
    {
        free_ndpi_flow((*rootp)->key);
        ndpi_free((*rootp)->key);
    }
    //printf("ndpi*****************************333333*********************************************free\n");
    ndpi_free((ndpi_node *) *rootp);	/* D4: Free node */
    *rootp = q;				/* link parent to new node */
    return(p);
}

/*************************************************
Function: packet_processing()
Description: 分析传入的每一个数据包的内容，得出相应的协议类型号并将结果传入五元组
Calls: 
Called By:pcap_packet_callback
Table Accessed: 
Table Updated: 
Input: 
Output: struct tuple5 *Five_attr五元组信息
Return: 0
Others: 
 *************************************************/
static unsigned int packet_processing(u_int16_t thread_id,
        const u_int64_t time,
        const struct ndpi_iphdr *iph,
        struct ndpi_ip6_hdr *iph6,
        u_int16_t ip_offset,
        u_int16_t ipsize, u_int16_t rawsize,struct tuple5 *Five_attr) {					      
    struct ndpi_id_struct *src=NULL, *dst=NULL;
    struct ndpi_flow *flow=NULL;
    struct ndpi_flow_struct *ndpi_flow = NULL;
    struct ndpi_tcphdr *tcph = NULL,*tcph_debug=NULL;
    u_int32_t protocol = 0;
    u_int8_t proto=0;
    if(iph)
    {
        tcph_debug = (struct ndpi_tcphdr *)((u_int8_t *)iph + 4 * iph->ihl);
        //			log_printf(ZLOG_LEVEL_DEBUG, "\n\n====================== packet_processing  source port = %d,dest port = %d===================\n\n",ntohs(tcph_debug->source),ntohs(tcph_debug->dest));
    }
    if(iph)
    {	
        tcph = (struct ndpi_tcphdr *)((u_int8_t *)iph + 4 * iph->ihl);
        if((ntohs(tcph->source) == 21)||(ntohs(tcph->dest) == 21)){//根据常规定义端口号：21过滤数据包，协议号定义为1，即ftp控制流
            Five_attr->four_attr.source = ntohs(tcph->source);
            Five_attr->four_attr.dest = ntohs(tcph->dest);
            Five_attr->four_attr.saddr = iph->saddr;
            Five_attr->four_attr.daddr = iph->daddr;
            Five_attr->protocol = 1;
            return (0);
        }

        if((ntohs(tcph->source) == 25)||(ntohs(tcph->dest) == 25)){//根据常规端口号：25过滤数据包，协议号定义为3，即SMTP数据流
            //printf("Protocol anaysis function: The protocol is port!!\n\n");
            Five_attr->four_attr.source = ntohs(tcph->source);
            Five_attr->four_attr.dest = ntohs(tcph->dest);
            Five_attr->four_attr.saddr = iph->saddr;
            Five_attr->four_attr.daddr = iph->daddr;
            Five_attr->protocol = 3;
            return (0);
        }
        //根据下述常规端口号过滤数据包，协议号定义为7，即http数据流
        if((ntohs(tcph->source) == 80)||(ntohs(tcph->dest) == 80)||(ntohs(tcph->source) == 8080)||(ntohs(tcph->dest) == 8080)){
            Five_attr->four_attr.source = ntohs(tcph->source);
            Five_attr->four_attr.dest = ntohs(tcph->dest);
            Five_attr->four_attr.saddr = iph->saddr;
            Five_attr->four_attr.daddr = iph->daddr;
            Five_attr->protocol = 7;
            return (0);
        }
        flow = get_ndpi_flow(thread_id, 4, iph, ip_offset, ipsize,
                ntohs(iph->tot_len) - (iph->ihl * 4),
                &src, &dst, &proto, NULL);
    }
    else
    {
        // flow = get_ndpi_flow6(thread_id, iph6, ip_offset, &src, &dst, &proto);
        return(0);
    }
    if(flow != NULL) {
        ndpi_thread_info[thread_id].stats.ip_packet_count++;
        ndpi_thread_info[thread_id].stats.total_wire_bytes += rawsize + 24 /* CRC etc */, ndpi_thread_info[thread_id].stats.total_ip_bytes += rawsize;
        ndpi_flow = flow->ndpi_flow;
        flow->packets++, flow->bytes += rawsize;
        flow->last_seen = time;
    } else {
        return(0);
    }

    //log_printf(ZLOG_LEVEL_INFO, "\n\n====================== iph->protol = %d===================\n\n",iph->protocol);
    if(flow->detection_completed){
        tcph = (struct ndpi_tcphdr *)((u_int8_t *)iph + 4 * iph->ihl);
        Five_attr->four_attr.source = ntohs(tcph->source);
        Five_attr->four_attr.dest = ntohs(tcph->dest);
        Five_attr->four_attr.saddr = iph->saddr;
        Five_attr->four_attr.daddr = iph->daddr;
        Five_attr->protocol = flow->detected_protocol;
        //	log_printf(ZLOG_LEVEL_INFO, "\n\n====================== Five_attr->protocol= %d===================\n\n",Five_attr->protocol);
        return(0);
    }




    protocol = (const u_int32_t)ndpi_detection_process_packet(ndpi_thread_info[thread_id].ndpi_struct, ndpi_flow,
            iph ? (uint8_t *)iph : (uint8_t *)iph6,
            ipsize, time, src, dst);

    flow->detected_protocol = protocol;
    if((flow->detected_protocol != NDPI_PROTOCOL_UNKNOWN)
            //   || ((proto == IPPROTO_UDP) && (flow->packets > 8))
            || ((proto == IPPROTO_TCP) && (flow->packets > 10))) {
        flow->detection_completed = 1;
        tcph = (struct ndpi_tcphdr *)((u_int8_t *)iph + 4 * iph->ihl);
        Five_attr->four_attr.source = ntohs(tcph->source);
        Five_attr->four_attr.dest = ntohs(tcph->dest);
        Five_attr->four_attr.saddr = iph->saddr;
        Five_attr->four_attr.daddr = iph->daddr;
        Five_attr->protocol = flow->detected_protocol;
        snprintf(flow->host_server_name, sizeof(flow->host_server_name), "%s", flow->ndpi_flow->host_server_name);

        //  if((proto == IPPROTO_TCP) && (flow->detected_protocol != NDPI_PROTOCOL_DNS)) {
        //    snprintf(flow->ssl.client_certificate, sizeof(flow->ssl.client_certificate), "%s", flow->ndpi_flow->protos.ssl.client_certificate);
        //    snprintf(flow->ssl.server_certificate, sizeof(flow->ssl.server_certificate), "%s", flow->ndpi_flow->protos.ssl.server_certificate);
        //   }

        free_ndpi_flow(flow);

        if(1) {
            if(enable_protocol_guess) {
                if(flow->detected_protocol == 0 /* UNKNOWN */) {
                    tcph = (struct ndpi_tcphdr *)((u_int8_t *)iph + 4 * iph->ihl);
                    Five_attr->four_attr.source = ntohs(tcph->source);
                    Five_attr->four_attr.dest = ntohs(tcph->dest);
                    Five_attr->four_attr.saddr = iph->saddr;
                    Five_attr->four_attr.daddr = iph->daddr;
                    Five_attr->protocol= node_guess_undetected_protocol(thread_id, flow);
                }
            }

            // printFlow(thread_id, flow);
        }
    }
    //	log_printf(ZLOG_LEVEL_INFO, "\n\n=========FUCK============= Five_attr->protocol= %d===================\n\n",Five_attr->protocol);

#if 0
    if(ndpi_flow->l4.tcp.host_server_name[0] != '\0')
        printf("%s\n", ndpi_flow->l4.tcp.host_server_name);

    if(live_capture) {
        if(ndpi_thread_info[thread_id].last_idle_scan_time + IDLE_SCAN_PERIOD < ndpi_thread_info[thread_id].last_time) {	 	
            /* scan for idle flows */	  
            ndpi_twalk(ndpi_thread_info[thread_id].ndpi_flows_root[ndpi_thread_info[thread_id].idle_scan_idx], node_idle_scan_walker, &thread_id);
            /* remove idle flows (unfortunately we cannot do this inline) */
            while (ndpi_thread_info[thread_id].num_idle_flows > 0)
            {	  	
                ndpi_tdelete2(ndpi_thread_info[thread_id].idle_flows[--ndpi_thread_info[thread_id].num_idle_flows],
                        &ndpi_thread_info[thread_id].ndpi_flows_root[ndpi_thread_info[thread_id].idle_scan_idx], node_cmp);
            }
            if(++ndpi_thread_info[thread_id].idle_scan_idx == NUM_ROOTS) ndpi_thread_info[thread_id].idle_scan_idx = 0;
            ndpi_thread_info[thread_id].last_idle_scan_time = ndpi_thread_info[thread_id].last_time;
        }
    }
#endif
    return 0;
}

/*************************************************
Function: ndpi_free_thread()
Description: 释放超时的NDPI树到诘隳
Calls: 
Called By:ndpi_timeout_free
Table Accessed: 
Table Updated: 
Input: 
Output: void
Return: 0
Others: 
 *************************************************/
void* ndpi_free_thread(void* _id) {
    //printf("ndpi*******************************111113*******************************************free\n");

    u_int16_t thread_id=0; 
    while(1) {
        if(ndpi_thread_info[thread_id].last_idle_scan_time + IDLE_SCAN_PERIOD < ndpi_thread_info[thread_id].last_time) {	 	
            /* scan for idle flows */	  
            //printf("ndpi****************************22222**********************************************free\n");
            ndpi_twalk(ndpi_thread_info[thread_id].ndpi_flows_root[ndpi_thread_info[thread_id].idle_scan_idx], node_idle_scan_walker, &thread_id);
            /* remove idle flows (unfortunately we cannot do this inline) */
            while (ndpi_thread_info[thread_id].num_idle_flows > 0)
            {	  	
                ndpi_tdelete2(ndpi_thread_info[thread_id].idle_flows[--ndpi_thread_info[thread_id].num_idle_flows],
                        &ndpi_thread_info[thread_id].ndpi_flows_root[ndpi_thread_info[thread_id].idle_scan_idx], node_cmp);
            }
            if(++ndpi_thread_info[thread_id].idle_scan_idx == NUM_ROOTS) ndpi_thread_info[thread_id].idle_scan_idx = 0;
            ndpi_thread_info[thread_id].last_idle_scan_time = ndpi_thread_info[thread_id].last_time;
        }
        sleep(1);
    }
}
/*************************************************
Function: pcap_packet_callback()
Description: 对传入的数据包的内容进行初步分析，按照参数pcap_pkthdr信息解析包结构并将结果传入下一个函数进行深入的协议分析
Calls: ndpi_packet_processing
Called By:packet_consumer_thread
Table Accessed: 
Table Updated: 
Input: 
struct pcap_pkthdr *header,//数据头信息 
u_char * packet//网络数据包
Output: struct tuple5 *Five_attr，经过分析的五元组信息
Return: 无
Others: 
 *************************************************/
static void pcap_packet_callback(u_int16_t thread_id, const struct pcap_pkthdr *header, const u_char *packet,struct tuple5 *Five_attr) 
{
    const struct ndpi_ethhdr *ethernet;
    struct ndpi_iphdr *iph;
    struct ndpi_ip6_hdr *iph6;
    u_int64_t time;
    u_int16_t type, ip_offset, ip_len;
    u_int16_t frag_off = 0;
    u_int8_t proto = 0, vlan_packet = 0;
    ndpi_thread_info[thread_id]._pcap_datalink_type =DLT_EN10MB;
    // printf("[ndpiReader] pcap_packet_callback : [%u.%u.%u.%u.%u -> %u.%u.%u.%u.%u]\n", ethernet->h_dest[1],ethernet->h_dest[2],ethernet->h_dest[3],ethernet->h_dest[4],ethernet->h_dest[5],ethernet->h_source[1],ethernet->h_source[2],ethernet->h_source[3],ethernet->h_source[4],ethernet->h_source[5]);
    ndpi_thread_info[thread_id].stats.raw_packet_count++;
    if((capture_until != 0) && (header->ts.tv_sec >= capture_until)) {
        // if(ndpi_thread_info[thread_id]._pcap_handle != NULL)
        // pcap_breakloop(ndpi_thread_info[thread_id]._pcap_handle);

        return;
    }
    // if (!live_capture) {
    //if (!pcap_start.tv_sec) pcap_start.tv_sec = header->ts.tv_sec, pcap_start.tv_usec = header->ts.tv_usec;
    // pcap_end.tv_sec = header->ts.tv_sec, pcap_end.tv_usec = header->ts.tv_usec;
    // }
    time = ((uint64_t) header->ts.tv_sec) * detection_tick_resolution +
        header->ts.tv_usec / (1000000 / detection_tick_resolution);
    if(ndpi_thread_info[thread_id].last_time > time) { /* safety check */
        //printf("111111111111111111\n");
        time = ndpi_thread_info[thread_id].last_time;
    }
    ndpi_thread_info[thread_id].last_time = time;

    if(ndpi_thread_info[thread_id]._pcap_datalink_type == DLT_NULL) {
        if(ntohl(*((u_int32_t*)packet)) == 2)
            type = ETH_P_IP;
        else
            type = 0x86DD; /* IPv6 */

        ip_offset = 4;
    } else if(ndpi_thread_info[thread_id]._pcap_datalink_type == DLT_EN10MB) {
        ethernet = (struct ndpi_ethhdr *) packet;
        ip_offset = sizeof(struct ndpi_ethhdr);
        type = ntohs(ethernet->h_proto);
    } else if(ndpi_thread_info[thread_id]._pcap_datalink_type == 113 /* Linux Cooked Capture */) {
        type = (packet[14] << 8) + packet[15];
        ip_offset = 16;
    } else
        return;	
    while(1) {
        if(type == 0x8100 /* VLAN */) {
            type = (packet[ip_offset+2] << 8) + packet[ip_offset+3];
            ip_offset += 4;
            vlan_packet = 1;
        } else if(type == 0x8847 /* MPLS */) {
            u_int32_t label = ntohl(*((u_int32_t*)&packet[ip_offset]));

            ndpi_thread_info[thread_id].stats.mpls_count++;
            type = 0x800, ip_offset += 4;

            while((label & 0x100) != 0x100) {
                ip_offset += 4;
                label = ntohl(*((u_int32_t*)&packet[ip_offset]));
            }
        } else if(type == 0x8864 /* PPPoE */) {
            ndpi_thread_info[thread_id].stats.pppoe_count++;
            type = 0x0800;
            ip_offset += 8;
        } else
            break;
    }

    ndpi_thread_info[thread_id].stats.vlan_count += vlan_packet;

    iph = (struct ndpi_iphdr *) &packet[ip_offset];

    // just work on Ethernet packets that contain IP
    if(type == ETH_P_IP && header->caplen >= ip_offset) {
        frag_off = ntohs(iph->frag_off);

        proto = iph->protocol;
        if(header->caplen < header->len) {
            static u_int8_t cap_warning_used = 0;

            if(cap_warning_used == 0) {
                log_printf(ZLOG_LEVEL_WARN, "Protocol anaysis WARNING: packet capture size is smaller than packet size, DETECTION MIGHT NOT WORK CORRECTLY!!");
                cap_warning_used = 1;
            }
        }
    }

    if(iph->version == 4) {
        ip_len = ((u_short)iph->ihl * 4);
        iph6 = NULL;

        if((frag_off & 0x3FFF) != 0) {
            static u_int8_t ipv4_frags_warning_used = 0;

            ndpi_thread_info[thread_id].stats.fragmented_count++;
            if(ipv4_frags_warning_used == 0) {
                log_printf(ZLOG_LEVEL_WARN,"WARNING: IPv4 fragments are not handled by this demo (nDPI supports them)\n");
                ipv4_frags_warning_used = 1;
            }

            ndpi_thread_info[thread_id].stats.total_discarded_bytes +=  header->len;
            return;
        }
    } else if(iph->version == 6) {
        iph6 = (struct ndpi_ip6_hdr *)&packet[ip_offset];
        proto = iph6->ip6_ctlun.ip6_un1.ip6_un1_nxt;
        ip_len = sizeof(struct ndpi_ip6_hdr);
        iph = NULL;
    } else {
        static u_int8_t ipv4_warning_used = 0;
v4_warning:
        if(ipv4_warning_used == 0) {
            log_printf(ZLOG_LEVEL_WARN,"WARNING: only IPv4/IPv6 packets are supported in this demo (nDPI supports both IPv4 and IPv6), all other packets will be discarded\n\n");
            ipv4_warning_used = 1;
        }
        ndpi_thread_info[thread_id].stats.total_discarded_bytes +=  header->len;
        return;
    }

    if(decode_tunnels && (proto == IPPROTO_UDP)) {
        struct ndpi_udphdr *udp = (struct ndpi_udphdr *)&packet[ip_offset+ip_len];
        u_int16_t sport = ntohs(udp->source), dport = ntohs(udp->dest);

        if((sport == GTP_U_V1_PORT) || (dport == GTP_U_V1_PORT)) {
            /* Check if it's GTPv1 */
            u_int offset = ip_offset+ip_len+sizeof(struct ndpi_udphdr);
            u_int8_t flags = packet[offset];
            u_int8_t message_type = packet[offset+1];

            if((((flags & 0xE0) >> 5) == 1 /* GTPv1 */) && (message_type == 0xFF /* T-PDU */)) {
                ip_offset = ip_offset+ip_len+sizeof(struct ndpi_udphdr)+8 /* GTPv1 header len */;

                if(flags & 0x04) ip_offset += 1; /* next_ext_header is present */
                if(flags & 0x02) ip_offset += 4; /* sequence_number is present (it also includes next_ext_header and pdu_number) */
                if(flags & 0x01) ip_offset += 1; /* pdu_number is present */

                iph = (struct ndpi_iphdr *) &packet[ip_offset];

                if(iph->version != 4) {
                    // printf("WARNING: not good (packet_id=%u)!\n", (unsigned int)ndpi_thread_info[thread_id].stats.raw_packet_count);
                    goto v4_warning;
                }
            }
        }
    }
    // process the packet
    packet_processing(thread_id, time, iph, iph6, ip_offset, header->len - ip_offset, header->len,Five_attr);
}

/***************************
Function: packet_consumer_thread()
Description: 协议类型识别线程，此线程主要是从数据池中取出数据并交给协议类型识别函数识别数据包的协议类型
并将识别出的五元组信息以及相应的数据存入另一个数据缓冲池等待下一个线程处理
Calls: rbuff_getdata，pcap_packet_callback，rbuff_putdata
Called By: 
Table Accessed: 
Table Updated: 
Input: 线程临时ID号，组要通过它与CPU绑定
Output: 无
Return: 无
Others: 
 ***************************/
void* packet_consumer_thread(void* _id) 
{
    u_char *pbuffer_p = NULL;//= buffer;
    u_int numCPU = sysconf( _SC_NPROCESSORS_ONLN );
    long thread_id = (long)_id;
    u_long core_id = thread_id % numCPU;

    if((num_threads > 1) && (numCPU > 1)) {
        if(bind2core(core_id) == 0)
            log_printf(ZLOG_LEVEL_INFO, "packet consumer INFO: Set thread %lu on core %lu/%u !!", thread_id, core_id, numCPU);
    }

    struct pcap_pkthdr *hdr = NULL;
    hdr = malloc(sizeof(struct pcap_pkthdr));
    if(hdr==NULL){
        log_printf(ZLOG_LEVEL_FATAL, "packet consumer FATAL: malloc pcap pkthdr failed!!");
        return (NULL);
    }
    pbuffer_p = malloc(65536);
    if(pbuffer_p==NULL){
        log_printf(ZLOG_LEVEL_FATAL, "packet consumer FATAL: malloc pbuffer_p failed!!");
        return(NULL);
    }
    while(1){

        if(rbuff_getdata(buffer_id, pbuffer_p, hdr, NULL)>0){	
            Five_attr->four_attr.source = 0;
            Five_attr->four_attr.dest = 0;
            Five_attr->four_attr.saddr = 0;
            Five_attr->four_attr.daddr = 0;
            Five_attr->protocol = 0;
            //	log_printf(ZLOG_LEVEL_DEBUG, "\n\n===================== Step 2 packet_consumer_thread Enter into =====================\n\n");
            pcap_packet_callback(thread_id, hdr, pbuffer_p, Five_attr);  
            //	log_printf(ZLOG_LEVEL_DEBUG, "\n\n===================== Step 2 packet_consumer_thread   Exit =====================\n\n");
            rbuff_putdata(bbuffer_id, pbuffer_p, hdr, Five_attr);
        }

    }
    return(NULL);
}

/////////////////////////////////////////////////1225//////////////////////////////////////////////////////////////////



/*************************************************
Function: singleton()
Description: 程序与计算机端口绑定函数，主要功能是使得一台机器上只能正常启动一次程序，一旦程序启动成功并与端口绑定，本机上将无法再次启动此程序
Calls: 
Called By: 
int main(int argc, char* argv[])
Table Accessed: 
Table Updated: 
Input: 计算机端口号
Output: 
Return: 
Others: 
 *************************************************/
void singleton(int port) {
    int servfd = 0;
    struct sockaddr_in servaddr;

    srand(time(NULL));
    if ((servfd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        log_printf(ZLOG_LEVEL_ERROR, "Create socket error!");
        exit(1);
    }

    int opt = 1;
    int len = sizeof(opt);
    setsockopt(servfd, SOL_SOCKET, SO_REUSEADDR, (char *) &opt, len);

    bzero(&servaddr, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(port);

    if (bind(servfd, (struct sockaddr *) &servaddr, sizeof(servaddr)) < 0) {
        log_printf(ZLOG_LEVEL_FATAL, "There has already been a running session_cap process!");
        exit(1);
    }
    if (listen(servfd, 1) < 0) {
        log_printf(ZLOG_LEVEL_FATAL, "Call listen failure!");
        exit(1);
    }
}

/*************************************************
Function: parstr()
Description: 分割字符串
Calls: 
Called By: 
int main(int argc, char* argv[])
Table Accessed: 
Table Updated: 
Input: 1. str         要分割的字符串
2. result     存放分割完的首地址  
3. delim      分割符
4. num       要分割成几个
5. size        每个的长度
Output: 
Return: 返回分割的个数
Others: 
 *************************************************/
unsigned int parstr(const char *str, \
        char *result, \
        const char *delim, \
        const unsigned int num, \
        const unsigned int size)
{
    char  *pStr1 = NULL, *pStr2 = NULL, *pStr3 = NULL;
    int   len = 0, i = 0, j = 0;

    len = strlen(str);
    pStr3 = pStr1 = malloc(len + 1);
    strncpy(pStr1, str, len + 1);
    pStr2 = (char *)strtok_r(pStr1, delim, &pStr1);

    while (pStr2 != 0 &&((*(pStr2)&0xff)!=0x0a)&& i < num)
    {
        strncpy((char *)(result + i * size), pStr2, size);
        for (j=0;j<size;j++)
        {
            if ((( *(result + i * size +j))&0xff)==0)
                break;
            if (((*(result + i * size +j))&0xff)==0x0a)
            {
                *(result + i * size + j) = 0;
            }
        }
        pStr2 = (char *)strtok_r(NULL, delim, &pStr1);
        i ++;
    }

    free(pStr3);
    return i;
}

/*************************************************
Function: doftp
Description: 解析ftp协议的控制流
Input: 1. packet_content    报文流
2. ftp_four_id           四元组
Output: 无
Return: 无
 *************************************************/

void doftp(const char *packet_content, struct tuple4 *ftp_four_id)
{   
    char            *p1=NULL, *p2=NULL, *p3=NULL;
    unsigned short	port1 = 0;  // 存放报文中得到的ip
    unsigned long   ip = 0;     //存放报文中得到的端口
    unsigned long   ip_add = 0;
    unsigned char   user_name[64] = {};  //用户名 
    unsigned char 	file_name[128] = {};  //长传文件名
    char		    pStr[64] = {}, pResult[6][8];   //分割字符串用
    //int             address[4] = {}, port[2] = {};
    struct in_addr  addr1;
    hash_bucket     *pBucket = NULL,*TBucket = NULL;	
    FtpSession	    *p_session = NULL;  //ftp控制流的结构体
    FtpdataSession  *data_session = NULL;  //ftp 数据流的结构体
    unsigned  char  flag = 0;
    four_tuple_t    key3;
    //uint32_t        hash_num = 0;
    //int             i = 0;
    int             len = 0;
    //int             n = 0;
    p1 =(char *)(packet_content);
    /*查找hash桶*/
    if ((pBucket = find_hash(ftp_session_tbl, ftp_four_id, sizeof(struct tuple4))) == NULL)
    {        /*分配内存*/

        LS_MEM_MALLOC(pBucket, hash_bucket);
        LS_MEM_MALLOC(p_session, FtpSession);
        memset(p_session,0,sizeof(FtpSession));
        /*给hash桶附值*/
        memcpy(&p_session->key, ftp_four_id, sizeof(struct tuple4));
        pBucket->content = (void *)p_session;
        pBucket->key = &(p_session->key);
        /*插入hash桶*/
        //  log_printf(ZLOG_LEVEL_INFO, "\n\n$$$$$$$$$$$$$$  step 1 insert_hash $$$$$$$$$$$$$$$$$$$$\n\n");
        insert_hash(ftp_session_tbl, pBucket, sizeof(struct tuple4));
    }
    /*取出hash桶的内容*/
    p_session=(FtpSession *)pBucket->content;
    if(p1!=NULL)
    {            
        /*STOR或STOU上传文件的命令*/
        if ((0 == memcmp(p1, "STOR ", 5))||(0 == memcmp(p1, "STOU ", 5)))  //add_time:20140826;reason:The other transfer command"STOU",but it just transfer once for the same name
        {
            p2 = (char *)strstr(p1,"\r\n");
            if(p2 != NULL)
            {
                p3=p1;
                p3 += 5;
                len = p2 - p3;
                len = len<128?len:(127);
                memset(file_name,0,sizeof(file_name));
                memcpy(file_name, p3, len);
                file_name[len]='\0';
                memcpy(p_session->file_name,file_name,127);
                //n=1;
                p_session->flag=1;//置1的目的是为了得到150命令时  知道上一条命令是 STOR
                /*因为只能得到一个端口 所以用三个元素 第四个固定不变*/
                key3.ip_src = ftp_four_id->saddr;
                key3.ip_dst = ftp_four_id->daddr;
                key3.port_src = (p_session->port&0x0000FFFF);	
                key3.port_dst = 0x0000;
                /*查找hash桶*/
                if ((TBucket = find_hash(ftp_session_tbl, &key3, sizeof(struct tuple4))) == NULL)
                {
                    LS_MEM_MALLOC(TBucket, hash_bucket);
                    LS_MEM_MALLOC(data_session, FtpdataSession);
                    memset(data_session,0,sizeof(FtpdataSession));
                    if(p_session->flag2==3)//如果是主动模式调换ip
                    {
                        key3.ip_src = ftp_four_id->daddr;
                        key3.ip_dst = ftp_four_id->saddr;
                    }
                    /*给hash桶赋值*/	
                    memcpy(&data_session->key, &key3, sizeof(struct tuple4));
                    memcpy(&data_session->file_name, p_session->file_name, 127);
                    TBucket->content = (void *)data_session;
                    TBucket->key = &(data_session->key);
                    data_session->flag=flag;
                    /*插入hash桶*/
                    //  log_printf(ZLOG_LEVEL_INFO, "\n\n$$$$$$$$$$$$$$  step 2 insert_hash $$$$$$$$$$$$$$$$$$$$\n\n");
                    insert_hash(ftp_session_tbl, TBucket, sizeof(struct tuple4));															
                }   
            }
        }
        /*APPE     增加文件内容*/				
        if (0 == memcmp(p1, "APPE ", 5))
        {
            p2 = (char *)strstr(p1,"\r\n");
            if(p2 != NULL)
            {
                /*解析出上传的文件名*/
                p3=p1;
                p3 += 5;
                len = p2 - p3;
                len = len<128?len:(127);
                memset(file_name,0,sizeof(file_name));
                memcpy(file_name, p3, len);
                file_name[len]='\0';
                memcpy(p_session->file_name,file_name,127);
                //n=1;
                p_session->flag=1;
                key3.ip_src = ftp_four_id->saddr;
                key3.ip_dst = ftp_four_id->daddr;
                key3.port_src = (p_session->port&0x0000FFFF);	
                key3.port_dst = 0x0000;
                if ((TBucket = find_hash(ftp_session_tbl, &key3, sizeof(struct tuple4))) == NULL)
                {
                    LS_MEM_MALLOC(TBucket, hash_bucket);
                    LS_MEM_MALLOC(data_session, FtpdataSession);
                    memset(data_session,0,sizeof(FtpdataSession));
                    if(p_session->flag2==3)
                    {
                        key3.ip_src = ftp_four_id->daddr;
                        key3.ip_dst = ftp_four_id->saddr;
                    }
                    memcpy(&data_session->key, &key3, sizeof(struct tuple4));
                    memcpy(&data_session->file_name, p_session->file_name, 127);
                    TBucket->content = (void *)data_session;
                    TBucket->key = &(data_session->key);
                    data_session->flag=flag;
                    //	  log_printf(ZLOG_LEVEL_INFO, "\n\n$$$$$$$$$$$$$$  step 3 insert_hash $$$$$$$$$$$$$$$$$$$$\n\n");
                    insert_hash(ftp_session_tbl, TBucket, sizeof(struct tuple4));															
                }
            }
        }
        /*PORT  主动模式*/
        if(0 == memcmp(p1,"PORT",4))
        {
            p2=p1+5;
            while (*p2!= '\0' && !isdigit(*p2))
                p2++;
            if (*p2== '\0')
            {
                log_printf(ZLOG_LEVEL_WARN, "doftp PORT Matching error!!");
            }
            p3 = (char *)strstr(p1,"\r\n");
            if (p2> p3)
            {
                log_printf(ZLOG_LEVEL_WARN, "doftp PORT Matching no end!!");
            }
            len =p3- p2;
            len = (len > 64)? 64:len;
            memcpy(pStr, p2, len+1);
            /*分割字符串*/
            if (parstr(pStr, (char *)pResult, ",", 6, 8) < 6)
            {
                log_printf(ZLOG_LEVEL_WARN, "doftp PORT Matching IP and Port fail!!");
            }                    
            /*解析出ip和端口*/		  
            ip = ((unsigned long)(atoi(pResult[0])) << 24) |\
                 ((unsigned long)(atoi(pResult[1])) << 16) |\
                 ((unsigned long)(atoi(pResult[2])) << 8) |\
                 ((unsigned long)(atoi(pResult[3])));
            port1 = ((unsigned short)(atoi(pResult[4])) << 8) |\
                    ((unsigned short) (atoi(pResult[5])));
            ip_add =ntohl(ip);
            memcpy(&addr1, &ip_add, 4);
            p_session->flag2=3;  //主动模式
            p_session->ip=ip;
            p_session->port=port1;
            p_session->flag=0;
        }
        /*150*/
        if(0 == memcmp(p1,"150",3))
        {
            if(1==p_session->flag)
            {
                key3.ip_src = ftp_four_id->saddr;
                key3.ip_dst = ftp_four_id->daddr;
                key3.port_src = (p_session->port&0x0000FFFF);	
                key3.port_dst = 0x0000;
                flag = 0x55;
                /*查找hash桶*/
                if ((TBucket = find_hash(ftp_session_tbl, &key3, sizeof(struct tuple4))) == NULL)
                {
                    LS_MEM_MALLOC(TBucket, hash_bucket);
                    LS_MEM_MALLOC(data_session, FtpdataSession);
                    memset(data_session,0,sizeof(FtpdataSession));
                    if(p_session->flag2==3)//如果是主动模式调换ip
                    {
                        key3.ip_src = ftp_four_id->daddr;
                        key3.ip_dst = ftp_four_id->saddr;
                    }
                    memcpy(&data_session->key, &key3, sizeof(struct tuple4));
                    memcpy(&data_session->file_name, p_session->file_name, 127);
                    TBucket->content = (void *)data_session;
                    TBucket->key = &(data_session->key);
                    data_session->flag=flag;
                    /*插入hash桶*/
                    //  log_printf(ZLOG_LEVEL_INFO, "\n\n$$$$$$$$$$$$$$  step 4 insert_hash $$$$$$$$$$$$$$$$$$$$\n\n");
                    insert_hash(ftp_session_tbl, TBucket, sizeof(struct tuple4));															
                }
#if 0 //by niulw
                /*删除hash桶*/
                remove_hash(ftp_session_tbl, pBucket, sizeof(struct tuple4));
                if(pBucket->content != NULL) { 
                    free(pBucket->content);
                    pBucket->content = NULL;
                }
                if(pBucket != NULL){
                    free(pBucket);
                    pBucket = NULL;
                }
                return;
#endif
            }
            p_session->flag=0;
        }
        /*USER   用户名*/	
        if(0 == memcmp(p1,"USER ",5))
        {
            p2 = (char *)strstr(p1,"\r\n");
            if(p2 != NULL)
            {
                p3=p1;
                p3 += 5;
                len = p2 - p3;
                len = len<64?len:(63);
                memset(user_name,0,sizeof(user_name));
                memcpy(user_name, p3, len);
                user_name[len]='\0';
            }
            p_session->flag=0; 
        }
        /*PASS  用户密码*/
        if(0 == memcmp(p1,"PASS ",5))
        {
            p2 = (char *)strstr(p1,"\r\n");
            if(p2 != NULL)
            {
                p3=p1;
                p3 += 5;
                len = p2 - p3;
                len = len<64?len:(63);
                memset(user_name,0,sizeof(user_name));
                memcpy(user_name, p3, len);
                user_name[len]='\0';
            }	  
            p_session->flag=0;
        }
        /*227 被动模式*/         
        /////////////////////////////////////////////PASV////////////////////////////////////////////            
        if (memcmp(p1, "227", 3)==0)
        {
            if ((p2= (char *)strstr(p1,"(")))
            {
                if ((p3= (char *)strstr(p1,")")))
                {
                    if (p2> p3)
                    {
                        log_printf(ZLOG_LEVEL_WARN, "doftp no Matching for 227!!");
                    }
                    len = p3-p2;
                    len = (len > 64)? 64:len;
                    memcpy(pStr, p2+1, len);
                    /*分割字符串*/				
                    if (parstr(pStr, (char *)pResult, ",", 6, 8) < 6)
                    {
                        log_printf(ZLOG_LEVEL_WARN, "doftp no Matching for IP and PORT in 227!!");
                    }                    
                    /*解析出ip和端口*/			
                    ip= ((unsigned long)(atoi(pResult[0])) << 24) |\
                        ((unsigned long)(atoi(pResult[1])) << 16) |\
                        ((unsigned long)(atoi(pResult[2])) << 8) |\
                        ((unsigned long)(atoi(pResult[3])));
                    port1= ((unsigned short)(atoi(pResult[4])) << 8) |\
                           ((unsigned short) (atoi(pResult[5])));
                    ip_add =ntohl(ip);
                    memcpy(&addr1, &ip_add, 4);
                    p_session->flag2=2;
                    p_session->ip=ip;
                    p_session->port=port1;
                }
                p_session->flag=0;
            }
        }
        /*RETR   下载文件*/
        if (0 == memcmp(p1, "RETR ", 5))
        {
            p2 = (char *)strstr(p1,"\r\n");
            if(p2 != NULL)
            {
                p3=p1;
                p3 += 5;
                len = p2 - p3;
                len = len<128?len:(127);
                memset(file_name,0,sizeof(file_name));
                memcpy(file_name, p3, len);
                file_name[len]='\0';
                memcpy(p_session->file_name,file_name,127);
            }
            p_session->flag=0;
        }
        /*221或421或530     都是失败的命令*/
        if ((0 == memcmp(p1, "221",3))||(memcmp(p1,"421",3) == 0)||(0 == memcmp(p1, "530",3)))
        {
            p_session->flag=0;
        }

    }
}

/*static struct ndpi_flow *get_ndpi_flow6(const struct ndpi_ip6_hdr *iph6,
  u_int16_t ip_offset,
  struct ndpi_id_struct **src,
  struct ndpi_id_struct **dst,
  u_int8_t *proto)
  {
  struct ndpi_iphdr iph;

  memset(&iph, 0, sizeof(iph));
  iph.version = 4;
  iph.saddr = iph6->ip6_src.__u6_addr.__u6_addr32[2] + iph6->ip6_src.__u6_addr.__u6_addr32[3];
  iph.daddr = iph6->ip6_dst.__u6_addr.__u6_addr32[2] + iph6->ip6_dst.__u6_addr.__u6_addr32[3];
  iph.protocol = iph6->ip6_ctlun.ip6_un1.ip6_un1_nxt;
  return(get_ndpi_flow(6, &iph, ip_offset,
  sizeof(struct ndpi_ip6_hdr),
  ntohs(iph6->ip6_ctlun.ip6_un1.ip6_un1_plen),
  src, dst, proto, iph6));
  }*/

/***************************
Function: init_slock_for_sync()
Description: sbuff信号池同步初始化函数
Calls: 
Called By: sbuff_create  --------> main
Table Accessed: 
Table Updated: 
Input: 
int iringbuffer_ID, 信号池ID
int iringbuffer_qlength 信号池中环形缓冲区长度
Output: 成功为0，失败为-1
Return: 成功为0，失败为-1
Others: 
 ***************************/

int init_slock_for_sync(int iringbuffer_ID, int iringbuffer_qlength)//initializing the semaphore lock_object
{
    if(sem_init(&sb_holder[iringbuffer_ID].empty_buffer,0,iringbuffer_qlength) == -1) 
    {
        log_printf(ZLOG_LEVEL_FATAL, "init_slock_sync FATAL: the slock sync is failed!!");
        goto lebel;
    }

    if(sem_init(&sb_holder[iringbuffer_ID].full_buffer,0,0) == -1) 
    {   
        sem_destroy(&sb_holder[iringbuffer_ID].empty_buffer);
        log_printf(ZLOG_LEVEL_FATAL, "init_slock_sync FATAL: the slock sync is failed!!");
        goto lebel;
    }
    return 0; 

lebel : free(sb_holder[iringbuffer_ID].rb_base);//freeing alocated memory on initialisation failure...
        return -1;
}

/***************************
Function: check_for_first_free_sring_buffer()
Description: sring buff中查找空闲的节点
Calls: 
Called By: sbuff_create  --------> main
Table Accessed: 
Table Updated: 
Input: 无
Output: 
Return: 成功为空闲ID号，失败为-1
Others: 
 ***************************/

int check_for_first_free_sring_buffer()
{
    int i;
    for( i =0 ; i < MAX_SRING_BUFFER_COUNT; i++)
    {
        if(sb_holder[i].rb_base == NULL)
        {
            return i; // Gets the first free ring buffer
        }
    }
    log_printf(ZLOG_LEVEL_FATAL, "Check for free sring buff FATAL: there is no free sring buffer!!");
    return -1;
}

/***************************
Function: create_sring_buffer()
Description: 创建sring_buff环形缓冲区
Calls: 
Called By: sbuff_create  --------> main
Table Accessed: 
Table Updated: 
Input: 
int iringbuffer_ID, 信号池ID
int iringbuffer_qlength 信号池环形缓冲区长度
Output: 
Return: 成功为空闲ID号，失败为-1
Others: 
 ***************************/

int  create_sring_buffer(int iringbuffer_ID, int iringbuffer_qlength)
{
    sb_holder[iringbuffer_ID].rb_base = (struct session_buffer_node_tag *)calloc(
            iringbuffer_qlength,
            sizeof(struct session_buffer_node_tag));
    if ( sb_holder[iringbuffer_ID].rb_base == NULL){
        log_printf(ZLOG_LEVEL_FATAL, "create sring buff FATAL: the base mem alloc is failed!!");
        return -1;
    }
    pthread_mutex_init(&sb_holder[iringbuffer_ID].mutex,NULL);
    pthread_mutex_init(&sb_holder[iringbuffer_ID].pmutex,NULL);
    sb_holder[iringbuffer_ID].irb_qlength          = iringbuffer_qlength; 
    sb_holder[iringbuffer_ID].irb_head_index       = 0; 
    sb_holder[iringbuffer_ID].irb_tail_index       = 0;
    sb_holder[iringbuffer_ID].irb_buffer_end_index = iringbuffer_qlength-1; // need justification

    return iringbuffer_ID;//returning buffer header ID
}

/***************************
Function: sbuff_create()
Description: 创建sring_buff环形缓冲区
Calls: check_for_first_free_sring_buffer
create_sring_buffer
init_slock_for_sync
Called By: main
Table Accessed: 
Table Updated: 
Input: 
int irb_qlength 信号池环形缓冲区长度
Output:
Return: 成功为空闲ID号，失败为-1
Others: 
 ***************************/

int sbuff_create (int irb_qlength)
{
    int irb_id = check_for_first_free_sring_buffer(); // Checks for the very first free 
    // ring buffer in the holder array
    if (irb_id == -1)
        return -1; // No Free ring buffers

    int ring_buffer_ID=create_sring_buffer(irb_id, irb_qlength); // Create new ring buffer in the holder with ID received
    if(ring_buffer_ID<0){
        log_printf(ZLOG_LEVEL_FATAL, "Sbuff_create FATAL: there is no buffer_id for using!!");
        return -1; // Failed to create ring buffer
    }
    if((init_slock_for_sync(irb_id, irb_qlength))==-1){
        log_printf(ZLOG_LEVEL_FATAL, "Sbuff_create FATAL: the slock sync is failed!!");
        return -1;       
    }
    return ring_buffer_ID; // Returning the ring buffer ID(index) in success

}

/***************************
Function: sbuff_putdata()
Description: 将已经解析的五元组及其他信息放入sring_buff环形缓冲区中
Calls: 
Called By: session_proc_fun，(do_smtp，do_http_file)--------> tcp_callback，(webmail_mail)--------->http_file_thread，session_consumer_thread
Table Accessed: 
Table Updated: 
Input: 
const int iringbuffer_ID, 使用中的缓冲区ID号
SESSION_BUFFER_NODE ft，五元组及其他信息结构体
Output: 
Return: 成功为1，失败为-1
Others: 
 ***************************/

int sbuff_putdata(const int iringbuffer_ID, SESSION_BUFFER_NODE ft)
{
    if(sem_wait(&sb_holder[iringbuffer_ID].empty_buffer)==-1)//获得缓冲区信号量有效
    {
        log_printf(ZLOG_LEVEL_ERROR, "sbuff sem_wait ERROR: sem_wait is failed to lock empty_buffer!!");
        return -1;
    }
    int temp_index;    	
    pthread_mutex_lock(&sb_holder[iringbuffer_ID].pmutex);//进入临界区操作环形缓冲区数据节点
    temp_index = sb_holder[iringbuffer_ID].irb_tail_index;
    if(sb_holder[iringbuffer_ID].irb_tail_index >= sb_holder[iringbuffer_ID].irb_buffer_end_index )
    {
        sb_holder[iringbuffer_ID].irb_tail_index = 0;
    }
    else
    {
        sb_holder[iringbuffer_ID].irb_tail_index++;
    }
    pthread_mutex_unlock(&sb_holder[iringbuffer_ID].pmutex);
    //相应的环形缓冲区数据赋值
    sb_holder[iringbuffer_ID].rb_base[temp_index].session_five_attr = ft.session_five_attr; //五元组信息赋值
    sb_holder[iringbuffer_ID].rb_base[temp_index].attr = ft.attr;//特殊属性赋值
    sb_holder[iringbuffer_ID].rb_base[temp_index].old_file = ft.old_file;//文件句柄赋值
    memcpy(sb_holder[iringbuffer_ID].rb_base[temp_index].orgname, ft.orgname, 128);//网络抓取原始文件名赋值
    memcpy(sb_holder[iringbuffer_ID].rb_base[temp_index].strname, ft.strname, 128);//网络抓取存储文件名赋值
    if(sem_post(&sb_holder[iringbuffer_ID].full_buffer)==-1)//释放已占用的信号资源，相应的信号量值减少
    {
        log_printf(ZLOG_LEVEL_ERROR, "sbuff sem_post ERROR: failed to unlock in full_buff!!!!");
        return -1;
    }
    return 1;

}

/***************************
Function: sbuff_getdata()
Description: 将从sring_buff环形缓冲区中提取SESSION_BUFFER_NODE结构体信息
Calls: 
Called By: http_file_thread，session_consumer_thread，tika_consumer_thread，
Table Accessed: 
Table Updated: 
Input: const int iringbuffer_ID, 使用中的缓冲区ID号
Output: SESSION_BUFFER_NODE ft，五元组及其他信息结构体
Return: 成功为1，失败为-1
Others: 
 ***************************/

int sbuff_getdata(const int iringbuffer_ID, SESSION_BUFFER_NODE *hdr)
{
    if(sem_wait(&sb_holder[iringbuffer_ID].full_buffer)==-1)//等待信号有效
    {
        log_printf(ZLOG_LEVEL_ERROR, "sbuff get sem_wait ERROR: sem_wait is failed to lock empty_buffer!!");
        //printf("sem_wait is failed to lock full_buffer!!\n");
        return -1;
    }

    if((sb_holder[iringbuffer_ID].rb_base)==NULL){ // Checks for Valid Ring Buffer
        log_printf(ZLOG_LEVEL_ERROR, "sbuff get sem_wait ERROR: the rb_base is NULL!!");
        return -1;
    }

    pthread_mutex_lock(&sb_holder[iringbuffer_ID].mutex);
    //取出已存入结构体的信息数据
    hdr->session_five_attr = sb_holder[iringbuffer_ID].rb_base[sb_holder[iringbuffer_ID].irb_head_index].session_five_attr;
    hdr->old_file = sb_holder[iringbuffer_ID].rb_base[sb_holder[iringbuffer_ID].irb_head_index].old_file;
    hdr->attr = sb_holder[iringbuffer_ID].rb_base[sb_holder[iringbuffer_ID].irb_head_index].attr;
    memcpy(hdr->orgname, sb_holder[iringbuffer_ID].rb_base[sb_holder[iringbuffer_ID].irb_head_index].orgname, 128);
    memcpy(hdr->strname, sb_holder[iringbuffer_ID].rb_base[sb_holder[iringbuffer_ID].irb_head_index].strname, 128);
    if(sb_holder[iringbuffer_ID].irb_head_index >= sb_holder[iringbuffer_ID].irb_qlength -1)
    {
        sb_holder[iringbuffer_ID].irb_head_index=0;
    }
    else        
    {
        sb_holder[iringbuffer_ID].irb_head_index++;
    }

    pthread_mutex_unlock(&sb_holder[iringbuffer_ID].mutex);

    if(sem_post(&sb_holder[iringbuffer_ID].empty_buffer)==-1)
    {
        log_printf(ZLOG_LEVEL_ERROR, "sbuff get sem_post ERROR: sem_post is failed to unlock full_buffer!!");
        return -1;
    }

    return 1; // return size  in terms of byte successfully Written to buffer
}

/***************************
Function: sbuff_destroy()
Description: 销毁sbuff中的数据链
Calls: 
Called By: 
Table Accessed: 
Table Updated: 
Input: const int iringbuffer_ID, 使用中的缓冲区ID号
Output: 
Return: 成功为1，失败为-1
Others: 
 ***************************/

int sbuff_destroy(int iringbuffer_ID)
{
    int i;

    if((sb_holder[iringbuffer_ID].rb_base)==NULL){ // Checks for Valid Ring Buffer
        log_printf(ZLOG_LEVEL_WARN, "sbuff destroy WARN: the rb_base is NULL!!");
        return -1;
    }
    for(i=0; i <sb_holder[iringbuffer_ID].irb_qlength ; i++)
    {     
        sb_holder[iringbuffer_ID].rb_base[i].session_five_attr.ip_src = 0;
        sb_holder[iringbuffer_ID].rb_base[i].session_five_attr.ip_dst = 0;
        sb_holder[iringbuffer_ID].rb_base[i].session_five_attr.port_src = 0;
        sb_holder[iringbuffer_ID].rb_base[i].session_five_attr.port_dst = 0;
        sb_holder[iringbuffer_ID].rb_base[i].session_five_attr.protocol = 0;
        memset(sb_holder[iringbuffer_ID].rb_base[i].orgname,0,128);
        memset(sb_holder[iringbuffer_ID].rb_base[i].strname,0,128);
    }

    sb_holder[iringbuffer_ID].rb_base        = NULL;
    sb_holder[iringbuffer_ID].irb_qlength    = 0; 
    sb_holder[iringbuffer_ID].irb_head_index = 0; 
    sb_holder[iringbuffer_ID].irb_tail_index = 0;
    sem_destroy(&sb_holder[iringbuffer_ID].empty_buffer);    
    sem_destroy(&sb_holder[iringbuffer_ID].full_buffer);    
    return 1; 
}

/***************************
Function: check_for_first_free_ring_buffer()
Description: ring buff中查找空闲的节点
Calls: 
Called By: rbuff_create  --------> main
Table Accessed: 
Table Updated: 
Input: 无
Output: 
Return: 成功为空闲ID号，失败为-1
Others: 
 ***************************/

int check_for_first_free_ring_buffer()
{
    int i;
    for( i =0 ; i < MAX_RING_BUFFER_COUNT; i++)
    {
        if(rb_holder[i].rb_base == NULL)
        {
            return i; // Gets the first free ring buffer
        }
    }
    return -1;
}

/***************************
Function: create_ring_buffer()
Description: 创建ring_buff环形缓冲区
Calls: 
Called By: rbuff_create  --------> main
Table Accessed: 
Table Updated: 
Input: 
int iringbuffer_ID, 信号池ID
int iringbuffer_qlength 信号池环形缓冲区长度
Output: 
Return: 成功为空闲ID号，失败为-1
Others: 
 ***************************/

int  create_ring_buffer(int iringbuffer_ID, int iringbuffer_qlength)
{
    rb_holder[iringbuffer_ID].rb_base = (struct ring_buffer_node_tag*)calloc(
            iringbuffer_qlength,
            sizeof(struct ring_buffer_node_tag));
    if ( rb_holder[iringbuffer_ID].rb_base == NULL){
        log_printf(ZLOG_LEVEL_FATAL, "ring buff create FATAL: the create process is failed!!");
        return -1;
    }
    pthread_mutex_init(&rb_holder[iringbuffer_ID].mutex,NULL);
    rb_holder[iringbuffer_ID].irb_qlength          = iringbuffer_qlength; 
    rb_holder[iringbuffer_ID].irb_head_index       = 0; 
    rb_holder[iringbuffer_ID].irb_tail_index       = 0;
    rb_holder[iringbuffer_ID].irb_buffer_end_index = iringbuffer_qlength-1; // need justification

    return iringbuffer_ID;//returning buffer header ID
}

/***************************
Function: init_lock_for_sync()
Description: rbuff信号池同步初始化函数
Calls: 
Called By: rbuff_create  --------> main
Table Accessed: 
Table Updated: 
Input: 
int iringbuffer_ID, 信号池ID
int iringbuffer_qlength 信号池中环形缓冲区长度
Output: 
Return: 成功为0，失败为-1
Others: 
 ***************************/

int init_lock_for_sync(int iringbuffer_ID, int iringbuffer_qlength)//initializing the semaphore lock_object
{
    if(sem_init(&rb_holder[iringbuffer_ID].empty_buffer,0,iringbuffer_qlength) == -1) 
    {
        log_printf(ZLOG_LEVEL_FATAL, "ring buff sync FATAL: the sync process is failed!!");
        goto lebel;
    }

    if(sem_init(&rb_holder[iringbuffer_ID].full_buffer,0,0) == -1) 
    {   
        sem_destroy(&rb_holder[iringbuffer_ID].empty_buffer);
        log_printf(ZLOG_LEVEL_FATAL, "ring buff sem init FATAL: the sem init process is failed!!");
        goto lebel;
    }
    return 0; 
lebel : free(rb_holder[iringbuffer_ID].rb_base);//freeing alocated memory on initialisation failure...
        return -1;
}

/***************************
Function: rbuff_create()
Description: 创建ring_buff环形缓冲区
Calls: check_for_first_free_ring_buffer
create_ring_buffer
init_lock_for_sync
Called By: main
Table Accessed: 
Table Updated: 
Input: 
int irb_qlength 信号池环形缓冲区长度
Output:
Return: 成功为空闲ID号，失败为-1
Others: 
 ***************************/

int rbuff_create (int irb_qlength)
{
    int irb_id = check_for_first_free_ring_buffer(); // Checks for the very first free 
    // ring buffer in the holder array
    if (irb_id == -1){
        log_printf(ZLOG_LEVEL_FATAL, "rbuff create FATAL: there is no id for using!!");
        return -1; // No Free ring buffers
    }

    int ring_buffer_ID=create_ring_buffer(irb_id, irb_qlength); // Create new ring buffer in the holder with ID received
    if(ring_buffer_ID<0){
        log_printf(ZLOG_LEVEL_FATAL, "rbuff create FATAL: there is no buff id for create!!");
        return -1; // Failed to create ring buffer
    }
    if((init_lock_for_sync(irb_id, irb_qlength))==-1){
        log_printf(ZLOG_LEVEL_FATAL, "rbuff create FATAL: the sync process is failed!!");
        return -1;       
    }
    return ring_buffer_ID; // Returning the ring buffer ID(index) in success

}

/**Putting data on ring buffer**/
/***************************
Function: rbuff_putdata()
Description: 将已经解析的五元组及数据包信息放入ring_buff环形缓冲区中
Calls: 
Called By: dummyProcessPacket，packet_consumer_thread
Table Accessed: 
Table Updated: 
Input: 
const int iringbuffer_ID, 使用的缓冲区ID号
struct tuple5 *session_five_attr  五元组及其他信息结构体
struct pcap_pkthdr *hdr，数据包头部信息  
void *pData，采集的数据包
Output: 
Return: 成功为1，失败为-1
Others: 
 ***************************/

int rbuff_putdata(const int iringbuffer_ID, void *pData, struct pcap_pkthdr *hdr, struct tuple5 *session_five_attr)
{
    if(sem_wait(&rb_holder[iringbuffer_ID].empty_buffer)==-1)//等待信号量有效
    {
        log_printf(ZLOG_LEVEL_ERROR, "ring buff put ERROR: the put data process pointer is NULL!!");
        return -1;
    }
    struct tuple5  five_attr;
    struct pcap_pkthdr  temp_hdr;
    temp_hdr.len = hdr->len;
    temp_hdr.caplen = hdr->caplen;
    temp_hdr.ts.tv_sec = hdr->ts.tv_sec;
    temp_hdr.ts.tv_usec = hdr->ts.tv_usec;
    if(session_five_attr != NULL){//五元组信息有效性赋值
        five_attr.four_attr.saddr = session_five_attr->four_attr.saddr;
        five_attr.four_attr.daddr = session_five_attr->four_attr.daddr;
        five_attr.four_attr.source = session_five_attr->four_attr.source;
        five_attr.four_attr.dest= session_five_attr->four_attr.dest;
        five_attr.protocol = session_five_attr->protocol;
        //five_attr.flow_flag = session_five_attr->flow_flag;
        //five_attr.flow = session_five_attr->flow;
    }

    if((temp_hdr.caplen>0)&&(temp_hdr.caplen<=65536)){//判断采集数据包是否有效
        rb_holder[iringbuffer_ID].rb_base[rb_holder[iringbuffer_ID].irb_tail_index].pdata_buffer = ikmem_malloc(temp_hdr.caplen);// Allocating memory
        if(rb_holder[iringbuffer_ID].rb_base[rb_holder[iringbuffer_ID].irb_tail_index].pdata_buffer == NULL){ // Checks Memory  Allocated
            log_printf(ZLOG_LEVEL_ERROR, "ring buff put ERROR: the put packet data pointer is NULL!!");
            return -1;
        }else{
            if(memcpy(rb_holder[iringbuffer_ID].rb_base[rb_holder[iringbuffer_ID].irb_tail_index].pdata_buffer,pData,temp_hdr.caplen)==NULL)//转存数据
            {   
                ikmem_free(rb_holder[iringbuffer_ID].rb_base[rb_holder[iringbuffer_ID].irb_tail_index].pdata_buffer);//释放之前内存
                rb_holder[iringbuffer_ID].rb_base=NULL;
                log_printf(ZLOG_LEVEL_ERROR, "ring buff put ERROR: the packet copy is failed!!");
                return -1;
            }
        }
    }
    else{
        log_printf(ZLOG_LEVEL_ERROR, "ring buff put ERROR: the put packet len is over flow, the packet len is %d!!", temp_hdr.len);
        return -1;    
    }            
    //赋值数据包头信息
    rb_holder[iringbuffer_ID].rb_base[rb_holder[iringbuffer_ID].irb_tail_index].pkthdr.len = temp_hdr.len;//数据包长度
    rb_holder[iringbuffer_ID].rb_base[rb_holder[iringbuffer_ID].irb_tail_index].pkthdr.caplen = temp_hdr.caplen;//采集数据包长度
    rb_holder[iringbuffer_ID].rb_base[rb_holder[iringbuffer_ID].irb_tail_index].pkthdr.ts.tv_sec = temp_hdr.ts.tv_sec;//采集时间(秒)
    rb_holder[iringbuffer_ID].rb_base[rb_holder[iringbuffer_ID].irb_tail_index].pkthdr.ts.tv_usec = temp_hdr.ts.tv_usec;//采集时间(微妙)
    if(session_five_attr != NULL)//五元组信息有效性赋值
    {
        rb_holder[iringbuffer_ID].rb_base[rb_holder[iringbuffer_ID].irb_tail_index].session_five_attr.ip_src = five_attr.four_attr.saddr;
        rb_holder[iringbuffer_ID].rb_base[rb_holder[iringbuffer_ID].irb_tail_index].session_five_attr.ip_dst = five_attr.four_attr.daddr;
        rb_holder[iringbuffer_ID].rb_base[rb_holder[iringbuffer_ID].irb_tail_index].session_five_attr.port_src= five_attr.four_attr.source;
        rb_holder[iringbuffer_ID].rb_base[rb_holder[iringbuffer_ID].irb_tail_index].session_five_attr.port_dst= five_attr.four_attr.dest;
        rb_holder[iringbuffer_ID].rb_base[rb_holder[iringbuffer_ID].irb_tail_index].session_five_attr.protocol = five_attr.protocol;
        //rb_holder[iringbuffer_ID].rb_base[rb_holder[iringbuffer_ID].irb_tail_index].session_five_attr.flow_flag = five_attr.flow_flag;
        //rb_holder[iringbuffer_ID].rb_base[rb_holder[iringbuffer_ID].irb_tail_index].session_five_attr.flow = five_attr.flow;
    }
    //修改环形缓冲区地址
    if(rb_holder[iringbuffer_ID].irb_tail_index >= rb_holder[iringbuffer_ID].irb_buffer_end_index )
    {
        rb_holder[iringbuffer_ID].irb_tail_index = 0;
    }
    else
    {
        rb_holder[iringbuffer_ID].irb_tail_index++;
    }

    if(sem_post(&rb_holder[iringbuffer_ID].full_buffer)==-1)//释放占用的信号量
    {
        log_printf(ZLOG_LEVEL_ERROR, "ring buff put ERROR: sem_post is failed to unlock in full_buffer!!");
        return -1;
    }
    return 1;
}

/**Getting data from ring buffer**/
/***************************
Function: rbuff_getdata()
Description: 将从ring_buff环形缓冲区中提取数据包以及相应的头信息和五元组信息
Calls: 
Called By: session_process_thread，packet_consumer_thread
Table Accessed: 
Table Updated: 
Input: const int iringbuffer_ID, 使用的缓冲区ID号
Output: void *pData  缓冲区中的数据
struct pcap_pkthdr *hdr  数据包头部信息
struct tuple5 *session_five_attr  五元组信息
Return: 成功为1，失败为-1
Others: 
 ***************************/

int rbuff_getdata(const int iringbuffer_ID, void *pData, struct pcap_pkthdr *hdr, struct tuple5 *session_five_attr)
{
    if(sem_wait(&rb_holder[iringbuffer_ID].full_buffer)==-1)//等待系统信号量
    {
        if(rb_holder[iringbuffer_ID].rb_base[rb_holder[iringbuffer_ID].irb_head_index].pdata_buffer != NULL)//如果等待失效，释放资源
        {
            ikmem_free(rb_holder[iringbuffer_ID].rb_base[rb_holder[iringbuffer_ID].irb_head_index].pdata_buffer);
        }
        log_printf(ZLOG_LEVEL_ERROR, "ring buff get ERROR: sem_wait is failed to lock in full_buffer!!");
        return -1;
    }

    if((rb_holder[iringbuffer_ID].rb_base)==NULL){ // Checks for Valid Ring Buffer
        log_printf(ZLOG_LEVEL_ERROR, "ring buff get ERROR: the rb_base pointer is NULL!!");
        return -1;
    }

    // 判断数据包数据的有效性
    if((rb_holder[iringbuffer_ID].rb_base[rb_holder[iringbuffer_ID].irb_head_index].pkthdr.caplen>0)&&(rb_holder[iringbuffer_ID].rb_base[rb_holder[iringbuffer_ID].irb_head_index].pkthdr.caplen<=65536))
    {
        if(rb_holder[iringbuffer_ID].rb_base[rb_holder[iringbuffer_ID].irb_head_index].pdata_buffer != NULL)
        {
            if(NULL == memcpy(pData,rb_holder[iringbuffer_ID].rb_base[rb_holder[iringbuffer_ID].irb_head_index].pdata_buffer,
                        rb_holder[iringbuffer_ID].rb_base[rb_holder[iringbuffer_ID].irb_head_index].pkthdr.caplen)){//转存数据包
                ikmem_free(rb_holder[iringbuffer_ID].rb_base[rb_holder[iringbuffer_ID].irb_head_index].pdata_buffer);
                rb_holder[iringbuffer_ID].rb_base[rb_holder[iringbuffer_ID].irb_head_index].pdata_buffer = NULL;
                log_printf(ZLOG_LEVEL_ERROR, "ring buff get ERROR: copy data is failed!!");
                return -1;
            }
        }
        else{
            log_printf(ZLOG_LEVEL_ERROR, "ring buff get ERROR: the pdata buff pointer is NULL!!");
            return -1;
        }
    }    
    else{
        if(rb_holder[iringbuffer_ID].rb_base[rb_holder[iringbuffer_ID].irb_head_index].pdata_buffer != NULL)
        {
            ikmem_free(rb_holder[iringbuffer_ID].rb_base[rb_holder[iringbuffer_ID].irb_head_index].pdata_buffer);
        }
        log_printf(ZLOG_LEVEL_ERROR, "ring buff get ERROR: the packet caplen is %d!!",rb_holder[iringbuffer_ID].rb_base[rb_holder[iringbuffer_ID].irb_head_index].pkthdr.caplen);
        return -1;
    }
    //转存数据包头信息
    hdr->len = rb_holder[iringbuffer_ID].rb_base[rb_holder[iringbuffer_ID].irb_head_index].pkthdr.len;
    hdr->caplen = rb_holder[iringbuffer_ID].rb_base[rb_holder[iringbuffer_ID].irb_head_index].pkthdr.caplen;
    hdr->ts.tv_sec = rb_holder[iringbuffer_ID].rb_base[rb_holder[iringbuffer_ID].irb_head_index].pkthdr.ts.tv_sec;
    hdr->ts.tv_usec = rb_holder[iringbuffer_ID].rb_base[rb_holder[iringbuffer_ID].irb_head_index].pkthdr.ts.tv_usec;
    //转存数据包五元组信息
    if(session_five_attr != NULL)
    {
        session_five_attr->four_attr.saddr= rb_holder[iringbuffer_ID].rb_base[rb_holder[iringbuffer_ID].irb_head_index].session_five_attr.ip_src;
        session_five_attr->four_attr.dest= rb_holder[iringbuffer_ID].rb_base[rb_holder[iringbuffer_ID].irb_head_index].session_five_attr.port_dst;
        session_five_attr->four_attr.source= rb_holder[iringbuffer_ID].rb_base[rb_holder[iringbuffer_ID].irb_head_index].session_five_attr.port_src;
        session_five_attr->four_attr.daddr= rb_holder[iringbuffer_ID].rb_base[rb_holder[iringbuffer_ID].irb_head_index].session_five_attr.ip_dst;
        session_five_attr->protocol = rb_holder[iringbuffer_ID].rb_base[rb_holder[iringbuffer_ID].irb_head_index].session_five_attr.protocol;
        //session_five_attr->flow_flag = rb_holder[iringbuffer_ID].rb_base[rb_holder[iringbuffer_ID].irb_head_index].session_five_attr.flow_flag;
        //session_five_attr->flow = rb_holder[iringbuffer_ID].rb_base[rb_holder[iringbuffer_ID].irb_head_index].session_five_attr.flow;
    }
    //释放之前资源
    ikmem_free(rb_holder[iringbuffer_ID].rb_base[rb_holder[iringbuffer_ID].irb_head_index].pdata_buffer);
    rb_holder[iringbuffer_ID].rb_base[rb_holder[iringbuffer_ID].irb_head_index].pdata_buffer = NULL;
    rb_holder[iringbuffer_ID].rb_base[rb_holder[iringbuffer_ID].irb_head_index].pkthdr.len = 0;
    rb_holder[iringbuffer_ID].rb_base[rb_holder[iringbuffer_ID].irb_head_index].pkthdr.caplen = 0;
    rb_holder[iringbuffer_ID].rb_base[rb_holder[iringbuffer_ID].irb_head_index].pkthdr.ts.tv_sec = 0;
    rb_holder[iringbuffer_ID].rb_base[rb_holder[iringbuffer_ID].irb_head_index].pkthdr.ts.tv_usec = 0;
    //updating head pointer
    if(rb_holder[iringbuffer_ID].irb_head_index >= rb_holder[iringbuffer_ID].irb_buffer_end_index)//修改环形缓冲区地址指针
    {
        rb_holder[iringbuffer_ID].irb_head_index=0;
    }
    else        
    {
        rb_holder[iringbuffer_ID].irb_head_index++;
    }

    if(sem_post(&rb_holder[iringbuffer_ID].empty_buffer)==-1)//释放系统信号量
    {
        log_printf(ZLOG_LEVEL_ERROR, "ring buff get ERROR: sem_post is failed to unlock in empty_buffer!!");
        return -1;
    }
    return 1; // return size  in terms of byte successfully Written to buffer
}

/**Destroying Ring Buffer**/
/***************************
Function: rbuff_destroy()
Description: 销毁buff中的数据链
Calls: 
Called By: 
Table Accessed: 
Table Updated: 
Input: const int iringbuffer_ID, 使用的缓冲区ID号
Output: 
Return: 成功为1，失败为-1
Others: 
 ***************************/

int rbuff_destroy(int iringbuffer_ID)
{
    int i;

    if((rb_holder[iringbuffer_ID].rb_base)==NULL){ // Checks for Valid Ring Buffer
        log_printf(ZLOG_LEVEL_WARN, "ring buff destroy WARN: the rb_base pointer is NULL!!");
        return -1;
    }

    for(i=0; i <rb_holder[iringbuffer_ID].irb_qlength ; i++)
    {
        if(rb_holder[iringbuffer_ID].rb_base[i].pdata_buffer != NULL)
        {
            ikmem_free(rb_holder[iringbuffer_ID].rb_base[i].pdata_buffer);
        }
    }

    rb_holder[iringbuffer_ID].rb_base        = NULL;
    rb_holder[iringbuffer_ID].irb_qlength    = 0; 
    rb_holder[iringbuffer_ID].irb_head_index = 0; 
    rb_holder[iringbuffer_ID].irb_tail_index = 0;
    sem_destroy(&rb_holder[iringbuffer_ID].empty_buffer);    
    sem_destroy(&rb_holder[iringbuffer_ID].full_buffer);    
    return 1; 
}

/****ring_buffer****/
/***************************
Function: nids_ip_filter()
Description: nids中ip过滤
Calls: 
Called By: 
Table Accessed: 
Table Updated: 
Input:  *x   ip结构体
len  长度
Output: 
Return: 成功为1，失败为-1
Others: 
 ***************************/

static int nids_ip_filter(struct ip *x, int len)
{
    (void)x;
    (void)len;
    return 1;
}

/***************************
Function: nids_syslog()
Description: nids中运行过程中的log记录
Calls: 
Called By: 
Table Accessed: 
Table Updated: 
Input:  type   日志类型
errnum 错误号
iph    ip信息
data   数据实体
Output: 
Return: 无
Others: 
 ***************************/

static void nids_syslog(int type, int errnum, struct ip *iph, void *data)
{
    char saddr[20], daddr[20];
    char buf[1024];
    struct host *this_host;
    unsigned char flagsand = 255, flagsor = 0;
    int i;

    switch (type) {

        case NIDS_WARN_IP:
            if (errnum != NIDS_WARN_IP_HDR) {
                strcpy(saddr, int_ntoa(iph->ip_src.s_addr));
                strcpy(daddr, int_ntoa(iph->ip_dst.s_addr));
                syslog(nids_params.syslog_level,
                        "%s, packet (apparently) from %s to %s\n",
                        nids_warnings[errnum], saddr, daddr);
            } else
                syslog(nids_params.syslog_level, "%s\n",
                        nids_warnings[errnum]);
            break;

        case NIDS_WARN_TCP:
            strcpy(saddr, int_ntoa(iph->ip_src.s_addr));
            strcpy(daddr, int_ntoa(iph->ip_dst.s_addr));
            if (errnum != NIDS_WARN_TCP_HDR)
                syslog(nids_params.syslog_level,
                        "%s,from %s to %s\n", nids_warnings[errnum],
                        saddr,  daddr);
            else
                syslog(nids_params.syslog_level, "%s,from %s to %s\n",
                        nids_warnings[errnum], saddr, daddr);
            break;

        case NIDS_WARN_SCAN:
            this_host = (struct host *) data;
            sprintf(buf, "Scan from %s. Scanned ports: ",
                    int_ntoa(this_host->addr));
            for (i = 0; i < this_host->n_packets; i++) {
                strcat(buf, int_ntoa(this_host->packets[i].addr));
                sprintf(buf + strlen(buf), ":%hu,",
                        this_host->packets[i].port);
                flagsand &= this_host->packets[i].flags;
                flagsor |= this_host->packets[i].flags;
            }
            if (flagsand == flagsor) {
                i = flagsand;
                switch (flagsand) {
                    case 2:
                        strcat(buf, "scan type: SYN");
                        break;
                    case 0:
                        strcat(buf, "scan type: NULL");
                        break;
                    case 1:
                        strcat(buf, "scan type: FIN");
                        break;
                    default:
                        sprintf(buf + strlen(buf), "flags=0x%x", i);
                }
            } else
                strcat(buf, "various flags");
            syslog(nids_params.syslog_level, "%s", buf);
            break;

        default:
            syslog(nids_params.syslog_level, "Unknown warning number ?\n");
    }
}

/***************************
Function: call_ip_frag_procs()
Description: ip数据包处理函数，通过此函数调用nids注册函数对ip数据包的深层整理函数
Calls: 
Called By: nids_pcap_handler
Table Accessed: 
Table Updated: 
Input:  data   网络数据包
caplen 数据包大小
hdr    数据包抓取信息
Five_attr   五元组信息
Output: 无
Return: 无
Others: 
 ***************************/

static void call_ip_frag_procs(void *data,bpf_u_int32 caplen, struct pcap_pkthdr *hdr, struct tuple5 *Five_attr)
{
    struct proc_node *i;
    for (i = ip_frag_procs; i; i = i->next)//轮询注册函数链表，提取处理函数
        (i->item) (data, caplen, hdr, Five_attr);
}

/* wireless frame types, mostly from tcpdump (wam) */
#define FC_TYPE(fc)             (((fc) >> 2) & 0x3)
#define FC_SUBTYPE(fc)          (((fc) >> 4) & 0xF)
#define DATA_FRAME_IS_QOS(x)    ((x) & 0x08)
#define FC_WEP(fc)              ((fc) & 0x4000)
#define FC_TO_DS(fc)            ((fc) & 0x0100)
#define FC_FROM_DS(fc)          ((fc) & 0x0200)
#define T_MGMT 0x0		/* management */
#define T_CTRL 0x1		/* control */
#define T_DATA 0x2		/* data */
#define T_RESV 0x3		/* reserved */
#define EXTRACT_LE_16BITS(p) \
    ((unsigned short)*((const unsigned char *)(p) + 1) << 8 | \
     (unsigned short)*((const unsigned char *)(p) + 0))
#define EXTRACT_16BITS(p)	((unsigned short)ntohs(*(const unsigned short *)(p)))
#define LLC_FRAME_SIZE 8
#define LLC_OFFSET_TO_TYPE_FIELD 6
#define ETHERTYPE_IP 0x0800

/***************************
  Function: nids_pcap_handler()
  Description: 会话重组处理函数，通过此函数调用TCP数据流的会话重组
  Calls: 
  Called By: session_process_thread
  Table Accessed: 
  Table Updated: 
  Input:  data   网络数据包
  caplen 数据包大小
  hdr    数据包抓取信息
  Five_attr   五元组信息
  Output: 无
  Return: 无
  Others: 
 ***************************/

void nids_pcap_handler(u_char * par, struct pcap_pkthdr *hdr, u_char * data, struct tuple5 *Five_attr)
{
    u_char *data_aligned;
#ifdef HAVE_LIBGTHREAD_2_0
    struct cap_queue_item *qitem;
#endif
#ifdef DLT_IEEE802_11
    unsigned short fc;
    int linkoffset_tweaked_by_prism_code = 0;
    int linkoffset_tweaked_by_radio_code = 0;
#endif
    /*
     * Check for savagely closed TCP connections. Might
     * happen only when nids_params.tcp_workarounds is non-zero;
     * otherwise nids_tcp_timeouts is always NULL.
     */
    if (NULL != nids_tcp_timeouts)
        tcp_check_timeouts(&hdr->ts);
    nids_last_pcap_data = data;
    (void)par; /* warnings... */
    switch (linktype) {//判断硬件系统连接类型
        case DLT_EN10MB://当前系统正常使用的连接类型，根据此类型初始化参数
            if (hdr->caplen < 14)
                return;
            /* Only handle IP packets and 802.1Q VLAN tagged packets below. */
            if (data[12] == 8 && data[13] == 0) {
                /* Regular ethernet */
                nids_linkoffset = 14;
            } else if (data[12] == 0x81 && data[13] == 0) {
                /* Skip 802.1Q VLAN and priority information */
                nids_linkoffset = 18;
            } else
                /* non-ip frame */
                return;
            break;
#ifdef DLT_PRISM_HEADER
#ifndef DLT_IEEE802_11
#error DLT_PRISM_HEADER is defined, but DLT_IEEE802_11 is not ???
#endif
        case DLT_PRISM_HEADER:
            nids_linkoffset = 144;
            linkoffset_tweaked_by_prism_code = 1;
            //now let DLT_IEEE802_11 do the rest
#endif
#ifdef DLT_IEEE802_11_RADIO
        case DLT_IEEE802_11_RADIO:
            // just get rid of the radio tap header
            if (!linkoffset_tweaked_by_prism_code) {
                nids_linkoffset = EXTRACT_LE_16BITS(data + 2); // skip radiotap header
                linkoffset_tweaked_by_radio_code = 1;
            }
            //now let DLT_IEEE802_11 do the rest
#endif
#ifdef DLT_IEEE802_11
        case DLT_IEEE802_11:
            /* I don't know why frame control is always little endian, but it 
             * works for tcpdump, so who am I to complain? (wam)
             */
            if (!linkoffset_tweaked_by_prism_code && !linkoffset_tweaked_by_radio_code)
                nids_linkoffset = 0;
            fc = EXTRACT_LE_16BITS(data + nids_linkoffset);
            if (FC_TYPE(fc) != T_DATA || FC_WEP(fc)) {
                return;
            }
            if (FC_TO_DS(fc) && FC_FROM_DS(fc)) {
                /* a wireless distribution system packet will have another
                 * MAC addr in the frame
                 */
                nids_linkoffset += 30;
            } else {
                nids_linkoffset += 24;
            }
            if (DATA_FRAME_IS_QOS(FC_SUBTYPE(fc)))
                nids_linkoffset += 2;
            if (hdr->len < nids_linkoffset + LLC_FRAME_SIZE)
                return;
            if (ETHERTYPE_IP !=
                    EXTRACT_16BITS(data + nids_linkoffset + LLC_OFFSET_TO_TYPE_FIELD)) {
                /* EAP, LEAP, and other 802.11 enhancements can be 
                 * encapsulated within a data packet too.  Look only at
                 * encapsulated IP packets (Type field of the LLC frame).
                 */
                return;
            }
            nids_linkoffset += LLC_FRAME_SIZE;
            break;
#endif
        default:;
    }
    if (hdr->caplen < nids_linkoffset)
        return;

    /*
     * sure, memcpy costs. But many EXTRACT_{SHORT, LONG} macros cost, too. 
     * Anyway, libpcap tries to ensure proper layer 3 alignment (look for
     * handle->offset in pcap sources), so memcpy should not be called.
     */
#ifdef LBL_ALIGN
    if ((unsigned long) (data + nids_linkoffset) & 0x3) {
        data_aligned = alloca(hdr->caplen - nids_linkoffset + 4);
        data_aligned -= (unsigned long) data_aligned % 4;
        memcpy(data_aligned, data + nids_linkoffset, hdr->caplen - nids_linkoffset);
    } else 
#endif
        data_aligned = data + nids_linkoffset;

#ifdef HAVE_LIBGTHREAD_2_0
    if(nids_params.multiproc) { //如果编译libgthread2.0的话，开启多线程处理功能
        /* 
         * Insert received fragment into the async capture queue.
         * We hope that the overhead of memcpy 
         * will be saturated by the benefits of SMP - mcree
         */
        qitem=malloc(sizeof(struct cap_queue_item));
        if (qitem && (qitem->data=malloc(hdr->caplen - nids_linkoffset))) {
            qitem->caplen=hdr->caplen - nids_linkoffset;
            memcpy(qitem->data,data_aligned,qitem->caplen);
            g_async_queue_lock(cap_queue);
            /* ensure queue does not overflow */
            if(g_async_queue_length_unlocked(cap_queue) > nids_params.queue_limit) {
                /* queue limit reached: drop packet - should we notify user via syslog? */
                free(qitem->data);
                free(qitem);
            } else {
                /* insert packet to queue */
                g_async_queue_push_unlocked(cap_queue,qitem);
            }
            g_async_queue_unlock(cap_queue);
        }
    } else { /* user requested simple passthru - no threading，本系统采用单线程处理方式 */
        call_ip_frag_procs(data_aligned,hdr->len - nids_linkoffset, hdr, Five_attr);
    }
#else
    call_ip_frag_procs(data_aligned, hdr->len - nids_linkoffset, hdr, Five_attr);
#endif
}

/***************************
Function: gen_ip_frag_proc()
Description: ip数据包处理函数，判断数据包是否为网络上的碎片包，如果是碎片包进行碎片整理，如果不是，调用会话重组函数进行会话数据重组
Calls: 
Called By: call_ip_frag_procs
Table Accessed: 
Table Updated: 
Input:  data   网络数据包
len 数据包大小
hdr    数据包抓取信息
Five_attr   五元组信息
Output: 无
Return: 无
Others: 
 ***************************/

static void gen_ip_frag_proc(u_char * data, int len, struct pcap_pkthdr *hdr, struct tuple5 *Five_attr)
{
    struct proc_node *i;
    struct ip *iph = (struct ip *) data;
    int need_free = 0;
    int skblen;
    void (*glibc_syslog_h_workaround)(int, int, struct ip *, void*)=
        nids_params.syslog;
    //首先进行ip过滤
    if (!nids_params.ip_filter(iph, len))
        return;
    //判断数据包信息是否符合标准，如果不符则跳出处理过程
    if (len < (int)sizeof(struct ip) || iph->ip_hl < 5 || iph->ip_v != 4 ||
            ip_fast_csum((unsigned char *) iph, iph->ip_hl) != 0 ||
            len < ntohs(iph->ip_len) || ntohs(iph->ip_len) < iph->ip_hl << 2) {
        glibc_syslog_h_workaround(NIDS_WARN_IP, NIDS_WARN_IP_HDR, iph, 0);
        return;
    }
    if (iph->ip_hl > 5 && ip_options_compile((unsigned char *)data)) {
        glibc_syslog_h_workaround(NIDS_WARN_IP, NIDS_WARN_IP_SRR, iph, 0);
        return;
    }
    switch (ip_defrag_stub((struct ip *) data, &iph)) {//判断符合的数据包是否为新的网络碎片
        case IPF_ISF:
            return;
        case IPF_NOTF:
            need_free = 0;
            iph = (struct ip *) data;
            break;
        case IPF_NEW:
            log_printf(ZLOG_LEVEL_INFO, "ip frag INFO: there is new fragment!!");
            //printf("There is new fragment!!!\n");
            need_free = 1;
            break;
        default:;
    }
    skblen = ntohs(iph->ip_len) + 16;
    if (!need_free)//如果是碎片，进行参数的修改
        skblen += nids_params.dev_addon;
    skblen = (skblen + 15) & ~15;
    skblen += nids_params.sk_buff_size;

    for (i = ip_procs; i; i = i->next)//调用TCP重组函数进行会话重组
        (i->item) (iph, skblen, hdr, Five_attr);
    if (need_free)
        free(iph);
}

#if HAVE_BSD_UDPHDR
#define UH_ULEN uh_ulen
#define UH_SPORT uh_sport
#define UH_DPORT uh_dport
#else
#define UH_ULEN len
#define UH_SPORT source
#define UH_DPORT dest
#endif
/***************************
Function: gen_ip_proc()
Description: 会话重组处理函数，通过此函数调用TCP数据流的会话重组
Calls: 
Called By: gen_ip_frag_proc
Table Accessed: 
Table Updated: 
Input:  data   网络数据包
caplen 数据包大小
hdr    数据包抓取信息
Five_attr   五元组信息
Output: 无
Return: 无
Others: 
 ***************************/

static void gen_ip_proc(u_char * data, int skblen, struct pcap_pkthdr *hdr, struct tuple5 *Five_attr)
{
    switch (((struct ip *) data)->ip_p) {
        case IPPROTO_TCP:  //TCP数据包重组
            process_tcp(data, skblen, hdr, Five_attr);
            break;
        default:
            break;
    }
}

/***************************
Function: init_procs()
Description: ip数据包处理函数链表初始化，主要将后续需要调用的处理函数添加到相应的链表节点中
Calls: 
Called By: nids_init
Table Accessed: 
Table Updated: 
Input: 无
Output: 无
Return: 无
Others: 
 ***************************/

static void init_procs()
{
    ip_frag_procs = mknew(struct proc_node);
    ip_frag_procs->item = gen_ip_frag_proc;
    ip_frag_procs->next = 0;
    ip_procs = mknew(struct proc_node);
    ip_procs->item = gen_ip_proc;
    ip_procs->next = 0;
    tcp_procs = 0;
    udp_procs = 0;
}

/***************************
Function: nids_register_udp()
Description: 注册udp函数
Calls: 
Called By: nids_init
Table Accessed: 
Table Updated: 
Input: *x  入口函数
Output: 无
Return: 无
Others: 
 ***************************/

void nids_register_udp(void (*x))
{
    register_callback(&udp_procs, x);
}

/***************************
Function: nids_unregister_udp()
Description: 卸载udp函数
Calls: 
Called By: nids_init
Table Accessed: 
Table Updated: 
Input: *x  入口函数
Output: 无
Return: 无
Others: 
 ***************************/

void nids_unregister_udp(void (*x))
{
    unregister_callback(&udp_procs, x);
}

/***************************
Function: nids_register_ip()
Description: 注册ip数据包处理函数
Calls: 
Called By: nids_init
Table Accessed: 
Table Updated: 
Input: *x  入口函数
Output: 无
Return: 无
Others: 
 ***************************/

void nids_register_ip(void (*x))
{
    register_callback(&ip_procs, x);
}

/***************************
Function: nids_unregister_ip()
Description: 卸载ip数据包处理函数
Calls: 
Called By: nids_init
Table Accessed: 
Table Updated: 
Input: *x  入口函数
Output: 无
Return: 无
Others: 
 ***************************/

void nids_unregister_ip(void (*x))
{
    unregister_callback(&ip_procs, x);
}

/***************************
Function: nids_register_ip_frag()
Description: 注册ip数据包碎片处理函数
Calls: 
Called By: nids_init
Table Accessed: 
Table Updated: 
Input: *x  入口函数
Output: 无
Return: 无
Others: 
 ***************************/

void nids_register_ip_frag(void (*x))
{
    register_callback(&ip_frag_procs, x);
}

/***************************
Function: nids_unregister_ip_frag()
Description: 卸载ip数据包碎片处理函数
Calls: 
Called By: nids_init
Table Accessed: 
Table Updated: 
Input: *x  入口函数
Output: 无
Return: 无
Others: 
 ***************************/


void nids_unregister_ip_frag(void (*x))
{
    unregister_callback(&ip_frag_procs, x);
}

/***************************
Function: nids_init()
Description: nids初始化函数
Calls: init_procs();//处理函数初始化
tcp_init(nids_params.n_tcp_streams);//tcp处理过程的初始化函数
ip_frag_init(nids_params.n_hosts);//主机参数初始化
scan_init();
Called By: main
Table Accessed: 
Table Updated: 
Input: 无
Output: 无
Return: 无
Others: 
 ***************************/

int nids_init()
{
    /* free resources that previous usages might have allocated */
    tcp_exit();
    ip_frag_exit();
    scan_exit();
    free(ip_procs);
    free(ip_frag_procs);

    if (nids_params.pcap_desc)
        desc = nids_params.pcap_desc;

    //根据物理链接类型初始化参数
    linktype = DLT_EN10MB;
    nids_linkoffset = 14;
    nids_params.dev_addon = 16;

    if (nids_params.syslog == nids_syslog)//开启nidslog记录文件
        openlog("libnids", 0, LOG_LOCAL0);

    init_procs();//处理函数初始化
    tcp_init(nids_params.n_tcp_streams);//tcp处理过程的初始化函数
    ip_frag_init(nids_params.n_hosts);//主机参数初始化
    scan_init();

    return 1;
}

/**********************************/
// struct tuple4 contains addresses and port numbers of the TCP connections
// the following auxiliary function produces a string looking like
// 10.0.0.1,1024,10.0.0.2,23
/***************************
Function: adres()
Description: ip地址转换函数，将数据结构中的ip地址转换为*.*.*.*,*格式
Calls: 
Called By: tcp_callback
Table Accessed: 
Table Updated: 
Input: 无
Output: 无
Return: 无
Others: 
 ***************************/
    char *
adres (struct tuple4 addr)
{
    static char buf[256];
    strcpy (buf, int_ntoa (addr.saddr));
    sprintf (buf + strlen (buf), "_%i_", addr.source);
    strcat (buf, int_ntoa (addr.daddr));
    sprintf (buf + strlen (buf), "_%i", addr.dest);
    return buf;
}

/***************************
Function: session_proc_fun()
Description: tcp会话处理函数，主要处理http、smtp以及ftp三种协议的文件处理以及http链接关闭部分部分
Calls: http，gmime_parse_mailbody
Called By: session_consumer_thread
Table Accessed: 
Table Updated: 
Input: SESSION_BUFFER_NODE *arg
Output: SESSION_BUFFER_NODE *hdr
Return: 数据处理返回1，其他处理返回0
Others: 
 ***************************/

uint16_t  session_num=0, ftp_num=0;
int session_proc_fun(SESSION_BUFFER_NODE *arg, SESSION_BUFFER_NODE *hdr)
{
    //	  log_printf(ZLOG_LEVEL_INFO, "\n\n################## session_proc_fun ===========================111=\n\n");
    four_tuple_t temp_f;
    //传入参数临时存储，防止多线程调用时参数交叉
    temp_f.ip_src = arg->session_five_attr.ip_src;
    temp_f.ip_dst = arg->session_five_attr.ip_dst;
    temp_f.port_src = arg->session_five_attr.port_src;
    temp_f.port_dst = arg->session_five_attr.port_dst;
    hdr->session_five_attr.protocol= arg->session_five_attr.protocol;   
    if(hdr->session_five_attr.protocol==7){//如果协议号为7，代表http协议，此处主要处理网络链接关闭后申请资源的释放
        struct tuple4         addr;
        hash_bucket           *httpBucket=NULL;  
        HttpSession           *http_session=NULL;
        dlp_http_post_head    *dlp_session=NULL;
        addr.saddr = temp_f.ip_src;
        addr.daddr = temp_f.ip_dst;
        addr.source = temp_f.port_src;
        addr.dest = temp_f.port_dst;
        httpBucket = find_hash(ftp_session_tbl, &addr, sizeof(struct tuple4));//查找hash表
        if(httpBucket != NULL)//hash表不为空表示内网通过http协议上传了数据，因此此处需要把使用的数据删除释放空间
        {    
            if(httpBucket->content!=NULL){
                http_session = (HttpSession *)httpBucket->content;

                remove_hash(ftp_session_tbl, httpBucket, sizeof(struct tuple4));//移除hash表节点

                //////////
#if 1
                if(http_session->http!=NULL){
                    //printf("11111111111111111111111111111111111111111111111111111\n");
                    //sleep(1);
                    dlp_session=(dlp_http_post_head  *)http_session->http;
                    if (dlp_session->cookie!= NULL)
                    {
                        free(dlp_session->cookie);
                        dlp_session->cookie=NULL;
                    }

                    if(dlp_session != NULL){
                        free(dlp_session);
                        dlp_session = NULL;
                    }

                }
#endif	
                /////////////


                if(http_session != NULL)
                {
                    free(http_session);
                    http_session = NULL;
                }
            }
            if(httpBucket != NULL)
            {
                free(httpBucket);
                httpBucket = NULL;
            }
        }	
        return 0;
    }
    else if(hdr->session_five_attr.protocol==1){ //协议号为1表示ftp控制流，由于在前期处理的时候已经释放完空间，所以此处无需额外的处理  
        log_printf(ZLOG_LEVEL_INFO, "session func INFO: the protocol is number 1!!");
        return 0;
    }
    else if(hdr->session_five_attr.protocol == 250)//协议号为250表示http协议处理阶段1完成，即将post数据存储完成但还需要进一步处理
    {   
        SESSION_BUFFER_NODE ft;
        HttpSession        sp_session;
        dlp_http_post_head *http_session = (dlp_http_post_head *)malloc(sizeof(dlp_http_post_head)+1);
        if(NULL == http_session)
        {
            log_printf(ZLOG_LEVEL_ERROR, "Insufficient_memory: -----%s------%s------%d\n",__FILE__, __func__, __LINE__);
            return 0;
        }
        memset(http_session,'\0',sizeof(dlp_http_post_head)+1);


        dlp_http_post_head *http_session_temp=NULL;

        //将前期处理的数据拷贝如结构体中的指定位置
        http_session_temp=(dlp_http_post_head *)arg->attr;//指针去赋值

        memcpy(http_session, http_session_temp, sizeof(dlp_http_post_head));


        // int len =strlen(http_session_temp->cookie);
        // http_session->cookie=malloc(len);
        //memcpy(http_session->cookie, http_session_temp->cookie, len);

        memcpy(sp_session.old_name, arg->orgname, PATHLEN);//原始文件名赋值
        memcpy(http_session->new_name, arg->strname, PATHLEN);//存储文件名赋值
        sp_session.http = http_session;//指针去赋值
        //下述为四元组信息赋值
        sp_session.key.saddr = temp_f.ip_src;
        sp_session.key.daddr = temp_f.ip_dst;
        sp_session.key.dest = temp_f.port_dst;
        sp_session.key.source = temp_f.port_src;
        if(http(&sp_session) != EXIT_SUCCESS){
#if 1
            if(http_session->cookie != NULL)
            {
                free(http_session->cookie);
                http_session->cookie = NULL;
            }

            if(http_session != NULL)
            {
                free(http_session);
                http_session = NULL;
            }
#endif
            return 0;//调用http数据包解析函数完成数据包头部信息以及body部分的提取
        }
        if(http_session->content_length == 0)
        {
            log_printf(ZLOG_LEVEL_WARN, "session func WARN: the body len is zero!!");
        }	
        //将提取的信息赋值给信号池中的信号节点等待后续的进一步处理
        ft.attr = sp_session.http;
        ft.session_five_attr.ip_src = sp_session.key.saddr;
        ft.session_five_attr.ip_dst = sp_session.key.daddr;
        ft.session_five_attr.port_src = sp_session.key.source;
        ft.session_five_attr.port_dst = sp_session.key.dest;
        ft.session_five_attr.protocol = 255;



        sbuff_putdata(http_buffer_id, ft); //将待处理的信号节点信息放入信号池等待处理
        return 0;
    }
    else if(hdr->session_five_attr.protocol==3)//协议号为3表示smtp协议，此处处理头部信息以及附件信息的提取
    {	 
        struct tuple4   addr;
        hash_bucket     *smtpBucket=NULL;	
        SmtpSession_t	  *smtp_session=NULL;
        addr.saddr = temp_f.ip_src;
        addr.daddr = temp_f.ip_dst;
        addr.source = temp_f.port_src;
        addr.dest = temp_f.port_dst;
        smtpBucket = find_hash(ftp_session_tbl, &addr, sizeof(struct tuple4));//查找系统hash表，判断是否有smtp信息发送
        if(smtpBucket != NULL)//hash桶有数据表示smtp截获成功
        {
            smtp_session = (SmtpSession_t  *)smtpBucket->content; 
            if (smtp_session->smtp_session_state.dot_seen == TRUE)
            {
                while(smtp_session->next != NULL)	
                {
                    smtp_session = smtp_session->next;
                }
                char *jsonstr = NULL;
                gmime_parse_mailbody (smtp_session, &jsonstr);//smtp邮件解析过程开始

                if (jsonstr )
                {
                    /* json串入桶 */
                    smtp_session->result_json = jsonstr;
                }
                else
                {
                    log_printf(ZLOG_LEVEL_ERROR, "session func ERROR: the jason pointer is NULL!!");
                    return 0;
                }
                //赋值五元组信息，等待后续进一步处理
                hdr->session_five_attr.ip_dst = addr.daddr;
                hdr->session_five_attr.ip_src = addr.saddr;
                hdr->session_five_attr.port_src = addr.source;
                hdr->session_five_attr.port_dst = addr.dest;
                hdr->session_five_attr.protocol = 3;
                return 1;
            }
        }
        else
        {
            log_printf(ZLOG_LEVEL_ERROR, "session func ERROR: there is no buck for smtp!!");
            return 0;
        }
    }
    else //其他未知协议号处理流程，此处主要处理ftp数据流部分内容
    {
        //	  log_printf(ZLOG_LEVEL_INFO, "\n\n################## FTP process=====================222=\n\n");
        //////////////////////////////////FTP process///////////////////////////////////////////////////////
        four_tuple_t   three_atrib;
        hash_bucket    *pBucket=NULL;	
        FtpdataSession *data_session=NULL;
        three_atrib.ip_src = temp_f.ip_src;
        three_atrib.ip_dst = temp_f.ip_dst;
        three_atrib.port_src = temp_f.port_src;
        three_atrib.port_dst = 0x0000;
        pBucket = find_hash(ftp_session_tbl, &three_atrib, sizeof(struct tuple4));
        //	  log_printf(ZLOG_LEVEL_INFO, "\n\n################## FTP process  pBucket = %p======================\n\n",pBucket);
        if(pBucket!=NULL){//hash桶中的数据存在，表示未知协议为ftp数据流
            //参数初始化

            data_session = (FtpdataSession  *)pBucket->content;
            memcpy(hdr->orgname, data_session->file_name, 128);//发送的原始文件名赋值
            remove_hash(ftp_session_tbl, pBucket, sizeof(struct tuple4));//删除hash表节点
            //释放申请空间
            if(pBucket->content != NULL) { 
                free(pBucket->content);
                pBucket->content = NULL;
            }
            if(pBucket != NULL){
                free(pBucket);
                pBucket = NULL;
            }
            memcpy(hdr->strname, arg->strname, PATHLEN);//数据捕获存储文件名

            //五元组信息赋值
            hdr->session_five_attr.ip_dst = temp_f.ip_dst;
            hdr->session_five_attr.ip_src = temp_f.ip_src;
            hdr->session_five_attr.port_src = temp_f.port_src;
            hdr->session_five_attr.port_dst = temp_f.port_dst;
            hdr->session_five_attr.protocol = 175;
            log_printf(ZLOG_LEVEL_INFO, "\n\n################## FTP process======================333 end 175\n\n");
            return 1;
        }
        else
        {
            three_atrib.ip_src = temp_f.ip_src;
            three_atrib.ip_dst = temp_f.ip_dst;
            three_atrib.port_src = temp_f.port_dst;
            three_atrib.port_dst = 0x0000;
            pBucket = find_hash(ftp_session_tbl, &three_atrib, sizeof(struct tuple4));
            if(pBucket!=NULL){	 	
                //参数初始化

                data_session = (FtpdataSession  *)pBucket->content;
                memcpy(hdr->orgname, data_session->file_name, 128);//发送的原始文件名赋值
                remove_hash(ftp_session_tbl, pBucket, sizeof(struct tuple4));//删除hash表节点
                //释放申请空间
                if(pBucket->content != NULL) { 
                    free(pBucket->content);
                    pBucket->content = NULL;
                }
                if(pBucket != NULL){
                    free(pBucket);
                    pBucket = NULL;
                }
                memcpy(hdr->strname, arg->strname, PATHLEN);//数据捕获存储文件名

                //五元组信息赋值
                hdr->session_five_attr.ip_dst = temp_f.ip_dst;
                hdr->session_five_attr.ip_src = temp_f.ip_src;
                hdr->session_five_attr.port_src = temp_f.port_src;
                hdr->session_five_attr.port_dst = temp_f.port_dst;
                hdr->session_five_attr.protocol = 175;
                log_printf(ZLOG_LEVEL_INFO, "\n\n################## FTP process====================== end 175\n\n");
                return 1;
            }
        }
        return 0;
        /////////////////////////////////////////////////////////////////////////////////////////
    }
    return 0;
}

/***************************
Function: tcp_callback()
Description: tcp会话处理回调函数，主要处理http、smtp以及ftp三种协议
Calls: do_smtp，do_http_file
Called By: nids_register_tcp-------->main
Table Accessed: 
Table Updated: 
Input: a_tcp tcp数据流
this_time_not_needed 系统未使用
Output: 无
Return: 无
Others: 
 ***************************/

void tcp_callback(struct tcp_stream *a_tcp, void **this_time_not_needed)
{
#if 1 
    //	  log_printf(ZLOG_LEVEL_INFO, "\n\n################## tcp_callback\n\n");
    SESSION_BUFFER_NODE    sft;
    struct tuple4  ftp_four;
    char           buf[128];
    int            number;
    int            protocol;
    strcpy(buf, adres(a_tcp->addr));//地址格式转换
    //下述处理过程按照tcp链接状态进行处理，分别为建立链接EST，数据传输DATA，链接关闭CLOSE，链接重置RESET四个过程进行处理
    if (a_tcp->nids_state == NIDS_JUST_EST) {//链接建立过程，初始化参数
        a_tcp->client.collect++;	// we want data received by a client
        a_tcp->server.collect++;	// and by a server, too
        a_tcp->server.collect_urg++;	// we want urgent data received by a
#ifdef WE_WANT_URGENT_DATA_RECEIVED_BY_A_CLIENT
        a_tcp->client.collect_urg++;	// if we don't increase this value,
#endif
        return;
    }
    if (a_tcp->nids_state == NIDS_DATA) //数据传输阶段，根据识别出的协议号进行协议处理
    {
        if (a_tcp->client.count_new) //新数据到来，对方向进行调整
        {
            hlf = &a_tcp->client;	// from now on, we will deal with hlf var,
        } else {
            hlf = &a_tcp->server;	// analogical
        }
        number = hlf->count_new;
        if(number>0)//如果数据包内容不为0，进行数据处理
        {	
            ftp_four.saddr = a_tcp->addr.saddr;
            ftp_four.daddr = a_tcp->addr.daddr;
            ftp_four.source = a_tcp->addr.source;
            ftp_four.dest = a_tcp->addr.dest;
            protocol = a_tcp->addr5.protocol;
            if(protocol==1)//ftp控制流协议解析
            {
                //  log_printf(ZLOG_LEVEL_INFO, "\n\n################## tcp_callback doftp enter \n\n");
                doftp(hlf->data, &ftp_four);
#if 0 //by niulw 20160128
                sft.session_five_attr.ip_src = a_tcp->addr.saddr;
                sft.session_five_attr.ip_dst = a_tcp->addr.daddr;
                sft.session_five_attr.port_src = a_tcp->addr.source;
                sft.session_five_attr.port_dst = a_tcp->addr.dest;
                sft.session_five_attr.protocol = a_tcp->addr5.protocol;
                char r_name[PATHLEN];
                sprintf (r_name, "%s/%s.txt", session_stream,buf);
                memcpy(sft.strname, r_name, PATHLEN);
                sbuff_putdata(sbuffer_id, sft);
#endif
                // log_printf(ZLOG_LEVEL_INFO, "\n\n################## tcp_callback Exit \n\n");
                return;
            }
            if(protocol==3)//smtp协议解析处理过程
            {

                do_smtp(hlf->data, &ftp_four, number, sbuff_putdata, sbuffer_id);
                return;
            }
            if(protocol==7)//http协议解析处理过程
            {
                do_http_file(hlf->data, number, &ftp_four, sbuff_putdata, sbuffer_id);
                return;
            }
            if(((ftp_four.source != 21)&&(ftp_four.dest != 21))&&((ftp_four.source != 443)&&(ftp_four.dest != 443)))//其他协议处理过程为将数据直接存储，本系统暂时不处理加密端口发送的数据，并且如果是ftp控制流的端口数据也不做处理
            {
                char  name[PATHLEN];
                FILE  *ftp_m=NULL;
                sprintf (name, "%s/%s.txt", session_stream,buf);
                ftp_m = fopen(name, "a+");
                if(ftp_m!=NULL)
                {  //
                    fwrite(hlf->data, number, 1, ftp_m);
                    fclose(ftp_m);
                }else log_printf(ZLOG_LEVEL_ERROR, "tcp callback other  protocol ERROR:open file !!");
                ftp_m=NULL;
                return;
            }
        }
    }
    if (a_tcp->nids_state == NIDS_CLOSE||a_tcp->nids_state == NIDS_TIMED_OUT) //tcp链接管理处理
    {
        // printf("This is tcp close, the flow_flag is %d!!\n",a_tcp->addr5.flow_flag);
        //	  log_printf(ZLOG_LEVEL_INFO, "\n\n===== sun bj==========tcp_callback NIDS Close a_tcp->addr5.protocol = %d==================\n\n",a_tcp->addr5.protocol);

        if((a_tcp->addr5.protocol != 3)&&(a_tcp->addr5.protocol != 1))//如果不是smtp以及ftp控制流进行下述处理
        {
            sft.session_five_attr.ip_src = a_tcp->addr.saddr;
            sft.session_five_attr.ip_dst = a_tcp->addr.daddr;
            sft.session_five_attr.port_src = a_tcp->addr.source;
            sft.session_five_attr.port_dst = a_tcp->addr.dest;
            sft.session_five_attr.protocol = a_tcp->addr5.protocol;
            char c_name[PATHLEN];
            sprintf (c_name, "%s/%s.txt", session_stream,buf);
            memcpy(sft.strname, c_name, PATHLEN);
            sbuff_putdata(sbuffer_id, sft);
        }
        return;
    }
    if (a_tcp->nids_state == NIDS_RESET) //tcp链接重置处理
    {
        //printf("This is tcp reset,the flow_flag is %d!!\n",a_tcp->addr5.flow_flag);

        // log_printf(ZLOG_LEVEL_INFO, "\n\n===============tcp_callback NIDS RESET a_tcp->addr5.protocol = %d==================\n\n",a_tcp->addr5.protocol);
        if((a_tcp->addr5.protocol != 3)&&(a_tcp->addr5.protocol != 1))//如果不是smtp以及ftp控制流进行下述处理
        {
            sft.session_five_attr.ip_src = a_tcp->addr.saddr;
            sft.session_five_attr.ip_dst = a_tcp->addr.daddr;
            sft.session_five_attr.port_src = a_tcp->addr.source;
            sft.session_five_attr.port_dst = a_tcp->addr.dest;
            sft.session_five_attr.protocol = a_tcp->addr5.protocol;
            char r_name[PATHLEN];
            sprintf (r_name, "%s/%s.txt", session_stream,buf);
            memcpy(sft.strname, r_name, PATHLEN);
            sbuff_putdata(sbuffer_id, sft);
        }
        return;
    }
    return;
#endif
}

/* ******************************** */
/***************************
Function: drop_packet_rule()
Description: 数据包丢弃规则创建函数，系统暂未使用
Calls: 
Called By: 
Table Accessed: 
Table Updated: 
Input: 
Output: 无
Return: 无
Others: 
 ***************************/

void drop_packet_rule(const struct pfring_pkthdr *h) {
    const struct pkt_parsing_info *hdr = &h->extended_hdr.parsed_pkt;
    static int rule_id=0;

    if(add_drop_rule == 1) {
        hash_filtering_rule rule;

        memset(&rule, 0, sizeof(hash_filtering_rule));

        rule.rule_id = rule_id++;    
        rule.vlan_id = hdr->vlan_id;
        rule.proto = hdr->l3_proto;
        rule.rule_action = dont_forward_packet_and_stop_rule_evaluation;
        rule.host4_peer_a = hdr->ip_src.v4, rule.host4_peer_b = hdr->ip_dst.v4;
        rule.port_peer_a = hdr->l4_src_port, rule.port_peer_b = hdr->l4_dst_port;

        if(pfring_handle_hash_filtering_rule(pd, &rule, 1 /* add_rule */) < 0){
            log_printf(ZLOG_LEVEL_ERROR, "drop packet ERROR: add_hash_filtering_rule(1) failed!!");
        }
        else{
            log_printf(ZLOG_LEVEL_INFO, "drop packet info: added filtering rule success!!");
        }
    } else {
        filtering_rule rule;
        int rc;

        memset(&rule, 0, sizeof(rule));

        rule.rule_id = rule_id++;
        rule.rule_action = dont_forward_packet_and_stop_rule_evaluation;
        rule.core_fields.proto = hdr->l3_proto;
        rule.core_fields.shost.v4 = hdr->ip_src.v4, rule.core_fields.shost_mask.v4 = 0xFFFFFFFF;
        rule.core_fields.sport_low = rule.core_fields.sport_high = hdr->l4_src_port;

        rule.core_fields.dhost.v4 = hdr->ip_dst.v4, rule.core_fields.dhost_mask.v4 = 0xFFFFFFFF;
        rule.core_fields.dport_low = rule.core_fields.dport_high = hdr->l4_dst_port;

        if((rc = pfring_add_filtering_rule(pd, &rule)) < 0){
            log_printf(ZLOG_LEVEL_ERROR, "drop packet ERROR: add_hash_filtering_rule(2) failed!!");
        }
        else{
            log_printf(ZLOG_LEVEL_INFO, "drop packet INFO: added filtering rule id success!!");
        }
    }
}

/* ****************************************************** */
/***************************
Function: parse_bpf_filter()
Description: pfring的bpf规则编译函数
Calls: 
Called By: main
Table Accessed: 
Table Updated: 
Input: filter_buffer，规则放置数组
caplen，采样长度
Output: 无
Return: 无
Others: 
 ***************************/

#ifdef ENABLE_BPF
int parse_bpf_filter(char *filter_buffer, u_int caplen) {
    if(pcap_compile_nopcap(caplen,        /* snaplen_arg */
                DLT_EN10MB,    /* linktype_arg */
                &filter,       /* program */
                filter_buffer, /* const char *buf */
                0,             /* optimize */
                0              /* mask */
                ) == -1) {
        return -1;
    }

    if(filter.bf_insns == NULL)
        return -1;

    return 0;
}
#endif

/* ****************************************************** */
/***************************
Function: etheraddr_string()
Description: mac地址转换成字符串
Calls: 
Called By: main
Table Accessed: 
Table Updated: 
Input: ep，待处理的数据串
Output: *buf
Return: *buf
Others: 
 ***************************/

static char *etheraddr_string(const u_char *ep, char *buf) {
    char *hex = "0123456789ABCDEF";
    u_int i, j;
    char *cp;

    cp = buf;
    if((j = *ep >> 4) != 0)
        *cp++ = hex[j];
    else
        *cp++ = '0';

    *cp++ = hex[*ep++ & 0xf];

    for(i = 5; (int)--i >= 0;) {
        *cp++ = ':';
        if((j = *ep >> 4) != 0)
            *cp++ = hex[j];
        else
            *cp++ = '0';

        *cp++ = hex[*ep++ & 0xf];
    }

    *cp = '\0';
    return (buf);
}

/* ****************************************************** */
static int32_t thiszone;
/***************************
Function: dummyProcesssPacket()
Description: 此函数主要是pfring_loop函数的回调函数，等待网络上数据包的到来，当有数据包
经过网络时，便将数据包存入事先申请的数据缓冲池中并写入信号等待下一个线程处理
Calls: rbuff_putdata
Called By: pfring_loop------>main
Table Accessed: 
Table Updated: 
Input: const struct pfring_pkthdr *h，采集数据头信息
const u_char *p，数据包
const u_char *user_bytes，此处无有效值
Output: 无
Return: 无
Others: 
 ***************************/

void dummyProcesssPacket(const struct pfring_pkthdr *h, 
        const u_char *p, const u_char *user_bytes) {
    long threadId = (long)user_bytes;
    struct pcap_pkthdr pkthdr;
    memset(&pkthdr, 0, sizeof(struct pcap_pkthdr)); 
    numPkts[threadId]++, numBytes[threadId] += h->len+24 /* 8 Preamble + 4 CRC + 12 IFG */;

#ifdef ENABLE_BPF
    if (userspace_bpf && bpf_filter(filter.bf_insns, p, h->caplen, h->len) == 0)
        return; /* rejected */ 
#endif
    //转存数据包抓取的头信息
    pkthdr.len = h->len;
    pkthdr.caplen = h->caplen;
    pkthdr.ts.tv_sec = h->ts.tv_sec;
    pkthdr.ts.tv_usec = h->ts.tv_usec;
    //	log_printf(ZLOG_LEVEL_DEBUG, "\n\n===================== Step 1 Dummy Capture Data Package Enter into =====================\n\n");
    if(p != NULL)
        rbuff_putdata(buffer_id, p, &pkthdr, NULL);//将网络上抓取的数据包存入系统的数据缓冲池
    //log_printf(ZLOG_LEVEL_DEBUG, "\n\n===================== Step 1 Dummy Capture Data Package Exit =====================\n\n");
}



/***************************
Function: session_process_thread()
Description: 网络会话重组线程，此线程主要是从数据池中取出数据并交给会话重组函数进行本会话数据的重组
并将重组后的数据通过内部注册的回调函数tcp_callback进行处理
Calls: rbuff_getdata，nids_pcap_handler
Called By: 
Table Accessed: 
Table Updated: 
Input: 线程临时ID号，组要通过它与CPU绑定
Output: 无
Return: 无
Others: 
 ***************************/

void* session_process_thread(void* _id) {
    u_int numCPU = sysconf( _SC_NPROCESSORS_ONLN );
    long thread_id = (long)_id;
    u_long core_id = thread_id % numCPU;

    if((num_threads > 1) && (numCPU > 1)) {
        if(bind2core(core_id) == 0)//线程与cpu绑定
            log_printf(ZLOG_LEVEL_INFO, "session process INFO: Set thread %lu on core %lu/%u !!", thread_id, core_id, numCPU);
    }
    u_char *sbuffer_p;// = buffer;

    struct pcap_pkthdr *shdr = NULL;
    struct tuple5 *sess_five = NULL;
    shdr = malloc(sizeof(struct pcap_pkthdr));//数据包消息头存储结构体
    if(shdr==NULL){
        log_printf(ZLOG_LEVEL_FATAL, "session process FATAL: malloc pcap pkthdr failed!!");
        return(NULL);
    }
    sess_five = malloc(sizeof(struct tuple5));//五元组信息存储结构体
    if(sess_five==NULL){
        log_printf(ZLOG_LEVEL_FATAL, "session process FATAL: malloc tuple5 failed!!");
        return(NULL);
    }
    sbuffer_p = malloc(65536);
    if(sbuffer_p==NULL){
        log_printf(ZLOG_LEVEL_FATAL, "session process FATAL: malloc sbuffer_p failed!!");
        return(NULL);
    }
    while(1){
        if(rbuff_getdata(bbuffer_id, sbuffer_p, shdr, sess_five)>0){//数据缓冲池中提取数据
            //printf("sess_five flow_flag is %d\n",sess_five->flow_flag);
            //log_printf(ZLOG_LEVEL_DEBUG, "\n\n===================== Step 3 session_process_thread nids_pcap_handler Enter into  =====================\n\n");
            nids_pcap_handler(NULL, shdr, sbuffer_p, sess_five);//将提取的数据传入会话重组函数进行会话重组
            //	log_printf(ZLOG_LEVEL_DEBUG, "\n\n===================== Step 3 session_process_thread nids_pcap_handler Exit  =====================\n\n");
        }
    }
    return(NULL);
}

/***************************
Function: WL_upload_thread()
Description: 白名单更新线程，此线程主要是等待主线程下发的白名单更新信号，根据命令码判断如果白名单下发成功，本线程便将
共享内存区中的表名单更新到本地；如果是下发全局参数更新，则进行全局参数的更新，
Calls: receive_msg
Called By: 
Table Accessed: 
Table Updated: 
Input: 线程临时ID号，组要通过它与CPU绑定
Output: 无
Return: 无
Others: 
 ***************************/

void* WL_upload_thread(void* _id) {
    u_int numCPU = sysconf( _SC_NPROCESSORS_ONLN );
    long thread_id = (long)_id;
    u_long core_id = thread_id % numCPU;
    int ret = 0;
    struct file_msg conf_msg;
    if((num_threads > 1) && (numCPU > 1)) {
        if(bind2core(core_id) == 0)//线程绑定cpu
            log_printf(ZLOG_LEVEL_INFO, "session process INFO: Set thread %lu on core %lu/%u !!", thread_id, core_id, numCPU);
    }

    while(1){
        ret = receive_msg(&conf_msg);//等待白名单更新信号
        log_printf(ZLOG_LEVEL_INFO,"################## White list = %s\n",conf_msg.mtext);
        if (ret == 1) {//如果等待成功，判断信号类型进行相应操作
            log_printf(ZLOG_LEVEL_INFO,"WL_rules->use_flag and WL_rules_copy->use_flag is %d and %d",WL_rules->use_flag ,WL_rules_copy->use_flag);
            log_printf(ZLOG_LEVEL_INFO,"WL_rules->up_flag and WL_rules_copy->up_flag is %d and %d",WL_rules->up_flag ,WL_rules_copy->up_flag);
            //通过解析json串的内容识别串里面的关键字
            json_t *conf_str = json_loads(conf_msg.mtext, 0, NULL);
            json_t *command = json_object_get(conf_str, "command");
            const char* cmd = json_string_value(command);
            if(NULL!=cmd)
                log_printf(ZLOG_LEVEL_INFO,"\n\n##### command = %s\n\n",cmd);	
            json_t *content = json_object_get(conf_str, "content");
            const char* cont = json_string_value(content);
            if(NULL!=cont)
                log_printf(ZLOG_LEVEL_INFO,"\n\n##### cont=%s\n\n",cont);	
            if((0 == strcmp(cmd, "10101"))&&(0!=strcmp(cont,"NULL"))){//利用共享内存中的内容更新白名单
                log_printf(ZLOG_LEVEL_INFO,"WL is uploaded!!");
                ReadShareMemory();//更新白名单
                global_flag = 1;//by niulw add
            }
            else if((0 == strcmp(cmd, "10101"))&&(0==strcmp(cont,"NULL"))){//删除白名单及所有数据都需要检测
                WL_rules->use_flag = 0;
                WL_rules->up_flag = 0;
                WL_rules_copy->use_flag = 0;
                WL_rules_copy->up_flag = 0;
                log_printf(ZLOG_LEVEL_INFO, "WL->update Rules is NULL....");
                if(global_flag==1)
                {
                    global_flag = -1;
                    system("killall -9 session_cap");
                }
            }
            else if(0 == strcmp(cmd, "10201")){//更新全局变量即允许存取最大文件大小
                json_t *max_str = json_loads(cont, 0, NULL);
                json_t *max_filesize = json_object_get(max_str, "max_filesize");
                max_cfilesize = json_integer_value(max_filesize);//更新全局变量即文件存取最大值
                json_decref(max_str);
                log_printf(ZLOG_LEVEL_INFO,"max_cfilesize is uploaded!,the number is %d!", max_cfilesize);
            }
            json_decref(conf_str);
        } else {
            log_printf(ZLOG_LEVEL_INFO, "WL->update Rules is failed....\n");
        }
    }
}
/*******************************************/

/***************************
Function: http_file_thread()
Description: http协议处理线程池，此线程池主要是负责所有与http协议相关的内容识别，包括即时通信、上传文件、webmail邮件传输，
Calls: sbuff_getdata，webmail_mail
Called By: 
Table Accessed: 
Table Updated: 
Input: 线程临时ID号，组要通过它与CPU绑定
Output: 无
Return: 无
Others: 
 ***************************/

void* http_file_thread(void* _id) 
{
    u_int numCPU = sysconf( _SC_NPROCESSORS_ONLN );
    long thread_id = (long)_id;
    u_long core_id = thread_id % numCPU;

    if(numCPU > 1) {
        if(bind2core(core_id) == 0)
            log_printf(ZLOG_LEVEL_INFO, "http file process INFO: Set thread %lu on core %lu/%u !!", thread_id, core_id, numCPU);
    }
    SESSION_BUFFER_NODE *sshdr = NULL;
    sshdr = malloc(sizeof(SESSION_BUFFER_NODE));

    if(sshdr==NULL){
        log_printf(ZLOG_LEVEL_FATAL, "http file process FATAL: malloc sshdr failed!!");
        return(NULL);
    }
    while(1){

        if(sbuff_getdata(http_buffer_id, sshdr)>0){//等待信号触发，如果触发成功，执行如下操作
            HttpSession        sp_session;
            sp_session.http = sshdr->attr;
            sp_session.key.saddr = sshdr->session_five_attr.ip_src;
            sp_session.key.daddr = sshdr->session_five_attr.ip_dst;
            sp_session.key.source = sshdr->session_five_attr.port_src;
            sp_session.key.dest = sshdr->session_five_attr.port_dst;

            char *webmailjson=NULL;
            webmail_mail(&sp_session, &webmailjson, sbuff_putdata, ssbuffer_id);//http内容处理函数，负责所有http相关的处理
        }

    }
}
/*******************************************/
/* *************************************** */

/***************************
Function: session_consumer_thread()
Description: ，此线程池主要是负责调用协议深度解析函数解析出上传文件内容，提取信息并触发tika处理流程
Calls: sbuff_getdata
Called By: 
Table Accessed: 
Table Updated: 
Input: 线程临时ID号，组要通过它与CPU绑定
Output: 无
Return: 无
Others: 
 ***************************/

void* session_consumer_thread(void* _id) 
{
    u_int numCPU = sysconf( _SC_NPROCESSORS_ONLN );
    long thread_id = (long)_id;
    u_long core_id = thread_id % numCPU;

    if(numCPU > 1) {
        if(bind2core(core_id) == 0)
            log_printf(ZLOG_LEVEL_INFO, "session consumer INFO: Set thread %lu on core %lu/%u !!", thread_id, core_id, numCPU);
    }

    SESSION_BUFFER_NODE *sshdr = NULL, *stshdr = NULL;
    sshdr = malloc(sizeof(SESSION_BUFFER_NODE));
    stshdr = malloc(sizeof(SESSION_BUFFER_NODE));
    if(sshdr==NULL){
        log_printf(ZLOG_LEVEL_FATAL, "session consumer FATAL: malloc sshdr failed!!");
        return(NULL);
    }
    if(stshdr == NULL){
        log_printf(ZLOG_LEVEL_FATAL, "session consumer FATAL: malloc stshdr failed!!");
        return(NULL);
    }

    while(1){
        //	log_printf(ZLOG_LEVEL_INFO, "\n\n++++++++++++ session_consumer_thread ============================\n\n");
        if(sbuff_getdata(sbuffer_id, sshdr)>0){//信号池中取出满足条件的信息
            //log_printf(ZLOG_LEVEL_INFO, "\n\n++++++++++++ session_consumer_thread ====================1111========\n\n");
            SESSION_BUFFER_NODE tikaattr;  
            if(0<session_proc_fun(sshdr, stshdr)){//调用会话重组协议分析处理函数
                tikaattr = *stshdr;//处理后信息赋值
                sbuff_putdata(ssbuffer_id, tikaattr);//添加待处理的信号量
            }
        }

    }
    return(NULL);
}

/***************************
Function: tika_consumer_thread()
Description: 此线程池主要是负责协议解析完成后的文件内容的提取以及属性的提取，此线程池首先判断数据是否是白名单数据，
如果是白名单内的数据便不进行处理，否则，根据协议号调用tika解析文件内容并生成相应的json串传输到策略匹配
进程进行策略匹配
Calls: sbuff_getdata，file_extract_pthread，send_json
Called By: 
Table Accessed: 
Table Updated: 
Input: 线程临时ID号，组要通过它与CPU绑定
Output: 无
Return: 无
Others: 
 ***************************/

void* tika_consumer_thread(void* _id) 
{
    u_int numCPU = sysconf( _SC_NPROCESSORS_ONLN );
    long thread_id = (long)_id;
    u_long core_id = thread_id % numCPU;

    if(numCPU > 1) {
        if(bind2core(core_id) == 0)
            log_printf(ZLOG_LEVEL_INFO, "file analysis consumer INFO: Set thread %lu on core %lu/%u !!", thread_id, core_id, numCPU);
    }

    SESSION_BUFFER_NODE *sshdr;
    sshdr = malloc(sizeof(SESSION_BUFFER_NODE));
    if(sshdr==NULL){
        log_printf(ZLOG_LEVEL_FATAL, "file analysis process FATAL: malloc sshdr is failed!!");
        return(NULL);
    }

    while(1){
        if(sbuff_getdata(ssbuffer_id, sshdr)>0){//获得信号量进行后续处理
            //char buffer[20] = {};
            char strIP[30] = {0},dstIP[30] = {0};
            int res = 0;
            //getcwd(buffer,20);
            strcpy(sshdr->strpath, session_stream); 
            log_printf(ZLOG_LEVEL_INFO, "\n\n\n#######################################\n\n\n");
            log_printf(ZLOG_LEVEL_INFO, "\n\n\n WL_rules->use_flag =%d,WL_rules_copy->use_flag=%d,WL_rules->up_flag=%d,WL_rules_copy->up_flag=%d\n\n\n");
            log_printf(ZLOG_LEVEL_INFO, "\n\n\n#######################################\n\n\n");
            //判断白名单是否生效
            if((WL_rules->use_flag != 0) || (WL_rules_copy->use_flag != 0)||(WL_rules->up_flag != 0) || (WL_rules_copy->up_flag != 0))
            {   
                strcpy (strIP, int_ntoa (sshdr->session_five_attr.ip_src));
                strcpy (dstIP, int_ntoa (sshdr->session_five_attr.ip_dst));
                log_printf(ZLOG_LEVEL_INFO, "\n\n\n############  strIP = %s,dstIP = %s    #########\n\n\n",strIP,dstIP);
                res = compareIP(strIP, dstIP);//白名单生效后判断此会话ip是否白名单
                if(res == 1){//如果是，释放资源，退出后续处理
                    log_printf(ZLOG_LEVEL_INFO, "\n\n\n############  COMPARE IP OK will be free    #########\n\n\n");
                    if(sshdr->attr != NULL){
                        free(sshdr->attr);
                        sshdr->attr = NULL;
                    }
                    if(sshdr->session_five_attr.protocol==7){//释放HTTP协议申请资源
                        struct tuple4         addr;
                        hash_bucket           *httpBucket=NULL;  
                        HttpSession           *http_session=NULL;
                        dlp_http_post_head    *dlp_session=NULL;
                        addr.saddr =sshdr->session_five_attr.ip_src; 
                        addr.daddr = sshdr->session_five_attr.ip_src;
                        addr.source = sshdr->session_five_attr.port_src;
                        addr.dest = sshdr->session_five_attr.port_dst;
                        httpBucket = find_hash(ftp_session_tbl, &addr, sizeof(struct tuple4));
                        if(httpBucket != NULL)
                        {    
                            http_session = (HttpSession *)httpBucket->content;
                            dlp_session = (dlp_http_post_head *)http_session->http;
                            remove_hash(ftp_session_tbl, httpBucket, sizeof(struct tuple4));
                            if(dlp_session->cookie != NULL)
                            {
                                free(dlp_session->cookie);
                                dlp_session->cookie = NULL;
                            }
                            if(http_session->http != NULL)
                            {
                                free(http_session->http);
                                http_session->http = NULL;
                            }
                            if(http_session != NULL)
                            {
                                free(http_session);
                                http_session = NULL;
                            }
                            if(httpBucket != NULL)
                            {
                                free(httpBucket);
                                httpBucket = NULL;
                            }
                        }
                    }
                    else if(sshdr->session_five_attr.protocol==3)//释放smtp申请资源
                    {
                        struct tuple4    addr;
                        hash_bucket      *smtpBucket=NULL; 
                        SmtpSession_t    *smtp_session=NULL;
                        addr.saddr = sshdr->session_five_attr.ip_src;
                        addr.daddr = sshdr->session_five_attr.ip_dst;
                        addr.source = sshdr->session_five_attr.port_src;
                        addr.dest = sshdr->session_five_attr.port_dst;
                        smtpBucket = find_hash(ftp_session_tbl, &addr, sizeof(struct tuple4));
                        if(smtpBucket != NULL){
                            smtp_session = (SmtpSession_t	*)smtpBucket->content; 
                            remove_hash(ftp_session_tbl, smtpBucket, sizeof(struct tuple4));
                            if(smtp_session){
                                free_smtp(smtp_session);
                                smtp_session = NULL;
                            }
                        }
                    }
                    return 0;	
                }
            }
            if(sshdr->session_five_attr.protocol == 175)//ftp传输的文件处理
            {
                int res = 0;
                res = send_json(sshdr, max_cfilesize);//ftp文件处理函数
                if(res == -1)
                    log_printf(ZLOG_LEVEL_ERROR, "file analysis process ERROR: ftp upload file process failed, jason is NULL!!");
            }
            else if((sshdr->session_five_attr.protocol==157)||(sshdr->session_five_attr.protocol==7))//webmail和http协议
            {
                if(sshdr->attr != NULL){
                    int res = 0;//sshdr->strpath
                    res = file_extract_pthread ((char *)sshdr->attr, session_json, max_cfilesize);//拾取文件处理函数
                    if (res == -1)
                    {    
                        log_printf(ZLOG_LEVEL_ERROR, "file analysis process ERROR: webmail process failed, jason is NULL!!");
                    }
                    free(sshdr->attr);    
                }else
                    log_printf(ZLOG_LEVEL_ERROR, "file analysis process ERROR: webmail process failed, parameter transfered is NULL!!");
            }
            else if(sshdr->session_five_attr.protocol==3)//smtp处理流程
            {
                struct tuple4    addr;
                hash_bucket      *smtpBucket=NULL; 
                SmtpSession_t    *smtp_session=NULL;
                addr.saddr = sshdr->session_five_attr.ip_src;
                addr.daddr = sshdr->session_five_attr.ip_dst;
                addr.source = sshdr->session_five_attr.port_src;
                addr.dest = sshdr->session_five_attr.port_dst;
                smtpBucket = find_hash(ftp_session_tbl, &addr, sizeof(struct tuple4));//判断是否有效的smtp传输
                if(smtpBucket != NULL){
                    smtp_session = (SmtpSession_t	*)smtpBucket->content; 
                    /* 调用tika进行解析 */
                    if (smtp_session->result_json == NULL)
                    {
                        log_printf(ZLOG_LEVEL_ERROR, "file analysis process ERROR: smtp file process failed, transfered jason is NULL!!");
                    }else{
                        int res = 0;
                        res = file_extract_pthread (smtp_session->result_json, session_json, max_cfilesize);//文件处理函数
                        if (res == -1)
                        {
                            log_printf(ZLOG_LEVEL_ERROR, "file analysis process ERROR: smtp file process failed, the return value is -1!!");
                        }
                    }
                    remove_hash(ftp_session_tbl, smtpBucket, sizeof(struct tuple4));//释放hash表节点
                    /* 释放桶中资源 */
                    if(smtp_session){
                        free_smtp(smtp_session);
                        smtp_session = NULL;
                    }
                }else
                    log_printf(ZLOG_LEVEL_ERROR, "file analysis process ERROR: smtp file process failed, the buck is NULL!!");
            }
        }
    }
    return(NULL);
}

/***************************
Function: gmt2local()
Description: 时间转换，将gmt时间转化为本地时间
Calls: 
Called By: main
Table Accessed: 
Table Updated: 
Input: time_t t  时间
Output: 无
Return: 无
Others: 
 ***************************/

int32_t gmt2local(time_t t) {
    int dt, dir;
    struct tm *gmt, *loc;
    struct tm sgmt;

    if(t == 0)
        t = time(NULL);
    gmt = &sgmt;
    *gmt = *gmtime(&t);
    loc = localtime(&t);
    dt = (loc->tm_hour - gmt->tm_hour) * 60 * 60 +
        (loc->tm_min - gmt->tm_min) * 60;

    /*
     * If the year or julian day is different, we span 00:00 GMT
     * and must add or subtract a day. Check the year first to
     * avoid problems when the julian day wraps.
     */
    dir = loc->tm_year - gmt->tm_year;
    if(dir == 0)
        dir = loc->tm_yday - gmt->tm_yday;
    dt += dir * 24 * 60 * 60;

    return (dt);
}

/* *************************************** */
/***************************
Function: printHelp()
Description: 答应帮助信息，解释相应的命令代表的意思
Calls: 
Called By: main
Table Accessed: 
Table Updated: 
Input: 
Output: 无
Return: 无
Others: 
 ***************************/

void printHelp(void) {
    printf("pfcount - (C) 2005-14 ntop.org\n\n");
    printf("-h              Print this help\n");
    printf("-i <device>     Device name. Use:\n"
            "                - ethX@Y for channels\n"
            "                - dnaX for DNA-based adapters\n"
            "                - dnacluster:X for DNA cluster Id X\n"
#ifdef HAVE_DAG
            "                - dag:dagX:Y for Endace DAG cards\n"
#endif
          );
    printf("-n <threads>    Number of polling threads (default %d)\n", num_threads);
    printf("-f <filter>     [BPF filter]\n"); 
    printf("-c <cluster id> cluster id\n");
    printf("-e <direction>  0=RX+TX, 1=RX only, 2=TX only\n");
    printf("-l <len>        Capture length\n");
    printf("-g <core_id>    Bind this app to a core\n");
    printf("-d <device>     Device on which incoming packets are copied (e.g. userspace:usr0 or dna1)\n");
    printf("-w <watermark>  Watermark\n");
    printf("-p <poll wait>  Poll wait (msec)\n");
    printf("-b <cpu %%>      CPU pergentage priority (0-99)\n");
    printf("-a              Active packet wait\n");
    printf("-m              Long packet header (with PF_RING extensions)\n");
    printf("-r              Rehash RSS packets\n");
    printf("-s              Enable hw timestamping\n");
    printf("-S              Do not strip hw timestamps (if present)\n");
    printf("-t              Touch payload (for force packet load on cache)\n");
    printf("-T              Dump CRC (test and DNA only)\n");
    printf("-C              Work in chunk mode (test only)\n");
    printf("-x <path>       File containing strings to search string on payload.\n");
    printf("-u <1|2>        For each incoming packet add a drop rule (1=hash, 2=wildcard rule)\n");
    printf("-v <mode>       Verbose [1: verbose, 2: very verbose (print packet payload)]\n");
    exit(0);
}

/***************************
Function: ac_match_handler()
Description: 
Calls: 
Called By: main
Table Accessed: 
Table Updated: 
Input: 
Output: 无
Return: 无
Others: 
 ***************************/

static int ac_match_handler(AC_MATCH_t *m, void *param) {
    int *matching_protocol_id = (int*)param;

    *matching_protocol_id = 1;

    return 1; /* 0 to continue searching, !0 to stop */
}

/* *************************************** */
/***************************
Function: add_string_to_automa()
Description: 
Calls: 
Called By: main
Table Accessed: 
Table Updated: 
Input: 
Output: 无
Return: 无
Others: 
 ***************************/

static void add_string_to_automa(char *value) {
    AC_PATTERN_t ac_pattern;

    log_printf(ZLOG_LEVEL_INFO, "add string to automa INFO:adding string '%s' to search list...", value);

    ac_pattern.astring = value, ac_pattern.rep.number = 2;
    ac_pattern.length = strlen(ac_pattern.astring);
    ac_automata_add(((AC_AUTOMATA_t*)automa), &ac_pattern);
}

/* *************************************** */
/***************************
Function: load_strings()
Description: 
Calls: 
Called By: main
Table Accessed: 
Table Updated: 
Input: 
Output: 无
Return: 无
Others: 
 ***************************/

static void load_strings(char *path) {
    FILE *f = fopen(path,"r");
    char *s, buf[256] = { 0 };

    if(f == NULL) {
        log_printf(ZLOG_LEVEL_FATAL, "load strings FATAL:unable to open file %s", path);
        //printf("Unable to open file %s\n", path);
        return;
    }
    automa = ac_automata_init(ac_match_handler);

    while((s = fgets(buf, sizeof(buf)-1, f)) != NULL) {
        if((s[0] != '\0')
                && (s[0] != '\n')
                && (s[0] != '\r')) {
            s[strlen(s)-1] = '\0';
            add_string_to_automa(s);
        }
    }

    ac_automata_finalize((AC_AUTOMATA_t*)automa);

    fclose(f);
}

/* *************************************** */
/******************************************/
/***************************
Function: runconf_table_conf()
Description: 系统全局参数配置函数，主要负责读取系统配置文件并提取其中的全局参数值进行系统运行参数配置
Calls: 
Called By: main
Table Accessed: 
Table Updated: 
Input: char *filename//配置文件文件名
Output: 无
Return: 无
Others: 
 ***************************/

int runconf_table_conf(char *filename){
    int num = 0;
    xmlDocPtr doc;
    xmlNodePtr root, node, detail;
    xmlChar *name = NULL, *value = NULL;
    doc = xmlReadFile(filename ,"UTF-8",XML_PARSE_RECOVER);//XML文件读取函数

    if(NULL == doc)
    {
        log_printf(ZLOG_LEVEL_ERROR, "Open configuration file ERROR: can not open file %s" , filename);
        return -1;
    }

    root = xmlDocGetRootElement(doc);//寻找根节点

    if(NULL == root)
    {
        log_printf(ZLOG_LEVEL_ERROR, "Read configuration file ERROR: empty document %s" , filename);
        /*******add code file default configuration*********/
        return -1;
    }

    for(node=root->children;node;node=node->next)
    {
        if(xmlStrcasecmp(node->name,BAD_CAST"init_dev_param")==0)//判断初始化设备名称
        {
            device=xmlGetProp(node,BAD_CAST"init_dev");
        }
        if(xmlStrcasecmp(node->name,BAD_CAST"init_bind_port")==0)//判断程序运行绑定的端口号
        {
            name=xmlGetProp(node,BAD_CAST"bind_port");  
            if(name){
                bind_port=atoi((char* )name);
                free(name);
            }
        }
        if(xmlStrcasecmp(node->name,BAD_CAST"init_tika_param")==0)//判断tika运行线程数
        {
            name=xmlGetProp(node,BAD_CAST"tika_threads");  
            if(name){
                tika_pthreads=atoi((char*)name);
                free(name);
            }
        }
        if(xmlStrcasecmp(node->name,BAD_CAST"init_http_param")==0)//判断http运行线程数
        {
            name=xmlGetProp(node,BAD_CAST"http_threads");  
            if(name){
                http_pthreads=atoi((char*)name);
                free(name);
            }
        }
        if(xmlStrcasecmp(node->name,BAD_CAST"init_session_param")==0)//判断会话处理线程数
        {
            name=xmlGetProp(node,BAD_CAST"session_threads");  
            if(name){
                session_pthreads=atoi((char*)name);
                free(name);
            }
        }
        if(xmlStrcasecmp(node->name,BAD_CAST"init_max_filesize")==0)//判断系统处理=最大文件大小
        {
            name=xmlGetProp(node,BAD_CAST"max_filesize");  
            if(name){
                max_cfilesize=atoi((char*)name);
                free(name);
            }
        }
        /************************************/

        if(xmlStrcasecmp(node->name,BAD_CAST"init_session_stream")==0)//判断系统处理=最大文件大小
        {
            value=xmlNodeGetContent(node);
            memcpy(session_stream,(char*)value,PATHLEN);
            xmlFree(value);
        }
        if(xmlStrcasecmp(node->name,BAD_CAST"init_session_json")==0)//判断系统处理=最大文件大小
        {
            value=xmlNodeGetContent(node);
            memcpy(session_json,(char*)value,PATHLEN);
            xmlFree(value);
        }


        /***********************************/
        //以下为协议扩展预留，本系统暂未使用
        if(xmlStrcasecmp(node->name,BAD_CAST"protocol")==0)
        {
            name=xmlGetProp(node,BAD_CAST"protocol_type");  
            num=atoi((char*)name);
            parser_protocol[num].protocol_num=num;
            xmlFree(name);
            for(detail=node->children;detail;detail=detail->next)
            {  
                if(xmlStrcasecmp(detail->name,BAD_CAST"protocal_mark")==0)
                {
                    value=xmlNodeGetContent(detail);
                    xmlFree(value);		   
                } 
                else if(xmlStrcasecmp(detail->name,BAD_CAST"protocal_so_file_name")==0)
                {
                    value=xmlNodeGetContent(detail);
                    memcpy(parser_protocol[num].parser_lib,(char*)value,128);
                    xmlFree(value);
                }
                else if(xmlStrcasecmp(detail->name,BAD_CAST"protocal_parser_packet_func")==0)
                {
                    value=xmlNodeGetContent(detail);
                    memcpy(parser_protocol[num].parser_packet_func,(char*)value,128);
                    xmlFree(value);
                }
                else if(xmlStrcasecmp(detail->name,BAD_CAST"protocal_parser_accessory_func")==0)
                {
                    value=xmlNodeGetContent(detail);
                    memcpy(parser_protocol[num].parser__accessory_func,(char*)value,128);
                    xmlFree(value);
                }		 
            }
        }
    }
    xmlFreeDoc(doc);
    return 0;
}
int check_process(char* pfile){
    if (pfile == NULL)
        return -1;
    int lockfd = open(pfile,O_RDWR);
    if (lockfd == -1)
        return -2;
    int iret = flock(lockfd,LOCK_EX|LOCK_NB);
    if (iret == -1)
        return -3;

    return 0;
}
/******************************************/
#define MAX_NUM_STRINGS  32

int main(int argc, char* argv[]) {
    if(check_process("/opt/mcwd-ndlp/dlp_capture/1.txt")<0)
        exit(0);
#if 0
    iconv_t hIconv = iconv_open( "UTF-8", "GBK" );
#endif
    char c, buf[32], path[256] = { 0 }, *reflector_device = NULL;
    u_char mac_address[6] = { 0 };
    int promisc = 0, snaplen = DEFAULT_SNAPLEN, rc = 0;
    u_int clusterId = 0;
    u_int8_t chunk_mode = 0;
    u_int32_t flags = 0;
    int bind_core = -1;
    packet_direction direction = rx_and_tx_direction;
    u_int16_t watermark = 0, poll_duration = 0, 
              cpu_percentage = 0, rehash_rss = 0;
    /******************************/ 
    char *conf_file = NULL;
    session_stream=malloc(PATHLEN);
    session_json=malloc(PATHLEN);
    memset(session_stream, 0, PATHLEN);
    memset(session_json, 0, PATHLEN);

    /******************************/

#if 1
    if (zlog_init("conf/session_cap.conf")) {//开启日志功能
        printf("zlog init failed");
        return (-1);
    }
    //设置日志等级
    ftl_log = zlog_get_category(LOG4C_CATEGORY_FTL);
    err_log = zlog_get_category(LOG4C_CATEGORY_ERR);
    wrn_log = zlog_get_category(LOG4C_CATEGORY_WRN);
    dbg_log = zlog_get_category(LOG4C_CATEGORY_DBG);
    inf_log = zlog_get_category(LOG4C_CATEGORY_INF);
    ntc_log = zlog_get_category(LOG4C_CATEGORY_NTC);
#endif

    if(0 != read_IM_config())
    {
        log_printf(ZLOG_LEVEL_ERROR, "failed to read IM config file: -----%s------%s------%d\n",__FILE__, __func__, __LINE__);
        return -1;
    }
#ifdef ENABLE_BPF
    char *bpfFilter = "not udp";//添加bpf过滤规则，本系统过滤udp协议数据
    log_printf(ZLOG_LEVEL_INFO, "bpf filter setting INFO: not udp");
#endif

    startTime.tv_sec = 0;
    thiszone = gmt2local(0);

    while((c = getopt(argc,argv,"hi:c:Cd:F:l:v:ae:n:w:p:b:rg:u:mtsSTx:"
#ifdef ENABLE_BPF
                    "f:"
#endif
                    )) != '?') {
        if((c == 255) || (c == -1)) break;

        switch(c) {
            case 'h':
                printHelp();
                return(0);
                break;
            case 'F'://读取配置文件
                conf_file = optarg;
                break;
        }
    }

    bind_core = bind_core;
    log_printf(ZLOG_LEVEL_INFO, "system INFO:this program version is v.%d.%d.%d!!",big_version, mid_version, dlp_version);
    log_printf(ZLOG_LEVEL_INFO, "system INFO: This version add version showing and tika thread return expression is deleted!!");

    if(conf_file != NULL)
    {
        runconf_table_conf(conf_file);
        singleton(bind_port);
        log_printf(ZLOG_LEVEL_INFO, "system INFO: bind_port is %d!", bind_port);
    }

    if(verbose) 
        watermark = 1;
    if(device == NULL) 
        device = DEFAULT_DEVICE;
    if(num_threads > MAX_NUM_THREADS) 
        num_threads = MAX_NUM_THREADS;

    promisc = 1;
    //以下为pfring初始化过程
    if(wait_for_packet && (cpu_percentage > 0)) {
        if(cpu_percentage > 99) cpu_percentage = 99;
        pfring_config(cpu_percentage);
    }

    if(num_threads > 1)         flags |= PF_RING_REENTRANT;
    if(use_extended_pkt_header) flags |= PF_RING_LONG_HEADER;
    if(promisc)                 flags |= PF_RING_PROMISC;
    if(enable_hw_timestamp)     flags |= PF_RING_HW_TIMESTAMP;
    if(!dont_strip_timestamps)  flags |= PF_RING_STRIP_HW_TIMESTAMP;
    if(chunk_mode)              flags |= PF_RING_CHUNK_MODE;
    flags |= PF_RING_DNA_SYMMETRIC_RSS;  /* Note that symmetric RSS is ignored by non-DNA drivers */

    //printf("flags: %d\n", flags);
    pd = pfring_open(device, snaplen, flags);
    log_printf(ZLOG_LEVEL_INFO, "system INFO:naplen: %d",snaplen);

    if(pd == NULL) {
        log_printf(ZLOG_LEVEL_FATAL, "system FATAL:pf_ring open debice failed,the device is %s!!!", device);
        return(-1);
    } else {
        u_int32_t version;

        pfring_set_application_name(pd, "pfcount");
        pfring_version(pd, &version);

        log_printf(ZLOG_LEVEL_INFO, "system INFO: using PF_RING v.%d.%d.%d!!!",
                (version & 0xFFFF0000) >> 16,
                (version & 0x0000FF00) >> 8,
                version & 0x000000FF);
    }

    if(strstr(device, "dnacluster:")) {
        log_printf(ZLOG_LEVEL_INFO, "system INFO: capturing from %s!!!", device);
    } else {
        if(show_crc && strncmp(device, "dna", 3)) {
            log_printf(ZLOG_LEVEL_ERROR, "system ERROR: -T can be enabled only with DNA!!!!");
            show_crc = 0;
        }

        if(pfring_get_bound_device_address(pd, mac_address) != 0)
            log_printf(ZLOG_LEVEL_ERROR, "system ERROR: unable to read the device address!!!");
        else {
            int ifindex = -1;

            pfring_get_bound_device_ifindex(pd, &ifindex);

            log_printf(ZLOG_LEVEL_INFO, "system INFO: capturing from %s [%s][ifIndex: %d]!!!", 
                    device, etheraddr_string(mac_address, buf), ifindex);
        }
    }

    log_printf(ZLOG_LEVEL_INFO, "system INFO: # Device RX channels: %d!!!", pfring_get_num_rx_channels(pd));
    log_printf(ZLOG_LEVEL_INFO, "system INFO: # Polling threads:    %d!!!", num_threads);

    if (enable_hw_timestamp) {
        struct timespec ltime;
        /* Setting current clock */
        if (clock_gettime(CLOCK_REALTIME, &ltime) != 0 ||
                pfring_set_device_clock(pd, &ltime) < 0)
            log_printf(ZLOG_LEVEL_ERROR, "system ERROR: error setting device clock!!!");
    }

#ifdef ENABLE_BPF
    if(bpfFilter != NULL)
    {

#if 0
        if (pd->dna.dna_mapped_device)
        {

            if (parse_bpf_filter(bpfFilter, snaplen) == 0)
            {
                userspace_bpf = 1;
                log_printf(ZLOG_LEVEL_INFO, "system INFO: Successfully set BPF filter '%s'!!!", bpfFilter);
            }
            else
                log_printf(ZLOG_LEVEL_ERROR, "system ERROR: error compiling BPF filter '%s'!!!", bpfFilter);

        } 
        else
#endif
        {

            rc = pfring_set_bpf_filter(pd, bpfFilter);
            if(rc != 0)
                log_printf(ZLOG_LEVEL_INFO, "system INFO: ring_set_bpf_filter(%s) returned %d!!!", bpfFilter, rc);
            else
                log_printf(ZLOG_LEVEL_INFO, "system INFO: Successfully set BPF filter '%s'!!!", bpfFilter);
        }
    }
#endif

    if(clusterId > 0) {
        rc = pfring_set_cluster(pd, clusterId, cluster_round_robin);
        log_printf(ZLOG_LEVEL_INFO, "system INFO: ring_set_cluster returned %d!!!", rc);
    }

    if((rc = pfring_set_direction(pd, direction)) != 0)
        ; //fprintf(stderr, "pfring_set_direction returned %d (perhaps you use a direction other than rx only with DNA ?)\n", rc);

    if((rc = pfring_set_socket_mode(pd, recv_only_mode)) != 0)
        log_printf(ZLOG_LEVEL_ERROR, "system ERROR: ring_set_socket_mode returned [rc=%d]!!!", rc);

    if(watermark > 0) {
        if((rc = pfring_set_poll_watermark(pd, watermark)) != 0)
            log_printf(ZLOG_LEVEL_ERROR, "system ERROR: ring_set_poll_watermark returned [rc=%d][watermark=%d]!!!", rc, watermark);
    }

    if(reflector_device != NULL) {
        rc = pfring_set_reflector_device(pd, reflector_device);

        if(rc == 0) {
            log_printf(ZLOG_LEVEL_INFO, "system INFO: ring_set_reflector_device(%s) succeeded\n", reflector_device);
        } else
            log_printf(ZLOG_LEVEL_ERROR, "system ERROR: ring_set_reflector_device(%s) failed [rc: %d]!!!", reflector_device, rc);
    }

    if(rehash_rss)
        pfring_enable_rss_rehash(pd);

    if(poll_duration > 0)
        pfring_set_poll_duration(pd, poll_duration);

    //以上为pfring初始化过程
    g_mime_init (0);//gmime多线程支持初始化函数
    nids_init();//会话重组初始化函数


    /* init kmem interface */
    ikmem_init(NULL);//内存管理器初始化函数

    build_JVM();//java虚拟机初始化函数 打开JVM //by niulw


    if(ftp_session_tbl==NULL)//系统使用hash表创建
    {
        if((ftp_session_tbl=create_hash(100000,session_hash, (t_comp_func)session_comp))==0)
        {
            log_printf(ZLOG_LEVEL_FATAL, "system FATAL: create primary table failed!!!");
            return (-1);
            //printf("ftp create_hash fail\n");
        }
    }
    /**********白名单初始化过程**************/
    WL_rules=malloc(sizeof(FilterInfo));
    if(WL_rules==NULL){
        log_printf(ZLOG_LEVEL_FATAL,"WL->malloc error[%d]...",__LINE__);
        return(-1);
    }   
    memset(WL_rules,0,sizeof(FilterInfo));
    WL_rules_copy=malloc(sizeof(FilterInfo));
    if(WL_rules_copy==NULL){
        log_printf(ZLOG_LEVEL_FATAL,"WL->malloc error[%d]...",__LINE__);
        return(-1);
    }   
    memset(WL_rules_copy,0,sizeof(FilterInfo));
    /**********白名单初始化过程**************/

    nids_register_tcp (tcp_callback);//会话重组回调函数注册

    Five_attr = malloc(sizeof(struct tuple5));//五元组结构体内存申请
    if(Five_attr==NULL){
        log_printf(ZLOG_LEVEL_FATAL, "system FATAL: Five attr alloca error!!!");
        return (-1);
    }
    memset(Five_attr, 0, sizeof(struct tuple5));

    buffer_id = rbuff_create (600000);//一级数据缓存池申请
    if(buffer_id < 0){
        log_printf(ZLOG_LEVEL_FATAL, "system FATAL: create buffer id failed!!!");
        return (-1);
    }else{
        log_printf(ZLOG_LEVEL_INFO, "system INFO: buffer_id is %d!!!", buffer_id);
    }

    bbuffer_id = rbuff_create (250000);//二级数据缓存池申请
    if(bbuffer_id < 0){
        log_printf(ZLOG_LEVEL_FATAL, "system FATAL: bbuffer id failed!!!");
        return (-1);
    }else{
        log_printf(ZLOG_LEVEL_INFO, "system INFO: bbuffer_id is %d!!!", bbuffer_id);
    }

    sbuffer_id = sbuff_create (100000);//一级信号池申请
    if(sbuffer_id < 0){
        log_printf(ZLOG_LEVEL_FATAL, "system FATAL: sbuffer id failed!!!");
        return (-1);
    }else{
        log_printf(ZLOG_LEVEL_INFO, "system INFO: sbuffer_id is %d!!!", sbuffer_id);
    }

    ssbuffer_id = sbuff_create (100000);//二级信号池申请
    if(ssbuffer_id < 0){
        log_printf(ZLOG_LEVEL_FATAL, "system FATAL: ssbuffer id failed!!!");
        return (-1);
    }else{
        log_printf(ZLOG_LEVEL_INFO, "system INFO: ssbuffer_id is %d!!!", ssbuffer_id);
    }

    http_buffer_id = sbuff_create (100000);//http处理信号池申请
    if(http_buffer_id < 0){
        log_printf(ZLOG_LEVEL_FATAL, "system FATAL: http_buffer_creat failed!!!");
        return (-1);
    }else{
        log_printf(ZLOG_LEVEL_INFO, "system INFO: http_buffer_id is %d!!!", http_buffer_id);
    }

    memset(ndpi_thread_info, 0, sizeof(ndpi_thread_info)); 
    setupDetection(0);//协议解析功能初始化

    struct timeval usec;
    gettimeofday(&usec, NULL);
    srand(usec.tv_usec);

    pfring_set_application_stats(pd, "Statistics not yet computed: please try again...");
    if(pfring_get_appl_stats_file_name(pd, path, sizeof(path)) != NULL)
        log_printf(ZLOG_LEVEL_INFO, "system INFO: dumping statistics on %s!!!", path);

    if (pfring_enable_ring(pd) != 0) {
        log_printf(ZLOG_LEVEL_FATAL, "system FATAL: unable to enable ring :-(!!!");
        pfring_close(pd);
        return(-1);
    }

    unsigned char i = 0;

    long thread_id;
    for(thread_id = 0; thread_id < 1; thread_id++)
        pthread_create(&ndpi_thread_info[thread_id].pthread, NULL, packet_consumer_thread, (void *) thread_id);
    //for(i=0; i<1; i++)
    //pthread_create(&my_thread[0], NULL, packet_consumer_thread, &i);//数据包存储线程创建

    for(i=0; i<1; i++)
        pthread_create(&my_thread[0], NULL, session_process_thread, &i);//数据包会话处理线程创建

    for(i=1; i<2; i++)
        pthread_create(&my_thread[1], NULL, WL_upload_thread, &i);//白名单更新及全局参数更新处理线程创建

    thread_id=0;
    pthread_create(&ndpi_timeout_free, NULL, ndpi_free_thread, (void *) thread_id);//释放ndpi树的超时节点

    pthread_t http_session_thread[800];
    pthread_t session_thread[1000];
    pthread_t tika_thread[1000];
    unsigned short thd_id = 0;

    for(thd_id=2; thd_id<session_pthreads; thd_id++)
        pthread_create(&session_thread[thd_id], NULL, session_consumer_thread, &thd_id);//会话内内容解析线程池创建

    for(thd_id=2; thd_id<http_pthreads; thd_id++)
        pthread_create(&http_session_thread[thd_id], NULL, http_file_thread, &thd_id);//http协议内容解析线程池创建

    for(thd_id=2; thd_id<tika_pthreads; thd_id++)
        pthread_create(&tika_thread[thd_id], NULL, tika_consumer_thread, &thd_id);//tika文件内容及属性提取处理线程池创建

    pfring_loop(pd, dummyProcesssPacket, (u_char*)NULL, wait_for_packet);//pfring抓包处理函数调用


    for(i=0; i<2; i++)
        pthread_join(my_thread[i], NULL);
    for(thread_id = 0; thread_id < 1; thread_id++)
        pthread_join(ndpi_thread_info[thread_id].pthread, NULL);

    pthread_join(ndpi_timeout_free, NULL);

    sleep(1);

    g_mime_shutdown ();//关闭gmime多线程功能

    pfring_close(pd);//关闭pfring抓包功能

    //destory_JVM();//销毁java虚拟机 //by  niulw打开JVM

    return(0);
}
