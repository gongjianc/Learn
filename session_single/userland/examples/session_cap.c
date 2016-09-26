/************************************************************
  Copyright (C), 2014, DLP FileName: session_cap.c 
Author: Dai Gang	
Version : V 1.0
Date: 2014-10-10 
Description: 
�˳���ΪDLP�������������֧�ֵĹ������£�
1��ͨ��PF_ringץȡ�����ϴ�������ݰ�		   
2��ͨ����ץȡ�����ݰ�����Ӧ��Э������ʶ�𣬵ó�ÿ�����ݰ���Э���			   
3��������Э��ŵ��������ݰ������������ݰ������Լ�����Ự��ԭ		   
4����ԭ������ݰ�����Э�����ͷַ�����ͬ��Э������ʶ����ģ��
5������Э��ʶ������ȡ���ļ�ͨ���ļ�����ģ��tika����ļ����ݵ���ȡ
Version:V 1.0
Function List:	 
1. int runconf_table_conf(char *filename) 
���ܣ�ϵͳ���в�����ʼ������������������ļ��ж������ò�����ֵ  
2. void dummyProcesssPacket(const struct pfring_pkthdr *h, const u_char *p, const u_char *user_bytes)	 
���ܣ�pf_ring�����ץȡ�ص��̣߳�������������ץȡ	 
3. void* packet_consumer_thread(void* _id) 
���ܣ�Э���ʶ���̣߳���Ҫ����ץȡ���������ݰ�����ndpiʶ��������Ӧ��Э���	
4. void* session_process_thread(void* _id)  
���ܣ��Ự�����̣߳���Ҫ��ɽ������ϲɼ������ݽ��лỰ����  
5. void* session_consumer_thread(void* _id)
���ܣ��Ự�����̳߳أ���Ҫ��ɽ������ĻỰ�ַ�����ͬ��Э��ʶ����ģ����лỰ������ȡ  
6. void* http_file_thread(void* _id) 
���ܣ�httpЭ�����ʹ����̳߳أ���Ҫ�������ͨ��httpӦ��Э�鴫�������ʶ������ȡ  
7. void* tika_consumer_thread(void* _id)	 
���ܣ��ļ�ʶ����������ȡ�̳߳أ���Ҫ������������ģ��ʶ������ļ�ͨ��tika��ȡ��Ӧ�������Լ��ļ�����	 
8. void* WL_upload_thread(void* _id)  
���ܣ��������Լ���̬���������̣߳���Ҫ�����������ʵʱ�����Լ�ϵͳ��̬�������ù����в�����ʵʱ����	 
9. static void pcap_packet_callback(u_char * args, const struct pcap_pkthdr *header, const u_char * packet, struct tuple5 *Five_attr)
���ܣ�Э������ʶ�������ӿڣ���Ҫ������������ݰ���Э�����ͽ���ʶ��
10.static unsigned int ndpi_packet_processing(const u_int64_t time, \
const struct ndpi_iphdr *iph, \
struct ndpi_ip6_hdr *iph6, \
u_int16_t ip_offset, \
u_int16_t ipsize, u_int16_t rawsize, struct tuple5 *Five_attr)
���ܣ�����Ự���麯���ӿڣ���Ҫ�����������ݱ��İ��մ���Э����б���������Ự��ԭ����Ӧ�����������������
11.void tcp_callback(struct tcp_stream *a_tcp, void **this_time_not_needed)
���ܣ��Ự��ԭ�ص������ӿڣ���Ҫ���𽫻Ự�������������ݰ��ַ�����ͬ��Э�鴦�����н��д�������ʶ��
12.int rbuff_create (int irb_qlength)
���ܣ����λ���ش��������ӿڣ���Ҫ���𴴽����λ���ش���������ݰ�
13.int rbuff_putdata(const int iringbuffer_ID, void *pData, struct pcap_pkthdr *hdr, struct tuple5 *session_five_attr)
���ܣ���价�λ���غ����ӿڣ���Ҫ���𽫲�����������ݰ����뻷�λ����
14.int rbuff_getdata(const int iringbuffer_ID, void *pData, struct pcap_pkthdr *hdr, struct tuple5 *session_five_attr)
���ܣ���ȡ���λ���غ����ӿڣ���Ҫ���𽫻��λ���ش�ŵ��������ݰ���ȡ����
15.int sbuff_create (int irb_qlength)
���ܣ��¼������źųش�����������Ҫ���𴴽��źų�
16.int sbuff_putdata(const int iringbuffer_ID, SESSION_BUFFER_NODE ft)
���ܣ��¼����봦�����ӿڣ���Ҫ���𽫴�������źŷ����źųصȴ�����
17.int sbuff_getdata(const int iringbuffer_ID, SESSION_BUFFER_NODE *hdr)
���ܣ��¼���ȡ����������Ҫ���¼����е��¼�����˳����ȡ�����������̬�Ĵ�����
18.void doftp(const char *packet_content, struct tuple4 *ftp_four_id)
���ܣ�FTPЭ����������������ӿڣ���Ҫ����FTPЭ����������ݵľ��������������Ӧ��hash��
19.int do_smtp (const u_char *packet_content, struct tuple4 *key, unsigned int packlen, smtp_sbuffputdata sbuff_putdata, int sbuffer_id)
���ܣ�SMTPЭ�������������Ҫ������ȡSMTPЭ�������Ľ����Լ��������ݵĳ�����ȡ�������ļ���Smtp_dissect.c�ļ�856��
20.int do_http_file(const char *packet_content, int len, struct tuple4 *key, http_sbuffputdata http_sbuff_putdata,const int iringbuffer_ID)
����: HTTPЭ�����������������Ҫ������ɻ���httpӦ��Э��ĻỰ������POST��Ϣ����ȡ�������ļ���Parsertrace.c�ļ�508��
21.int http( HttpSession *p_session)
���ܣ���Ҫ������ȡhttpЭ�鴫�������postͷ��Ϣ�Լ����ݵ���ȡ�������ļ���Parsertrace.c�ļ�488��
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
/*�Ự��������ͷ�ļ�*/
#include "checksum.h"
#include "ip_fragment.h"
#include "scan.h"
#include "tcp.h"
#include "util.h"
#include "nids.h"
/*�Ự��������ͷ�ļ�*/
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
/*Э��ʶ��ģ��ͷ�ļ�*/
#include "nDPI/config.h"
#include "linux_compat.h"
#include "ndpi_main.h"
/*Э��ʶ��ģ��ͷ�ļ�*/
/***********����������ʹ��ͷ�ļ�**************************/
#include "msgqueue.h"
#include <jansson.h>
#include "pfcountWL.h"
#include "plog.h"
/***********����������ʹ��ͷ�ļ�**************************/
/***********�ڴ����������**************************/
#define IMLOG_ENABLE
#define IMEM_DEBUG
#define IKMEM_NOFIB
#include "imembase.c"
#include "beap_hash.c"
/***********�ڴ����������**************************/
//#include "fileparse.h"

#if defined(__OpenBSD__)
#include <sys/socket.h>
#endif

/**********************/
//HTTP Э�����ͷ�ļ�
/**********************/
#include "http_parser.h"

#include "http_IM_parser.h"
#include "webmail_parser.h"
/**********************/
//HTTP Э�����ͷ�ļ�
/**********************/

/**********************/
//SMTP Э�����ͷ�ļ�
/**********************/
#include "extractmain.h"

#include "gmime_smtp.h"
/**********************/
//SMTP Э�����
/**********************/

/*******************************/
//ץ�����������ļ�����ͷ�ļ�
/*******************************/
#include<libxml/parser.h>
#include<libxml/tree.h>
/*******************************/
//ץ�����������ļ�����ͷ�ļ�
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
/****************ץ�������ʼ����������*********************/
char *device = NULL;
unsigned int tika_pthreads = 0;
unsigned int http_pthreads = 0;
unsigned int session_pthreads = 0;
unsigned int max_cfilesize = 104857600;

/*******************************/
//Configration file parser parameter��Ԥ��
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
//Configration file parser parameter��Ԥ��
/*******************************/

/****************��ʱ�ļ��洢·��***************/
#define PATHLEN  128
char *session_stream;
char *session_json;
static int global_flag=-1;
/*******************************/
/****************ץ�������ʼ����������*********************/
pfring  *pd;
int verbose = 0, num_threads = 8;
pfring_stat pfringStats;
void *automa = NULL;
static struct timeval startTime;
unsigned long long numPkts[MAX_NUM_THREADS] = { 0 }, numBytes[MAX_NUM_THREADS] = { 0 }, numStringMatches[MAX_NUM_THREADS] = { 0 };
int buffer_id, bbuffer_id, sbuffer_id, ssbuffer_id,http_buffer_id; //ץ���������ó��򻺳��ID����

#ifdef ENABLE_BPF
unsigned long long numPktsFiltered[MAX_NUM_THREADS] = { 0 };
struct bpf_program filter;
u_int8_t userspace_bpf = 0;
#endif

/*pf_ring��ʼ������*/
u_int8_t wait_for_packet = 1, do_shutdown = 0, add_drop_rule = 0, show_crc = 0;
u_int8_t use_extended_pkt_header = 0, touch_payload = 0, enable_hw_timestamp = 0, dont_strip_timestamps = 0;
/*pf_ring��ʼ������*/

//�Ự��ԭ���岿�ִ��룡��
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
/*********???????PF_RING?????��?��???????????20140626*********/
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

/*�Ự��ԭ���ֳ�ʼ������*/
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

/*Э��ʶ����ģ�����������*/

/************��������������******************/
FilterInfo *WL_rules = NULL;
FilterInfo *WL_rules_copy = NULL;
/************��������������******************/

//����󶨶˿ںų�ʼ������
int  bind_port = 60004;

/////////////////////////////////////////////////////1225////////////////////////////////////////////////////////////
/*�Ự��ԭ���ֳ�ʼ������*/
/*************��Ԫ��ṹ�嶨��**************/
struct tuple5  *Five_attr = NULL;
/*Э��ʶ����ģ�����������*/
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
struct reader_thread {//ndpi �߳̽ṹ��
    struct ndpi_detection_module_struct *ndpi_struct;
    void *ndpi_flows_root[NUM_ROOTS];//��
    char _pcap_error_buffer[PCAP_ERRBUF_SIZE];
    pcap_t *_pcap_handle;
    u_int64_t last_time;//��ǰʱ��
    u_int64_t last_idle_scan_time;//�ϴμ��һ���ʱ���
    u_int32_t idle_scan_idx;//��ǰ��ѯ�������
    u_int32_t num_idle_flows;//��¼��ǰ����ʱ�Ľڵ�����
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
Description: �ͷ����������������Դ
Calls: 
Called By:ndpi_packet_processing
Table Accessed: 
Table Updated: 
Input: �������������
Output: ��
Return: ��
Others: 
 *************************************************/
static void free_ndpi_flow(struct ndpi_flow *flow) {
    if(flow->ndpi_flow) { ndpi_free(flow->ndpi_flow); flow->ndpi_flow = NULL; }
    if(flow->src_id)    { ndpi_free(flow->src_id); flow->src_id = NULL;       }
    if(flow->dst_id)    { ndpi_free(flow->dst_id); flow->dst_id = NULL;       }
}
/*************************************************
Function: node_cmp()
Description: Э��ʶ������жԽڵ���������ݵ��жϡ�
Calls: 
Called By: ndpi_tsearch() -------> get_ndpi_flow()
Table Accessed: 
Table Updated: 
Input: ���Ƚϵ�������
Output: ��Ƚϵ�����һ�·���0�����෵�ط�0
Return: ��Ƚϵ�����һ�·���0�����෵�ط�0
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
Description: Э��ʶ������еĴ洢�ռ����뺯����
Calls: 
Called By: ndpi_init_detection_module() -------> setupDetection()
Table Accessed: 
Table Updated: 
Input: ����ռ��С
Output: ����ɹ��ĵ�ַ�ռ��׵�ַ
Return: ����ɹ��ĵ�ַ�ռ��׵�ַ
Others: 
 *************************************************/
static void *malloc_wrapper(unsigned long size)
{
    return malloc(size);
}
/*************************************************
Function: free_wrapper()
Description: Э��ʶ������е�������洢�ռ��ͷź�����
Calls: 
Called By: ndpi_init_detection_module() -------> setupDetection()
Table Accessed: 
Table Updated: 
Input: ������ռ��׵�ַ
Output: ��
Return: ��
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
Description: Э����������У�ͨ��Ԥ����������޷�ʶ���Э��ŵ��������������˺���
����һ����Ԥ���жϵó�һ��Э��š�
Calls: 
Called By: static unsigned int ndpi_packet_processing
Table Accessed: 
Table Updated: 
Input: ���紫��������
Output: �Ʋ�ó���Э���
Return: �Ʋ�ó���Э���
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
Description: ������
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
Description: �����������Ľڵ�
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
Description: ndpi���Э���ע�ᣬ�Լ���������
Calls: ndpi_init_detection_module��ndpi_set_protocol_detection_bitmask2��ndpi_detection_get_sizeof_ndpi_id_struct��ndpi_detection_get_sizeof_ndpi_flow_struct
Called By:main
Table Accessed: 
Table Updated: 
Input: ��
Output: ��
Return: ��
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
Description: 1��ͨ������������Э�����Դ��Ŀ�Ķ˿ڣ�tcpͨ��ndpi_tcphdr ��udpͨ��ndpi_udphdr�ֱ���в����
2����������ʹ��������ݣ�ͨ��ԴĿ��ip�Ͷ˿ڷ���Ự
3����ndpi_flows_rootΪhash���飬(lower_ip + upper_ip + iph->protocol + lower_port + upper_port) % NUM_ROOTS������Ự��Ӧ������λ�á�Ȼ����������ÿ����Ԫά��һ�������������
4��ͨ��ndpi_tfind�����Զ��������в��ҡ�
Calls: Called By:1�� ndpi_packet_processing() 2�� get_ndpi_flow6()
Table Accessed: 
Table Updated: 
Input: 
Output: ����������Ӧ�ĻỰ���򷵻ض�Ӧ�������������ڣ���ͨ��ndpi_tsearch���µĻỰ����������С�
Return: ����������Ӧ�ĻỰ���򷵻ض�Ӧ�������������ڣ���ͨ��ndpi_tsearch���µĻỰ����������С�
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
Description: ɾ��NDPI�ĳ�ʱ�Ŀ��нڵ㣬ɾ��3�����
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
Description: ���������ÿһ�����ݰ������ݣ��ó���Ӧ��Э�����ͺŲ������������Ԫ��
Calls: 
Called By:pcap_packet_callback
Table Accessed: 
Table Updated: 
Input: 
Output: struct tuple5 *Five_attr��Ԫ����Ϣ
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
        if((ntohs(tcph->source) == 21)||(ntohs(tcph->dest) == 21)){//���ݳ��涨��˿ںţ�21�������ݰ���Э��Ŷ���Ϊ1����ftp������
            Five_attr->four_attr.source = ntohs(tcph->source);
            Five_attr->four_attr.dest = ntohs(tcph->dest);
            Five_attr->four_attr.saddr = iph->saddr;
            Five_attr->four_attr.daddr = iph->daddr;
            Five_attr->protocol = 1;
            return (0);
        }

        if((ntohs(tcph->source) == 25)||(ntohs(tcph->dest) == 25)){//���ݳ���˿ںţ�25�������ݰ���Э��Ŷ���Ϊ3����SMTP������
            //printf("Protocol anaysis function: The protocol is port!!\n\n");
            Five_attr->four_attr.source = ntohs(tcph->source);
            Five_attr->four_attr.dest = ntohs(tcph->dest);
            Five_attr->four_attr.saddr = iph->saddr;
            Five_attr->four_attr.daddr = iph->daddr;
            Five_attr->protocol = 3;
            return (0);
        }
        //������������˿ںŹ������ݰ���Э��Ŷ���Ϊ7����http������
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
Description: �ͷų�ʱ��NDPI����ڵ��
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
Description: �Դ�������ݰ������ݽ��г������������ղ���pcap_pkthdr��Ϣ�������ṹ�������������һ���������������Э�����
Calls: ndpi_packet_processing
Called By:packet_consumer_thread
Table Accessed: 
Table Updated: 
Input: 
struct pcap_pkthdr *header,//����ͷ��Ϣ 
u_char * packet//�������ݰ�
Output: struct tuple5 *Five_attr��������������Ԫ����Ϣ
Return: ��
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
Description: Э������ʶ���̣߳����߳���Ҫ�Ǵ����ݳ���ȡ�����ݲ�����Э������ʶ����ʶ�����ݰ���Э������
����ʶ�������Ԫ����Ϣ�Լ���Ӧ�����ݴ�����һ�����ݻ���صȴ���һ���̴߳���
Calls: rbuff_getdata��pcap_packet_callback��rbuff_putdata
Called By: 
Table Accessed: 
Table Updated: 
Input: �߳���ʱID�ţ���Ҫͨ������CPU��
Output: ��
Return: ��
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
Description: �����������˿ڰ󶨺�������Ҫ������ʹ��һ̨������ֻ����������һ�γ���һ�����������ɹ�����˿ڰ󶨣������Ͻ��޷��ٴ������˳���
Calls: 
Called By: 
int main(int argc, char* argv[])
Table Accessed: 
Table Updated: 
Input: ������˿ں�
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
Description: �ָ��ַ���
Calls: 
Called By: 
int main(int argc, char* argv[])
Table Accessed: 
Table Updated: 
Input: 1. str         Ҫ�ָ���ַ���
2. result     ��ŷָ�����׵�ַ  
3. delim      �ָ��
4. num       Ҫ�ָ�ɼ���
5. size        ÿ���ĳ���
Output: 
Return: ���طָ�ĸ���
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
Description: ����ftpЭ��Ŀ�����
Input: 1. packet_content    ������
2. ftp_four_id           ��Ԫ��
Output: ��
Return: ��
 *************************************************/

void doftp(const char *packet_content, struct tuple4 *ftp_four_id)
{   
    char            *p1=NULL, *p2=NULL, *p3=NULL;
    unsigned short	port1 = 0;  // ��ű����еõ���ip
    unsigned long   ip = 0;     //��ű����еõ��Ķ˿�
    unsigned long   ip_add = 0;
    unsigned char   user_name[64] = {};  //�û��� 
    unsigned char 	file_name[128] = {};  //�����ļ���
    char		    pStr[64] = {}, pResult[6][8];   //�ָ��ַ�����
    //int             address[4] = {}, port[2] = {};
    struct in_addr  addr1;
    hash_bucket     *pBucket = NULL,*TBucket = NULL;	
    FtpSession	    *p_session = NULL;  //ftp�������Ľṹ��
    FtpdataSession  *data_session = NULL;  //ftp �������Ľṹ��
    unsigned  char  flag = 0;
    four_tuple_t    key3;
    //uint32_t        hash_num = 0;
    //int             i = 0;
    int             len = 0;
    //int             n = 0;
    p1 =(char *)(packet_content);
    /*����hashͰ*/
    if ((pBucket = find_hash(ftp_session_tbl, ftp_four_id, sizeof(struct tuple4))) == NULL)
    {        /*�����ڴ�*/

        LS_MEM_MALLOC(pBucket, hash_bucket);
        LS_MEM_MALLOC(p_session, FtpSession);
        memset(p_session,0,sizeof(FtpSession));
        /*��hashͰ��ֵ*/
        memcpy(&p_session->key, ftp_four_id, sizeof(struct tuple4));
        pBucket->content = (void *)p_session;
        pBucket->key = &(p_session->key);
        /*����hashͰ*/
        //  log_printf(ZLOG_LEVEL_INFO, "\n\n$$$$$$$$$$$$$$  step 1 insert_hash $$$$$$$$$$$$$$$$$$$$\n\n");
        insert_hash(ftp_session_tbl, pBucket, sizeof(struct tuple4));
    }
    /*ȡ��hashͰ������*/
    p_session=(FtpSession *)pBucket->content;
    if(p1!=NULL)
    {            
        /*STOR��STOU�ϴ��ļ�������*/
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
                p_session->flag=1;//��1��Ŀ����Ϊ�˵õ�150����ʱ  ֪����һ�������� STOR
                /*��Ϊֻ�ܵõ�һ���˿� ����������Ԫ�� ���ĸ��̶�����*/
                key3.ip_src = ftp_four_id->saddr;
                key3.ip_dst = ftp_four_id->daddr;
                key3.port_src = (p_session->port&0x0000FFFF);	
                key3.port_dst = 0x0000;
                /*����hashͰ*/
                if ((TBucket = find_hash(ftp_session_tbl, &key3, sizeof(struct tuple4))) == NULL)
                {
                    LS_MEM_MALLOC(TBucket, hash_bucket);
                    LS_MEM_MALLOC(data_session, FtpdataSession);
                    memset(data_session,0,sizeof(FtpdataSession));
                    if(p_session->flag2==3)//���������ģʽ����ip
                    {
                        key3.ip_src = ftp_four_id->daddr;
                        key3.ip_dst = ftp_four_id->saddr;
                    }
                    /*��hashͰ��ֵ*/	
                    memcpy(&data_session->key, &key3, sizeof(struct tuple4));
                    memcpy(&data_session->file_name, p_session->file_name, 127);
                    TBucket->content = (void *)data_session;
                    TBucket->key = &(data_session->key);
                    data_session->flag=flag;
                    /*����hashͰ*/
                    //  log_printf(ZLOG_LEVEL_INFO, "\n\n$$$$$$$$$$$$$$  step 2 insert_hash $$$$$$$$$$$$$$$$$$$$\n\n");
                    insert_hash(ftp_session_tbl, TBucket, sizeof(struct tuple4));															
                }   
            }
        }
        /*APPE     �����ļ�����*/				
        if (0 == memcmp(p1, "APPE ", 5))
        {
            p2 = (char *)strstr(p1,"\r\n");
            if(p2 != NULL)
            {
                /*�������ϴ����ļ���*/
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
        /*PORT  ����ģʽ*/
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
            /*�ָ��ַ���*/
            if (parstr(pStr, (char *)pResult, ",", 6, 8) < 6)
            {
                log_printf(ZLOG_LEVEL_WARN, "doftp PORT Matching IP and Port fail!!");
            }                    
            /*������ip�Ͷ˿�*/		  
            ip = ((unsigned long)(atoi(pResult[0])) << 24) |\
                 ((unsigned long)(atoi(pResult[1])) << 16) |\
                 ((unsigned long)(atoi(pResult[2])) << 8) |\
                 ((unsigned long)(atoi(pResult[3])));
            port1 = ((unsigned short)(atoi(pResult[4])) << 8) |\
                    ((unsigned short) (atoi(pResult[5])));
            ip_add =ntohl(ip);
            memcpy(&addr1, &ip_add, 4);
            p_session->flag2=3;  //����ģʽ
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
                /*����hashͰ*/
                if ((TBucket = find_hash(ftp_session_tbl, &key3, sizeof(struct tuple4))) == NULL)
                {
                    LS_MEM_MALLOC(TBucket, hash_bucket);
                    LS_MEM_MALLOC(data_session, FtpdataSession);
                    memset(data_session,0,sizeof(FtpdataSession));
                    if(p_session->flag2==3)//���������ģʽ����ip
                    {
                        key3.ip_src = ftp_four_id->daddr;
                        key3.ip_dst = ftp_four_id->saddr;
                    }
                    memcpy(&data_session->key, &key3, sizeof(struct tuple4));
                    memcpy(&data_session->file_name, p_session->file_name, 127);
                    TBucket->content = (void *)data_session;
                    TBucket->key = &(data_session->key);
                    data_session->flag=flag;
                    /*����hashͰ*/
                    //  log_printf(ZLOG_LEVEL_INFO, "\n\n$$$$$$$$$$$$$$  step 4 insert_hash $$$$$$$$$$$$$$$$$$$$\n\n");
                    insert_hash(ftp_session_tbl, TBucket, sizeof(struct tuple4));															
                }
#if 0 //by niulw
                /*ɾ��hashͰ*/
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
        /*USER   �û���*/	
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
        /*PASS  �û�����*/
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
        /*227 ����ģʽ*/         
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
                    /*�ָ��ַ���*/				
                    if (parstr(pStr, (char *)pResult, ",", 6, 8) < 6)
                    {
                        log_printf(ZLOG_LEVEL_WARN, "doftp no Matching for IP and PORT in 227!!");
                    }                    
                    /*������ip�Ͷ˿�*/			
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
        /*RETR   �����ļ�*/
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
        /*221��421��530     ����ʧ�ܵ�����*/
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
Description: sbuff�źų�ͬ����ʼ������
Calls: 
Called By: sbuff_create  --------> main
Table Accessed: 
Table Updated: 
Input: 
int iringbuffer_ID, �źų�ID
int iringbuffer_qlength �źų��л��λ���������
Output: �ɹ�Ϊ0��ʧ��Ϊ-1
Return: �ɹ�Ϊ0��ʧ��Ϊ-1
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
Description: sring buff�в��ҿ��еĽڵ�
Calls: 
Called By: sbuff_create  --------> main
Table Accessed: 
Table Updated: 
Input: ��
Output: 
Return: �ɹ�Ϊ����ID�ţ�ʧ��Ϊ-1
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
Description: ����sring_buff���λ�����
Calls: 
Called By: sbuff_create  --------> main
Table Accessed: 
Table Updated: 
Input: 
int iringbuffer_ID, �źų�ID
int iringbuffer_qlength �źųػ��λ���������
Output: 
Return: �ɹ�Ϊ����ID�ţ�ʧ��Ϊ-1
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
Description: ����sring_buff���λ�����
Calls: check_for_first_free_sring_buffer
create_sring_buffer
init_slock_for_sync
Called By: main
Table Accessed: 
Table Updated: 
Input: 
int irb_qlength �źųػ��λ���������
Output:
Return: �ɹ�Ϊ����ID�ţ�ʧ��Ϊ-1
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
Description: ���Ѿ���������Ԫ�鼰������Ϣ����sring_buff���λ�������
Calls: 
Called By: session_proc_fun��(do_smtp��do_http_file)--------> tcp_callback��(webmail_mail)--------->http_file_thread��session_consumer_thread
Table Accessed: 
Table Updated: 
Input: 
const int iringbuffer_ID, ʹ���еĻ�����ID��
SESSION_BUFFER_NODE ft����Ԫ�鼰������Ϣ�ṹ��
Output: 
Return: �ɹ�Ϊ1��ʧ��Ϊ-1
Others: 
 ***************************/

int sbuff_putdata(const int iringbuffer_ID, SESSION_BUFFER_NODE ft)
{
    if(sem_wait(&sb_holder[iringbuffer_ID].empty_buffer)==-1)//��û������ź�����Ч
    {
        log_printf(ZLOG_LEVEL_ERROR, "sbuff sem_wait ERROR: sem_wait is failed to lock empty_buffer!!");
        return -1;
    }
    int temp_index;    	
    pthread_mutex_lock(&sb_holder[iringbuffer_ID].pmutex);//�����ٽ����������λ��������ݽڵ�
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
    //��Ӧ�Ļ��λ��������ݸ�ֵ
    sb_holder[iringbuffer_ID].rb_base[temp_index].session_five_attr = ft.session_five_attr; //��Ԫ����Ϣ��ֵ
    sb_holder[iringbuffer_ID].rb_base[temp_index].attr = ft.attr;//�������Ը�ֵ
    sb_holder[iringbuffer_ID].rb_base[temp_index].old_file = ft.old_file;//�ļ������ֵ
    memcpy(sb_holder[iringbuffer_ID].rb_base[temp_index].orgname, ft.orgname, 128);//����ץȡԭʼ�ļ�����ֵ
    memcpy(sb_holder[iringbuffer_ID].rb_base[temp_index].strname, ft.strname, 128);//����ץȡ�洢�ļ�����ֵ
    if(sem_post(&sb_holder[iringbuffer_ID].full_buffer)==-1)//�ͷ���ռ�õ��ź���Դ����Ӧ���ź���ֵ����
    {
        log_printf(ZLOG_LEVEL_ERROR, "sbuff sem_post ERROR: failed to unlock in full_buff!!!!");
        return -1;
    }
    return 1;

}

/***************************
Function: sbuff_getdata()
Description: ����sring_buff���λ���������ȡSESSION_BUFFER_NODE�ṹ����Ϣ
Calls: 
Called By: http_file_thread��session_consumer_thread��tika_consumer_thread��
Table Accessed: 
Table Updated: 
Input: const int iringbuffer_ID, ʹ���еĻ�����ID��
Output: SESSION_BUFFER_NODE ft����Ԫ�鼰������Ϣ�ṹ��
Return: �ɹ�Ϊ1��ʧ��Ϊ-1
Others: 
 ***************************/

int sbuff_getdata(const int iringbuffer_ID, SESSION_BUFFER_NODE *hdr)
{
    if(sem_wait(&sb_holder[iringbuffer_ID].full_buffer)==-1)//�ȴ��ź���Ч
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
    //ȡ���Ѵ���ṹ�����Ϣ����
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
Description: ����sbuff�е�������
Calls: 
Called By: 
Table Accessed: 
Table Updated: 
Input: const int iringbuffer_ID, ʹ���еĻ�����ID��
Output: 
Return: �ɹ�Ϊ1��ʧ��Ϊ-1
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
Description: ring buff�в��ҿ��еĽڵ�
Calls: 
Called By: rbuff_create  --------> main
Table Accessed: 
Table Updated: 
Input: ��
Output: 
Return: �ɹ�Ϊ����ID�ţ�ʧ��Ϊ-1
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
Description: ����ring_buff���λ�����
Calls: 
Called By: rbuff_create  --------> main
Table Accessed: 
Table Updated: 
Input: 
int iringbuffer_ID, �źų�ID
int iringbuffer_qlength �źųػ��λ���������
Output: 
Return: �ɹ�Ϊ����ID�ţ�ʧ��Ϊ-1
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
Description: rbuff�źų�ͬ����ʼ������
Calls: 
Called By: rbuff_create  --------> main
Table Accessed: 
Table Updated: 
Input: 
int iringbuffer_ID, �źų�ID
int iringbuffer_qlength �źų��л��λ���������
Output: 
Return: �ɹ�Ϊ0��ʧ��Ϊ-1
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
Description: ����ring_buff���λ�����
Calls: check_for_first_free_ring_buffer
create_ring_buffer
init_lock_for_sync
Called By: main
Table Accessed: 
Table Updated: 
Input: 
int irb_qlength �źųػ��λ���������
Output:
Return: �ɹ�Ϊ����ID�ţ�ʧ��Ϊ-1
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
Description: ���Ѿ���������Ԫ�鼰���ݰ���Ϣ����ring_buff���λ�������
Calls: 
Called By: dummyProcessPacket��packet_consumer_thread
Table Accessed: 
Table Updated: 
Input: 
const int iringbuffer_ID, ʹ�õĻ�����ID��
struct tuple5 *session_five_attr  ��Ԫ�鼰������Ϣ�ṹ��
struct pcap_pkthdr *hdr�����ݰ�ͷ����Ϣ  
void *pData���ɼ������ݰ�
Output: 
Return: �ɹ�Ϊ1��ʧ��Ϊ-1
Others: 
 ***************************/

int rbuff_putdata(const int iringbuffer_ID, void *pData, struct pcap_pkthdr *hdr, struct tuple5 *session_five_attr)
{
    if(sem_wait(&rb_holder[iringbuffer_ID].empty_buffer)==-1)//�ȴ��ź�����Ч
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
    if(session_five_attr != NULL){//��Ԫ����Ϣ��Ч�Ը�ֵ
        five_attr.four_attr.saddr = session_five_attr->four_attr.saddr;
        five_attr.four_attr.daddr = session_five_attr->four_attr.daddr;
        five_attr.four_attr.source = session_five_attr->four_attr.source;
        five_attr.four_attr.dest= session_five_attr->four_attr.dest;
        five_attr.protocol = session_five_attr->protocol;
        //five_attr.flow_flag = session_five_attr->flow_flag;
        //five_attr.flow = session_five_attr->flow;
    }

    if((temp_hdr.caplen>0)&&(temp_hdr.caplen<=65536)){//�жϲɼ����ݰ��Ƿ���Ч
        rb_holder[iringbuffer_ID].rb_base[rb_holder[iringbuffer_ID].irb_tail_index].pdata_buffer = ikmem_malloc(temp_hdr.caplen);// Allocating memory
        if(rb_holder[iringbuffer_ID].rb_base[rb_holder[iringbuffer_ID].irb_tail_index].pdata_buffer == NULL){ // Checks Memory  Allocated
            log_printf(ZLOG_LEVEL_ERROR, "ring buff put ERROR: the put packet data pointer is NULL!!");
            return -1;
        }else{
            if(memcpy(rb_holder[iringbuffer_ID].rb_base[rb_holder[iringbuffer_ID].irb_tail_index].pdata_buffer,pData,temp_hdr.caplen)==NULL)//ת������
            {   
                ikmem_free(rb_holder[iringbuffer_ID].rb_base[rb_holder[iringbuffer_ID].irb_tail_index].pdata_buffer);//�ͷ�֮ǰ�ڴ�
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
    //��ֵ���ݰ�ͷ��Ϣ
    rb_holder[iringbuffer_ID].rb_base[rb_holder[iringbuffer_ID].irb_tail_index].pkthdr.len = temp_hdr.len;//���ݰ�����
    rb_holder[iringbuffer_ID].rb_base[rb_holder[iringbuffer_ID].irb_tail_index].pkthdr.caplen = temp_hdr.caplen;//�ɼ����ݰ�����
    rb_holder[iringbuffer_ID].rb_base[rb_holder[iringbuffer_ID].irb_tail_index].pkthdr.ts.tv_sec = temp_hdr.ts.tv_sec;//�ɼ�ʱ��(��)
    rb_holder[iringbuffer_ID].rb_base[rb_holder[iringbuffer_ID].irb_tail_index].pkthdr.ts.tv_usec = temp_hdr.ts.tv_usec;//�ɼ�ʱ��(΢��)
    if(session_five_attr != NULL)//��Ԫ����Ϣ��Ч�Ը�ֵ
    {
        rb_holder[iringbuffer_ID].rb_base[rb_holder[iringbuffer_ID].irb_tail_index].session_five_attr.ip_src = five_attr.four_attr.saddr;
        rb_holder[iringbuffer_ID].rb_base[rb_holder[iringbuffer_ID].irb_tail_index].session_five_attr.ip_dst = five_attr.four_attr.daddr;
        rb_holder[iringbuffer_ID].rb_base[rb_holder[iringbuffer_ID].irb_tail_index].session_five_attr.port_src= five_attr.four_attr.source;
        rb_holder[iringbuffer_ID].rb_base[rb_holder[iringbuffer_ID].irb_tail_index].session_five_attr.port_dst= five_attr.four_attr.dest;
        rb_holder[iringbuffer_ID].rb_base[rb_holder[iringbuffer_ID].irb_tail_index].session_five_attr.protocol = five_attr.protocol;
        //rb_holder[iringbuffer_ID].rb_base[rb_holder[iringbuffer_ID].irb_tail_index].session_five_attr.flow_flag = five_attr.flow_flag;
        //rb_holder[iringbuffer_ID].rb_base[rb_holder[iringbuffer_ID].irb_tail_index].session_five_attr.flow = five_attr.flow;
    }
    //�޸Ļ��λ�������ַ
    if(rb_holder[iringbuffer_ID].irb_tail_index >= rb_holder[iringbuffer_ID].irb_buffer_end_index )
    {
        rb_holder[iringbuffer_ID].irb_tail_index = 0;
    }
    else
    {
        rb_holder[iringbuffer_ID].irb_tail_index++;
    }

    if(sem_post(&rb_holder[iringbuffer_ID].full_buffer)==-1)//�ͷ�ռ�õ��ź���
    {
        log_printf(ZLOG_LEVEL_ERROR, "ring buff put ERROR: sem_post is failed to unlock in full_buffer!!");
        return -1;
    }
    return 1;
}

/**Getting data from ring buffer**/
/***************************
Function: rbuff_getdata()
Description: ����ring_buff���λ���������ȡ���ݰ��Լ���Ӧ��ͷ��Ϣ����Ԫ����Ϣ
Calls: 
Called By: session_process_thread��packet_consumer_thread
Table Accessed: 
Table Updated: 
Input: const int iringbuffer_ID, ʹ�õĻ�����ID��
Output: void *pData  �������е�����
struct pcap_pkthdr *hdr  ���ݰ�ͷ����Ϣ
struct tuple5 *session_five_attr  ��Ԫ����Ϣ
Return: �ɹ�Ϊ1��ʧ��Ϊ-1
Others: 
 ***************************/

int rbuff_getdata(const int iringbuffer_ID, void *pData, struct pcap_pkthdr *hdr, struct tuple5 *session_five_attr)
{
    if(sem_wait(&rb_holder[iringbuffer_ID].full_buffer)==-1)//�ȴ�ϵͳ�ź���
    {
        if(rb_holder[iringbuffer_ID].rb_base[rb_holder[iringbuffer_ID].irb_head_index].pdata_buffer != NULL)//����ȴ�ʧЧ���ͷ���Դ
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

    // �ж����ݰ����ݵ���Ч��
    if((rb_holder[iringbuffer_ID].rb_base[rb_holder[iringbuffer_ID].irb_head_index].pkthdr.caplen>0)&&(rb_holder[iringbuffer_ID].rb_base[rb_holder[iringbuffer_ID].irb_head_index].pkthdr.caplen<=65536))
    {
        if(rb_holder[iringbuffer_ID].rb_base[rb_holder[iringbuffer_ID].irb_head_index].pdata_buffer != NULL)
        {
            if(NULL == memcpy(pData,rb_holder[iringbuffer_ID].rb_base[rb_holder[iringbuffer_ID].irb_head_index].pdata_buffer,
                        rb_holder[iringbuffer_ID].rb_base[rb_holder[iringbuffer_ID].irb_head_index].pkthdr.caplen)){//ת�����ݰ�
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
    //ת�����ݰ�ͷ��Ϣ
    hdr->len = rb_holder[iringbuffer_ID].rb_base[rb_holder[iringbuffer_ID].irb_head_index].pkthdr.len;
    hdr->caplen = rb_holder[iringbuffer_ID].rb_base[rb_holder[iringbuffer_ID].irb_head_index].pkthdr.caplen;
    hdr->ts.tv_sec = rb_holder[iringbuffer_ID].rb_base[rb_holder[iringbuffer_ID].irb_head_index].pkthdr.ts.tv_sec;
    hdr->ts.tv_usec = rb_holder[iringbuffer_ID].rb_base[rb_holder[iringbuffer_ID].irb_head_index].pkthdr.ts.tv_usec;
    //ת�����ݰ���Ԫ����Ϣ
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
    //�ͷ�֮ǰ��Դ
    ikmem_free(rb_holder[iringbuffer_ID].rb_base[rb_holder[iringbuffer_ID].irb_head_index].pdata_buffer);
    rb_holder[iringbuffer_ID].rb_base[rb_holder[iringbuffer_ID].irb_head_index].pdata_buffer = NULL;
    rb_holder[iringbuffer_ID].rb_base[rb_holder[iringbuffer_ID].irb_head_index].pkthdr.len = 0;
    rb_holder[iringbuffer_ID].rb_base[rb_holder[iringbuffer_ID].irb_head_index].pkthdr.caplen = 0;
    rb_holder[iringbuffer_ID].rb_base[rb_holder[iringbuffer_ID].irb_head_index].pkthdr.ts.tv_sec = 0;
    rb_holder[iringbuffer_ID].rb_base[rb_holder[iringbuffer_ID].irb_head_index].pkthdr.ts.tv_usec = 0;
    //updating head pointer
    if(rb_holder[iringbuffer_ID].irb_head_index >= rb_holder[iringbuffer_ID].irb_buffer_end_index)//�޸Ļ��λ�������ַָ��
    {
        rb_holder[iringbuffer_ID].irb_head_index=0;
    }
    else        
    {
        rb_holder[iringbuffer_ID].irb_head_index++;
    }

    if(sem_post(&rb_holder[iringbuffer_ID].empty_buffer)==-1)//�ͷ�ϵͳ�ź���
    {
        log_printf(ZLOG_LEVEL_ERROR, "ring buff get ERROR: sem_post is failed to unlock in empty_buffer!!");
        return -1;
    }
    return 1; // return size  in terms of byte successfully Written to buffer
}

/**Destroying Ring Buffer**/
/***************************
Function: rbuff_destroy()
Description: ����buff�е�������
Calls: 
Called By: 
Table Accessed: 
Table Updated: 
Input: const int iringbuffer_ID, ʹ�õĻ�����ID��
Output: 
Return: �ɹ�Ϊ1��ʧ��Ϊ-1
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
Description: nids��ip����
Calls: 
Called By: 
Table Accessed: 
Table Updated: 
Input:  *x   ip�ṹ��
len  ����
Output: 
Return: �ɹ�Ϊ1��ʧ��Ϊ-1
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
Description: nids�����й����е�log��¼
Calls: 
Called By: 
Table Accessed: 
Table Updated: 
Input:  type   ��־����
errnum �����
iph    ip��Ϣ
data   ����ʵ��
Output: 
Return: ��
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
Description: ip���ݰ���������ͨ���˺�������nidsע�ắ����ip���ݰ������������
Calls: 
Called By: nids_pcap_handler
Table Accessed: 
Table Updated: 
Input:  data   �������ݰ�
caplen ���ݰ���С
hdr    ���ݰ�ץȡ��Ϣ
Five_attr   ��Ԫ����Ϣ
Output: ��
Return: ��
Others: 
 ***************************/

static void call_ip_frag_procs(void *data,bpf_u_int32 caplen, struct pcap_pkthdr *hdr, struct tuple5 *Five_attr)
{
    struct proc_node *i;
    for (i = ip_frag_procs; i; i = i->next)//��ѯע�ắ��������ȡ������
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
  Description: �Ự���鴦������ͨ���˺�������TCP�������ĻỰ����
  Calls: 
  Called By: session_process_thread
  Table Accessed: 
  Table Updated: 
  Input:  data   �������ݰ�
  caplen ���ݰ���С
  hdr    ���ݰ�ץȡ��Ϣ
  Five_attr   ��Ԫ����Ϣ
  Output: ��
  Return: ��
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
    switch (linktype) {//�ж�Ӳ��ϵͳ��������
        case DLT_EN10MB://��ǰϵͳ����ʹ�õ��������ͣ����ݴ����ͳ�ʼ������
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
    if(nids_params.multiproc) { //�������libgthread2.0�Ļ����������̴߳�����
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
    } else { /* user requested simple passthru - no threading����ϵͳ���õ��̴߳���ʽ */
        call_ip_frag_procs(data_aligned,hdr->len - nids_linkoffset, hdr, Five_attr);
    }
#else
    call_ip_frag_procs(data_aligned, hdr->len - nids_linkoffset, hdr, Five_attr);
#endif
}

/***************************
Function: gen_ip_frag_proc()
Description: ip���ݰ����������ж����ݰ��Ƿ�Ϊ�����ϵ���Ƭ�����������Ƭ��������Ƭ����������ǣ����ûỰ���麯�����лỰ��������
Calls: 
Called By: call_ip_frag_procs
Table Accessed: 
Table Updated: 
Input:  data   �������ݰ�
len ���ݰ���С
hdr    ���ݰ�ץȡ��Ϣ
Five_attr   ��Ԫ����Ϣ
Output: ��
Return: ��
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
    //���Ƚ���ip����
    if (!nids_params.ip_filter(iph, len))
        return;
    //�ж����ݰ���Ϣ�Ƿ���ϱ�׼����������������������
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
    switch (ip_defrag_stub((struct ip *) data, &iph)) {//�жϷ��ϵ����ݰ��Ƿ�Ϊ�µ�������Ƭ
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
    if (!need_free)//�������Ƭ�����в������޸�
        skblen += nids_params.dev_addon;
    skblen = (skblen + 15) & ~15;
    skblen += nids_params.sk_buff_size;

    for (i = ip_procs; i; i = i->next)//����TCP���麯�����лỰ����
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
Description: �Ự���鴦������ͨ���˺�������TCP�������ĻỰ����
Calls: 
Called By: gen_ip_frag_proc
Table Accessed: 
Table Updated: 
Input:  data   �������ݰ�
caplen ���ݰ���С
hdr    ���ݰ�ץȡ��Ϣ
Five_attr   ��Ԫ����Ϣ
Output: ��
Return: ��
Others: 
 ***************************/

static void gen_ip_proc(u_char * data, int skblen, struct pcap_pkthdr *hdr, struct tuple5 *Five_attr)
{
    switch (((struct ip *) data)->ip_p) {
        case IPPROTO_TCP:  //TCP���ݰ�����
            process_tcp(data, skblen, hdr, Five_attr);
            break;
        default:
            break;
    }
}

/***************************
Function: init_procs()
Description: ip���ݰ������������ʼ������Ҫ��������Ҫ���õĴ�������ӵ���Ӧ������ڵ���
Calls: 
Called By: nids_init
Table Accessed: 
Table Updated: 
Input: ��
Output: ��
Return: ��
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
Description: ע��udp����
Calls: 
Called By: nids_init
Table Accessed: 
Table Updated: 
Input: *x  ��ں���
Output: ��
Return: ��
Others: 
 ***************************/

void nids_register_udp(void (*x))
{
    register_callback(&udp_procs, x);
}

/***************************
Function: nids_unregister_udp()
Description: ж��udp����
Calls: 
Called By: nids_init
Table Accessed: 
Table Updated: 
Input: *x  ��ں���
Output: ��
Return: ��
Others: 
 ***************************/

void nids_unregister_udp(void (*x))
{
    unregister_callback(&udp_procs, x);
}

/***************************
Function: nids_register_ip()
Description: ע��ip���ݰ�������
Calls: 
Called By: nids_init
Table Accessed: 
Table Updated: 
Input: *x  ��ں���
Output: ��
Return: ��
Others: 
 ***************************/

void nids_register_ip(void (*x))
{
    register_callback(&ip_procs, x);
}

/***************************
Function: nids_unregister_ip()
Description: ж��ip���ݰ�������
Calls: 
Called By: nids_init
Table Accessed: 
Table Updated: 
Input: *x  ��ں���
Output: ��
Return: ��
Others: 
 ***************************/

void nids_unregister_ip(void (*x))
{
    unregister_callback(&ip_procs, x);
}

/***************************
Function: nids_register_ip_frag()
Description: ע��ip���ݰ���Ƭ������
Calls: 
Called By: nids_init
Table Accessed: 
Table Updated: 
Input: *x  ��ں���
Output: ��
Return: ��
Others: 
 ***************************/

void nids_register_ip_frag(void (*x))
{
    register_callback(&ip_frag_procs, x);
}

/***************************
Function: nids_unregister_ip_frag()
Description: ж��ip���ݰ���Ƭ������
Calls: 
Called By: nids_init
Table Accessed: 
Table Updated: 
Input: *x  ��ں���
Output: ��
Return: ��
Others: 
 ***************************/


void nids_unregister_ip_frag(void (*x))
{
    unregister_callback(&ip_frag_procs, x);
}

/***************************
Function: nids_init()
Description: nids��ʼ������
Calls: init_procs();//��������ʼ��
tcp_init(nids_params.n_tcp_streams);//tcp������̵ĳ�ʼ������
ip_frag_init(nids_params.n_hosts);//����������ʼ��
scan_init();
Called By: main
Table Accessed: 
Table Updated: 
Input: ��
Output: ��
Return: ��
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

    //���������������ͳ�ʼ������
    linktype = DLT_EN10MB;
    nids_linkoffset = 14;
    nids_params.dev_addon = 16;

    if (nids_params.syslog == nids_syslog)//����nidslog��¼�ļ�
        openlog("libnids", 0, LOG_LOCAL0);

    init_procs();//��������ʼ��
    tcp_init(nids_params.n_tcp_streams);//tcp������̵ĳ�ʼ������
    ip_frag_init(nids_params.n_hosts);//����������ʼ��
    scan_init();

    return 1;
}

/**********************************/
// struct tuple4 contains addresses and port numbers of the TCP connections
// the following auxiliary function produces a string looking like
// 10.0.0.1,1024,10.0.0.2,23
/***************************
Function: adres()
Description: ip��ַת�������������ݽṹ�е�ip��ַת��Ϊ*.*.*.*,*��ʽ
Calls: 
Called By: tcp_callback
Table Accessed: 
Table Updated: 
Input: ��
Output: ��
Return: ��
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
Description: tcp�Ự����������Ҫ����http��smtp�Լ�ftp����Э����ļ������Լ�http���ӹرղ��ֲ���
Calls: http��gmime_parse_mailbody
Called By: session_consumer_thread
Table Accessed: 
Table Updated: 
Input: SESSION_BUFFER_NODE *arg
Output: SESSION_BUFFER_NODE *hdr
Return: ���ݴ�����1������������0
Others: 
 ***************************/

uint16_t  session_num=0, ftp_num=0;
int session_proc_fun(SESSION_BUFFER_NODE *arg, SESSION_BUFFER_NODE *hdr)
{
    //	  log_printf(ZLOG_LEVEL_INFO, "\n\n################## session_proc_fun ===========================111=\n\n");
    four_tuple_t temp_f;
    //���������ʱ�洢����ֹ���̵߳���ʱ��������
    temp_f.ip_src = arg->session_five_attr.ip_src;
    temp_f.ip_dst = arg->session_five_attr.ip_dst;
    temp_f.port_src = arg->session_five_attr.port_src;
    temp_f.port_dst = arg->session_five_attr.port_dst;
    hdr->session_five_attr.protocol= arg->session_five_attr.protocol;   
    if(hdr->session_five_attr.protocol==7){//���Э���Ϊ7������httpЭ�飬�˴���Ҫ�����������ӹرպ�������Դ���ͷ�
        struct tuple4         addr;
        hash_bucket           *httpBucket=NULL;  
        HttpSession           *http_session=NULL;
        dlp_http_post_head    *dlp_session=NULL;
        addr.saddr = temp_f.ip_src;
        addr.daddr = temp_f.ip_dst;
        addr.source = temp_f.port_src;
        addr.dest = temp_f.port_dst;
        httpBucket = find_hash(ftp_session_tbl, &addr, sizeof(struct tuple4));//����hash��
        if(httpBucket != NULL)//hash��Ϊ�ձ�ʾ����ͨ��httpЭ���ϴ������ݣ���˴˴���Ҫ��ʹ�õ�����ɾ���ͷſռ�
        {    
            if(httpBucket->content!=NULL){
                http_session = (HttpSession *)httpBucket->content;

                remove_hash(ftp_session_tbl, httpBucket, sizeof(struct tuple4));//�Ƴ�hash��ڵ�

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
    else if(hdr->session_five_attr.protocol==1){ //Э���Ϊ1��ʾftp��������������ǰ�ڴ����ʱ���Ѿ��ͷ���ռ䣬���Դ˴��������Ĵ���  
        log_printf(ZLOG_LEVEL_INFO, "session func INFO: the protocol is number 1!!");
        return 0;
    }
    else if(hdr->session_five_attr.protocol == 250)//Э���Ϊ250��ʾhttpЭ�鴦��׶�1��ɣ�����post���ݴ洢��ɵ�����Ҫ��һ������
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

        //��ǰ�ڴ�������ݿ�����ṹ���е�ָ��λ��
        http_session_temp=(dlp_http_post_head *)arg->attr;//ָ��ȥ��ֵ

        memcpy(http_session, http_session_temp, sizeof(dlp_http_post_head));


        // int len =strlen(http_session_temp->cookie);
        // http_session->cookie=malloc(len);
        //memcpy(http_session->cookie, http_session_temp->cookie, len);

        memcpy(sp_session.old_name, arg->orgname, PATHLEN);//ԭʼ�ļ�����ֵ
        memcpy(http_session->new_name, arg->strname, PATHLEN);//�洢�ļ�����ֵ
        sp_session.http = http_session;//ָ��ȥ��ֵ
        //����Ϊ��Ԫ����Ϣ��ֵ
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
            return 0;//����http���ݰ���������������ݰ�ͷ����Ϣ�Լ�body���ֵ���ȡ
        }
        if(http_session->content_length == 0)
        {
            log_printf(ZLOG_LEVEL_WARN, "session func WARN: the body len is zero!!");
        }	
        //����ȡ����Ϣ��ֵ���źų��е��źŽڵ�ȴ������Ľ�һ������
        ft.attr = sp_session.http;
        ft.session_five_attr.ip_src = sp_session.key.saddr;
        ft.session_five_attr.ip_dst = sp_session.key.daddr;
        ft.session_five_attr.port_src = sp_session.key.source;
        ft.session_five_attr.port_dst = sp_session.key.dest;
        ft.session_five_attr.protocol = 255;



        sbuff_putdata(http_buffer_id, ft); //����������źŽڵ���Ϣ�����źųصȴ�����
        return 0;
    }
    else if(hdr->session_five_attr.protocol==3)//Э���Ϊ3��ʾsmtpЭ�飬�˴�����ͷ����Ϣ�Լ�������Ϣ����ȡ
    {	 
        struct tuple4   addr;
        hash_bucket     *smtpBucket=NULL;	
        SmtpSession_t	  *smtp_session=NULL;
        addr.saddr = temp_f.ip_src;
        addr.daddr = temp_f.ip_dst;
        addr.source = temp_f.port_src;
        addr.dest = temp_f.port_dst;
        smtpBucket = find_hash(ftp_session_tbl, &addr, sizeof(struct tuple4));//����ϵͳhash���ж��Ƿ���smtp��Ϣ����
        if(smtpBucket != NULL)//hashͰ�����ݱ�ʾsmtp�ػ�ɹ�
        {
            smtp_session = (SmtpSession_t  *)smtpBucket->content; 
            if (smtp_session->smtp_session_state.dot_seen == TRUE)
            {
                while(smtp_session->next != NULL)	
                {
                    smtp_session = smtp_session->next;
                }
                char *jsonstr = NULL;
                gmime_parse_mailbody (smtp_session, &jsonstr);//smtp�ʼ��������̿�ʼ

                if (jsonstr )
                {
                    /* json����Ͱ */
                    smtp_session->result_json = jsonstr;
                }
                else
                {
                    log_printf(ZLOG_LEVEL_ERROR, "session func ERROR: the jason pointer is NULL!!");
                    return 0;
                }
                //��ֵ��Ԫ����Ϣ���ȴ�������һ������
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
    else //����δ֪Э��Ŵ������̣��˴���Ҫ����ftp��������������
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
        if(pBucket!=NULL){//hashͰ�е����ݴ��ڣ���ʾδ֪Э��Ϊftp������
            //������ʼ��

            data_session = (FtpdataSession  *)pBucket->content;
            memcpy(hdr->orgname, data_session->file_name, 128);//���͵�ԭʼ�ļ�����ֵ
            remove_hash(ftp_session_tbl, pBucket, sizeof(struct tuple4));//ɾ��hash��ڵ�
            //�ͷ�����ռ�
            if(pBucket->content != NULL) { 
                free(pBucket->content);
                pBucket->content = NULL;
            }
            if(pBucket != NULL){
                free(pBucket);
                pBucket = NULL;
            }
            memcpy(hdr->strname, arg->strname, PATHLEN);//���ݲ���洢�ļ���

            //��Ԫ����Ϣ��ֵ
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
                //������ʼ��

                data_session = (FtpdataSession  *)pBucket->content;
                memcpy(hdr->orgname, data_session->file_name, 128);//���͵�ԭʼ�ļ�����ֵ
                remove_hash(ftp_session_tbl, pBucket, sizeof(struct tuple4));//ɾ��hash��ڵ�
                //�ͷ�����ռ�
                if(pBucket->content != NULL) { 
                    free(pBucket->content);
                    pBucket->content = NULL;
                }
                if(pBucket != NULL){
                    free(pBucket);
                    pBucket = NULL;
                }
                memcpy(hdr->strname, arg->strname, PATHLEN);//���ݲ���洢�ļ���

                //��Ԫ����Ϣ��ֵ
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
Description: tcp�Ự����ص���������Ҫ����http��smtp�Լ�ftp����Э��
Calls: do_smtp��do_http_file
Called By: nids_register_tcp-------->main
Table Accessed: 
Table Updated: 
Input: a_tcp tcp������
this_time_not_needed ϵͳδʹ��
Output: ��
Return: ��
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
    strcpy(buf, adres(a_tcp->addr));//��ַ��ʽת��
    //����������̰���tcp����״̬���д����ֱ�Ϊ��������EST�����ݴ���DATA�����ӹر�CLOSE����������RESET�ĸ����̽��д���
    if (a_tcp->nids_state == NIDS_JUST_EST) {//���ӽ������̣���ʼ������
        a_tcp->client.collect++;	// we want data received by a client
        a_tcp->server.collect++;	// and by a server, too
        a_tcp->server.collect_urg++;	// we want urgent data received by a
#ifdef WE_WANT_URGENT_DATA_RECEIVED_BY_A_CLIENT
        a_tcp->client.collect_urg++;	// if we don't increase this value,
#endif
        return;
    }
    if (a_tcp->nids_state == NIDS_DATA) //���ݴ���׶Σ�����ʶ�����Э��Ž���Э�鴦��
    {
        if (a_tcp->client.count_new) //�����ݵ������Է�����е���
        {
            hlf = &a_tcp->client;	// from now on, we will deal with hlf var,
        } else {
            hlf = &a_tcp->server;	// analogical
        }
        number = hlf->count_new;
        if(number>0)//������ݰ����ݲ�Ϊ0���������ݴ���
        {	
            ftp_four.saddr = a_tcp->addr.saddr;
            ftp_four.daddr = a_tcp->addr.daddr;
            ftp_four.source = a_tcp->addr.source;
            ftp_four.dest = a_tcp->addr.dest;
            protocol = a_tcp->addr5.protocol;
            if(protocol==1)//ftp������Э�����
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
            if(protocol==3)//smtpЭ������������
            {

                do_smtp(hlf->data, &ftp_four, number, sbuff_putdata, sbuffer_id);
                return;
            }
            if(protocol==7)//httpЭ������������
            {
                do_http_file(hlf->data, number, &ftp_four, sbuff_putdata, sbuffer_id);
                return;
            }
            if(((ftp_four.source != 21)&&(ftp_four.dest != 21))&&((ftp_four.source != 443)&&(ftp_four.dest != 443)))//����Э�鴦�����Ϊ������ֱ�Ӵ洢����ϵͳ��ʱ��������ܶ˿ڷ��͵����ݣ����������ftp�������Ķ˿�����Ҳ��������
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
    if (a_tcp->nids_state == NIDS_CLOSE||a_tcp->nids_state == NIDS_TIMED_OUT) //tcp���ӹ�����
    {
        // printf("This is tcp close, the flow_flag is %d!!\n",a_tcp->addr5.flow_flag);
        //	  log_printf(ZLOG_LEVEL_INFO, "\n\n===== sun bj==========tcp_callback NIDS Close a_tcp->addr5.protocol = %d==================\n\n",a_tcp->addr5.protocol);

        if((a_tcp->addr5.protocol != 3)&&(a_tcp->addr5.protocol != 1))//�������smtp�Լ�ftp������������������
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
    if (a_tcp->nids_state == NIDS_RESET) //tcp�������ô���
    {
        //printf("This is tcp reset,the flow_flag is %d!!\n",a_tcp->addr5.flow_flag);

        // log_printf(ZLOG_LEVEL_INFO, "\n\n===============tcp_callback NIDS RESET a_tcp->addr5.protocol = %d==================\n\n",a_tcp->addr5.protocol);
        if((a_tcp->addr5.protocol != 3)&&(a_tcp->addr5.protocol != 1))//�������smtp�Լ�ftp������������������
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
Description: ���ݰ��������򴴽�������ϵͳ��δʹ��
Calls: 
Called By: 
Table Accessed: 
Table Updated: 
Input: 
Output: ��
Return: ��
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
Description: pfring��bpf������뺯��
Calls: 
Called By: main
Table Accessed: 
Table Updated: 
Input: filter_buffer�������������
caplen����������
Output: ��
Return: ��
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
Description: mac��ַת�����ַ���
Calls: 
Called By: main
Table Accessed: 
Table Updated: 
Input: ep������������ݴ�
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
Description: �˺�����Ҫ��pfring_loop�����Ļص��������ȴ����������ݰ��ĵ������������ݰ�
��������ʱ���㽫���ݰ�����������������ݻ�����в�д���źŵȴ���һ���̴߳���
Calls: rbuff_putdata
Called By: pfring_loop------>main
Table Accessed: 
Table Updated: 
Input: const struct pfring_pkthdr *h���ɼ�����ͷ��Ϣ
const u_char *p�����ݰ�
const u_char *user_bytes���˴�����Чֵ
Output: ��
Return: ��
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
    //ת�����ݰ�ץȡ��ͷ��Ϣ
    pkthdr.len = h->len;
    pkthdr.caplen = h->caplen;
    pkthdr.ts.tv_sec = h->ts.tv_sec;
    pkthdr.ts.tv_usec = h->ts.tv_usec;
    //	log_printf(ZLOG_LEVEL_DEBUG, "\n\n===================== Step 1 Dummy Capture Data Package Enter into =====================\n\n");
    if(p != NULL)
        rbuff_putdata(buffer_id, p, &pkthdr, NULL);//��������ץȡ�����ݰ�����ϵͳ�����ݻ����
    //log_printf(ZLOG_LEVEL_DEBUG, "\n\n===================== Step 1 Dummy Capture Data Package Exit =====================\n\n");
}



/***************************
Function: session_process_thread()
Description: ����Ự�����̣߳����߳���Ҫ�Ǵ����ݳ���ȡ�����ݲ������Ự���麯�����б��Ự���ݵ�����
��������������ͨ���ڲ�ע��Ļص�����tcp_callback���д���
Calls: rbuff_getdata��nids_pcap_handler
Called By: 
Table Accessed: 
Table Updated: 
Input: �߳���ʱID�ţ���Ҫͨ������CPU��
Output: ��
Return: ��
Others: 
 ***************************/

void* session_process_thread(void* _id) {
    u_int numCPU = sysconf( _SC_NPROCESSORS_ONLN );
    long thread_id = (long)_id;
    u_long core_id = thread_id % numCPU;

    if((num_threads > 1) && (numCPU > 1)) {
        if(bind2core(core_id) == 0)//�߳���cpu��
            log_printf(ZLOG_LEVEL_INFO, "session process INFO: Set thread %lu on core %lu/%u !!", thread_id, core_id, numCPU);
    }
    u_char *sbuffer_p;// = buffer;

    struct pcap_pkthdr *shdr = NULL;
    struct tuple5 *sess_five = NULL;
    shdr = malloc(sizeof(struct pcap_pkthdr));//���ݰ���Ϣͷ�洢�ṹ��
    if(shdr==NULL){
        log_printf(ZLOG_LEVEL_FATAL, "session process FATAL: malloc pcap pkthdr failed!!");
        return(NULL);
    }
    sess_five = malloc(sizeof(struct tuple5));//��Ԫ����Ϣ�洢�ṹ��
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
        if(rbuff_getdata(bbuffer_id, sbuffer_p, shdr, sess_five)>0){//���ݻ��������ȡ����
            //printf("sess_five flow_flag is %d\n",sess_five->flow_flag);
            //log_printf(ZLOG_LEVEL_DEBUG, "\n\n===================== Step 3 session_process_thread nids_pcap_handler Enter into  =====================\n\n");
            nids_pcap_handler(NULL, shdr, sbuffer_p, sess_five);//����ȡ�����ݴ���Ự���麯�����лỰ����
            //	log_printf(ZLOG_LEVEL_DEBUG, "\n\n===================== Step 3 session_process_thread nids_pcap_handler Exit  =====================\n\n");
        }
    }
    return(NULL);
}

/***************************
Function: WL_upload_thread()
Description: �����������̣߳����߳���Ҫ�ǵȴ����߳��·��İ����������źţ������������ж�����������·��ɹ������̱߳㽫
�����ڴ����еı��������µ����أ�������·�ȫ�ֲ������£������ȫ�ֲ����ĸ��£�
Calls: receive_msg
Called By: 
Table Accessed: 
Table Updated: 
Input: �߳���ʱID�ţ���Ҫͨ������CPU��
Output: ��
Return: ��
Others: 
 ***************************/

void* WL_upload_thread(void* _id) {
    u_int numCPU = sysconf( _SC_NPROCESSORS_ONLN );
    long thread_id = (long)_id;
    u_long core_id = thread_id % numCPU;
    int ret = 0;
    struct file_msg conf_msg;
    if((num_threads > 1) && (numCPU > 1)) {
        if(bind2core(core_id) == 0)//�̰߳�cpu
            log_printf(ZLOG_LEVEL_INFO, "session process INFO: Set thread %lu on core %lu/%u !!", thread_id, core_id, numCPU);
    }

    while(1){
        ret = receive_msg(&conf_msg);//�ȴ������������ź�
        log_printf(ZLOG_LEVEL_INFO,"################## White list = %s\n",conf_msg.mtext);
        if (ret == 1) {//����ȴ��ɹ����ж��ź����ͽ�����Ӧ����
            log_printf(ZLOG_LEVEL_INFO,"WL_rules->use_flag and WL_rules_copy->use_flag is %d and %d",WL_rules->use_flag ,WL_rules_copy->use_flag);
            log_printf(ZLOG_LEVEL_INFO,"WL_rules->up_flag and WL_rules_copy->up_flag is %d and %d",WL_rules->up_flag ,WL_rules_copy->up_flag);
            //ͨ������json��������ʶ������Ĺؼ���
            json_t *conf_str = json_loads(conf_msg.mtext, 0, NULL);
            json_t *command = json_object_get(conf_str, "command");
            const char* cmd = json_string_value(command);
            if(NULL!=cmd)
                log_printf(ZLOG_LEVEL_INFO,"\n\n##### command = %s\n\n",cmd);	
            json_t *content = json_object_get(conf_str, "content");
            const char* cont = json_string_value(content);
            if(NULL!=cont)
                log_printf(ZLOG_LEVEL_INFO,"\n\n##### cont=%s\n\n",cont);	
            if((0 == strcmp(cmd, "10101"))&&(0!=strcmp(cont,"NULL"))){//���ù����ڴ��е����ݸ��°�����
                log_printf(ZLOG_LEVEL_INFO,"WL is uploaded!!");
                ReadShareMemory();//���°�����
                global_flag = 1;//by niulw add
            }
            else if((0 == strcmp(cmd, "10101"))&&(0==strcmp(cont,"NULL"))){//ɾ�����������������ݶ���Ҫ���
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
            else if(0 == strcmp(cmd, "10201")){//����ȫ�ֱ����������ȡ����ļ���С
                json_t *max_str = json_loads(cont, 0, NULL);
                json_t *max_filesize = json_object_get(max_str, "max_filesize");
                max_cfilesize = json_integer_value(max_filesize);//����ȫ�ֱ������ļ���ȡ���ֵ
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
Description: httpЭ�鴦���̳߳أ����̳߳���Ҫ�Ǹ���������httpЭ����ص�����ʶ�𣬰�����ʱͨ�š��ϴ��ļ���webmail�ʼ����䣬
Calls: sbuff_getdata��webmail_mail
Called By: 
Table Accessed: 
Table Updated: 
Input: �߳���ʱID�ţ���Ҫͨ������CPU��
Output: ��
Return: ��
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

        if(sbuff_getdata(http_buffer_id, sshdr)>0){//�ȴ��źŴ�������������ɹ���ִ�����²���
            HttpSession        sp_session;
            sp_session.http = sshdr->attr;
            sp_session.key.saddr = sshdr->session_five_attr.ip_src;
            sp_session.key.daddr = sshdr->session_five_attr.ip_dst;
            sp_session.key.source = sshdr->session_five_attr.port_src;
            sp_session.key.dest = sshdr->session_five_attr.port_dst;

            char *webmailjson=NULL;
            webmail_mail(&sp_session, &webmailjson, sbuff_putdata, ssbuffer_id);//http���ݴ���������������http��صĴ���
        }

    }
}
/*******************************************/
/* *************************************** */

/***************************
Function: session_consumer_thread()
Description: �����̳߳���Ҫ�Ǹ������Э����Ƚ��������������ϴ��ļ����ݣ���ȡ��Ϣ������tika��������
Calls: sbuff_getdata
Called By: 
Table Accessed: 
Table Updated: 
Input: �߳���ʱID�ţ���Ҫͨ������CPU��
Output: ��
Return: ��
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
        if(sbuff_getdata(sbuffer_id, sshdr)>0){//�źų���ȡ��������������Ϣ
            //log_printf(ZLOG_LEVEL_INFO, "\n\n++++++++++++ session_consumer_thread ====================1111========\n\n");
            SESSION_BUFFER_NODE tikaattr;  
            if(0<session_proc_fun(sshdr, stshdr)){//���ûỰ����Э�����������
                tikaattr = *stshdr;//�������Ϣ��ֵ
                sbuff_putdata(ssbuffer_id, tikaattr);//��Ӵ�������ź���
            }
        }

    }
    return(NULL);
}

/***************************
Function: tika_consumer_thread()
Description: ���̳߳���Ҫ�Ǹ���Э�������ɺ���ļ����ݵ���ȡ�Լ����Ե���ȡ�����̳߳������ж������Ƿ��ǰ��������ݣ�
����ǰ������ڵ����ݱ㲻���д������򣬸���Э��ŵ���tika�����ļ����ݲ�������Ӧ��json�����䵽����ƥ��
���̽��в���ƥ��
Calls: sbuff_getdata��file_extract_pthread��send_json
Called By: 
Table Accessed: 
Table Updated: 
Input: �߳���ʱID�ţ���Ҫͨ������CPU��
Output: ��
Return: ��
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
        if(sbuff_getdata(ssbuffer_id, sshdr)>0){//����ź������к�������
            //char buffer[20] = {};
            char strIP[30] = {0},dstIP[30] = {0};
            int res = 0;
            //getcwd(buffer,20);
            strcpy(sshdr->strpath, session_stream); 
            log_printf(ZLOG_LEVEL_INFO, "\n\n\n#######################################\n\n\n");
            log_printf(ZLOG_LEVEL_INFO, "\n\n\n WL_rules->use_flag =%d,WL_rules_copy->use_flag=%d,WL_rules->up_flag=%d,WL_rules_copy->up_flag=%d\n\n\n");
            log_printf(ZLOG_LEVEL_INFO, "\n\n\n#######################################\n\n\n");
            //�жϰ������Ƿ���Ч
            if((WL_rules->use_flag != 0) || (WL_rules_copy->use_flag != 0)||(WL_rules->up_flag != 0) || (WL_rules_copy->up_flag != 0))
            {   
                strcpy (strIP, int_ntoa (sshdr->session_five_attr.ip_src));
                strcpy (dstIP, int_ntoa (sshdr->session_five_attr.ip_dst));
                log_printf(ZLOG_LEVEL_INFO, "\n\n\n############  strIP = %s,dstIP = %s    #########\n\n\n",strIP,dstIP);
                res = compareIP(strIP, dstIP);//��������Ч���жϴ˻Ựip�Ƿ������
                if(res == 1){//����ǣ��ͷ���Դ���˳���������
                    log_printf(ZLOG_LEVEL_INFO, "\n\n\n############  COMPARE IP OK will be free    #########\n\n\n");
                    if(sshdr->attr != NULL){
                        free(sshdr->attr);
                        sshdr->attr = NULL;
                    }
                    if(sshdr->session_five_attr.protocol==7){//�ͷ�HTTPЭ��������Դ
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
                    else if(sshdr->session_five_attr.protocol==3)//�ͷ�smtp������Դ
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
            if(sshdr->session_five_attr.protocol == 175)//ftp������ļ�����
            {
                int res = 0;
                res = send_json(sshdr, max_cfilesize);//ftp�ļ�������
                if(res == -1)
                    log_printf(ZLOG_LEVEL_ERROR, "file analysis process ERROR: ftp upload file process failed, jason is NULL!!");
            }
            else if((sshdr->session_five_attr.protocol==157)||(sshdr->session_five_attr.protocol==7))//webmail��httpЭ��
            {
                if(sshdr->attr != NULL){
                    int res = 0;//sshdr->strpath
                    res = file_extract_pthread ((char *)sshdr->attr, session_json, max_cfilesize);//ʰȡ�ļ�������
                    if (res == -1)
                    {    
                        log_printf(ZLOG_LEVEL_ERROR, "file analysis process ERROR: webmail process failed, jason is NULL!!");
                    }
                    free(sshdr->attr);    
                }else
                    log_printf(ZLOG_LEVEL_ERROR, "file analysis process ERROR: webmail process failed, parameter transfered is NULL!!");
            }
            else if(sshdr->session_five_attr.protocol==3)//smtp��������
            {
                struct tuple4    addr;
                hash_bucket      *smtpBucket=NULL; 
                SmtpSession_t    *smtp_session=NULL;
                addr.saddr = sshdr->session_five_attr.ip_src;
                addr.daddr = sshdr->session_five_attr.ip_dst;
                addr.source = sshdr->session_five_attr.port_src;
                addr.dest = sshdr->session_five_attr.port_dst;
                smtpBucket = find_hash(ftp_session_tbl, &addr, sizeof(struct tuple4));//�ж��Ƿ���Ч��smtp����
                if(smtpBucket != NULL){
                    smtp_session = (SmtpSession_t	*)smtpBucket->content; 
                    /* ����tika���н��� */
                    if (smtp_session->result_json == NULL)
                    {
                        log_printf(ZLOG_LEVEL_ERROR, "file analysis process ERROR: smtp file process failed, transfered jason is NULL!!");
                    }else{
                        int res = 0;
                        res = file_extract_pthread (smtp_session->result_json, session_json, max_cfilesize);//�ļ�������
                        if (res == -1)
                        {
                            log_printf(ZLOG_LEVEL_ERROR, "file analysis process ERROR: smtp file process failed, the return value is -1!!");
                        }
                    }
                    remove_hash(ftp_session_tbl, smtpBucket, sizeof(struct tuple4));//�ͷ�hash��ڵ�
                    /* �ͷ�Ͱ����Դ */
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
Description: ʱ��ת������gmtʱ��ת��Ϊ����ʱ��
Calls: 
Called By: main
Table Accessed: 
Table Updated: 
Input: time_t t  ʱ��
Output: ��
Return: ��
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
Description: ��Ӧ������Ϣ��������Ӧ������������˼
Calls: 
Called By: main
Table Accessed: 
Table Updated: 
Input: 
Output: ��
Return: ��
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
Output: ��
Return: ��
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
Output: ��
Return: ��
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
Output: ��
Return: ��
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
Description: ϵͳȫ�ֲ������ú�������Ҫ�����ȡϵͳ�����ļ�����ȡ���е�ȫ�ֲ���ֵ����ϵͳ���в�������
Calls: 
Called By: main
Table Accessed: 
Table Updated: 
Input: char *filename//�����ļ��ļ���
Output: ��
Return: ��
Others: 
 ***************************/

int runconf_table_conf(char *filename){
    int num = 0;
    xmlDocPtr doc;
    xmlNodePtr root, node, detail;
    xmlChar *name = NULL, *value = NULL;
    doc = xmlReadFile(filename ,"UTF-8",XML_PARSE_RECOVER);//XML�ļ���ȡ����

    if(NULL == doc)
    {
        log_printf(ZLOG_LEVEL_ERROR, "Open configuration file ERROR: can not open file %s" , filename);
        return -1;
    }

    root = xmlDocGetRootElement(doc);//Ѱ�Ҹ��ڵ�

    if(NULL == root)
    {
        log_printf(ZLOG_LEVEL_ERROR, "Read configuration file ERROR: empty document %s" , filename);
        /*******add code file default configuration*********/
        return -1;
    }

    for(node=root->children;node;node=node->next)
    {
        if(xmlStrcasecmp(node->name,BAD_CAST"init_dev_param")==0)//�жϳ�ʼ���豸����
        {
            device=xmlGetProp(node,BAD_CAST"init_dev");
        }
        if(xmlStrcasecmp(node->name,BAD_CAST"init_bind_port")==0)//�жϳ������а󶨵Ķ˿ں�
        {
            name=xmlGetProp(node,BAD_CAST"bind_port");  
            if(name){
                bind_port=atoi((char* )name);
                free(name);
            }
        }
        if(xmlStrcasecmp(node->name,BAD_CAST"init_tika_param")==0)//�ж�tika�����߳���
        {
            name=xmlGetProp(node,BAD_CAST"tika_threads");  
            if(name){
                tika_pthreads=atoi((char*)name);
                free(name);
            }
        }
        if(xmlStrcasecmp(node->name,BAD_CAST"init_http_param")==0)//�ж�http�����߳���
        {
            name=xmlGetProp(node,BAD_CAST"http_threads");  
            if(name){
                http_pthreads=atoi((char*)name);
                free(name);
            }
        }
        if(xmlStrcasecmp(node->name,BAD_CAST"init_session_param")==0)//�жϻỰ�����߳���
        {
            name=xmlGetProp(node,BAD_CAST"session_threads");  
            if(name){
                session_pthreads=atoi((char*)name);
                free(name);
            }
        }
        if(xmlStrcasecmp(node->name,BAD_CAST"init_max_filesize")==0)//�ж�ϵͳ����=����ļ���С
        {
            name=xmlGetProp(node,BAD_CAST"max_filesize");  
            if(name){
                max_cfilesize=atoi((char*)name);
                free(name);
            }
        }
        /************************************/

        if(xmlStrcasecmp(node->name,BAD_CAST"init_session_stream")==0)//�ж�ϵͳ����=����ļ���С
        {
            value=xmlNodeGetContent(node);
            memcpy(session_stream,(char*)value,PATHLEN);
            xmlFree(value);
        }
        if(xmlStrcasecmp(node->name,BAD_CAST"init_session_json")==0)//�ж�ϵͳ����=����ļ���С
        {
            value=xmlNodeGetContent(node);
            memcpy(session_json,(char*)value,PATHLEN);
            xmlFree(value);
        }


        /***********************************/
        //����ΪЭ����չԤ������ϵͳ��δʹ��
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
    if (zlog_init("conf/session_cap.conf")) {//������־����
        printf("zlog init failed");
        return (-1);
    }
    //������־�ȼ�
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
    char *bpfFilter = "not udp";//���bpf���˹��򣬱�ϵͳ����udpЭ������
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
            case 'F'://��ȡ�����ļ�
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
    //����Ϊpfring��ʼ������
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

    //����Ϊpfring��ʼ������
    g_mime_init (0);//gmime���߳�֧�ֳ�ʼ������
    nids_init();//�Ự�����ʼ������


    /* init kmem interface */
    ikmem_init(NULL);//�ڴ��������ʼ������

    build_JVM();//java�������ʼ������ ��JVM //by niulw


    if(ftp_session_tbl==NULL)//ϵͳʹ��hash����
    {
        if((ftp_session_tbl=create_hash(100000,session_hash, (t_comp_func)session_comp))==0)
        {
            log_printf(ZLOG_LEVEL_FATAL, "system FATAL: create primary table failed!!!");
            return (-1);
            //printf("ftp create_hash fail\n");
        }
    }
    /**********��������ʼ������**************/
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
    /**********��������ʼ������**************/

    nids_register_tcp (tcp_callback);//�Ự����ص�����ע��

    Five_attr = malloc(sizeof(struct tuple5));//��Ԫ��ṹ���ڴ�����
    if(Five_attr==NULL){
        log_printf(ZLOG_LEVEL_FATAL, "system FATAL: Five attr alloca error!!!");
        return (-1);
    }
    memset(Five_attr, 0, sizeof(struct tuple5));

    buffer_id = rbuff_create (600000);//һ�����ݻ��������
    if(buffer_id < 0){
        log_printf(ZLOG_LEVEL_FATAL, "system FATAL: create buffer id failed!!!");
        return (-1);
    }else{
        log_printf(ZLOG_LEVEL_INFO, "system INFO: buffer_id is %d!!!", buffer_id);
    }

    bbuffer_id = rbuff_create (250000);//�������ݻ��������
    if(bbuffer_id < 0){
        log_printf(ZLOG_LEVEL_FATAL, "system FATAL: bbuffer id failed!!!");
        return (-1);
    }else{
        log_printf(ZLOG_LEVEL_INFO, "system INFO: bbuffer_id is %d!!!", bbuffer_id);
    }

    sbuffer_id = sbuff_create (100000);//һ���źų�����
    if(sbuffer_id < 0){
        log_printf(ZLOG_LEVEL_FATAL, "system FATAL: sbuffer id failed!!!");
        return (-1);
    }else{
        log_printf(ZLOG_LEVEL_INFO, "system INFO: sbuffer_id is %d!!!", sbuffer_id);
    }

    ssbuffer_id = sbuff_create (100000);//�����źų�����
    if(ssbuffer_id < 0){
        log_printf(ZLOG_LEVEL_FATAL, "system FATAL: ssbuffer id failed!!!");
        return (-1);
    }else{
        log_printf(ZLOG_LEVEL_INFO, "system INFO: ssbuffer_id is %d!!!", ssbuffer_id);
    }

    http_buffer_id = sbuff_create (100000);//http�����źų�����
    if(http_buffer_id < 0){
        log_printf(ZLOG_LEVEL_FATAL, "system FATAL: http_buffer_creat failed!!!");
        return (-1);
    }else{
        log_printf(ZLOG_LEVEL_INFO, "system INFO: http_buffer_id is %d!!!", http_buffer_id);
    }

    memset(ndpi_thread_info, 0, sizeof(ndpi_thread_info)); 
    setupDetection(0);//Э��������ܳ�ʼ��

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
    //pthread_create(&my_thread[0], NULL, packet_consumer_thread, &i);//���ݰ��洢�̴߳���

    for(i=0; i<1; i++)
        pthread_create(&my_thread[0], NULL, session_process_thread, &i);//���ݰ��Ự�����̴߳���

    for(i=1; i<2; i++)
        pthread_create(&my_thread[1], NULL, WL_upload_thread, &i);//���������¼�ȫ�ֲ������´����̴߳���

    thread_id=0;
    pthread_create(&ndpi_timeout_free, NULL, ndpi_free_thread, (void *) thread_id);//�ͷ�ndpi���ĳ�ʱ�ڵ�

    pthread_t http_session_thread[800];
    pthread_t session_thread[1000];
    pthread_t tika_thread[1000];
    unsigned short thd_id = 0;

    for(thd_id=2; thd_id<session_pthreads; thd_id++)
        pthread_create(&session_thread[thd_id], NULL, session_consumer_thread, &thd_id);//�Ự�����ݽ����̳߳ش���

    for(thd_id=2; thd_id<http_pthreads; thd_id++)
        pthread_create(&http_session_thread[thd_id], NULL, http_file_thread, &thd_id);//httpЭ�����ݽ����̳߳ش���

    for(thd_id=2; thd_id<tika_pthreads; thd_id++)
        pthread_create(&tika_thread[thd_id], NULL, tika_consumer_thread, &thd_id);//tika�ļ����ݼ�������ȡ�����̳߳ش���

    pfring_loop(pd, dummyProcesssPacket, (u_char*)NULL, wait_for_packet);//pfringץ������������


    for(i=0; i<2; i++)
        pthread_join(my_thread[i], NULL);
    for(thread_id = 0; thread_id < 1; thread_id++)
        pthread_join(ndpi_thread_info[thread_id].pthread, NULL);

    pthread_join(ndpi_timeout_free, NULL);

    sleep(1);

    g_mime_shutdown ();//�ر�gmime���̹߳���

    pfring_close(pd);//�ر�pfringץ������

    //destory_JVM();//����java����� //by  niulw��JVM

    return(0);
}
