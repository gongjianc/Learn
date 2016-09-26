/*
  Copyright (c) 1999 Rafal Wojtczuk <nergal@7bulls.com>. All rights reserved.
  See the file COPYING for license details.
*/

#include <config.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <netinet/in_systm.h>
#include <netinet/ip.h>
#include <netinet/tcp.h>
#include <netinet/udp.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <string.h>
#include <syslog.h>
#include <alloca.h>
#include <pcap.h>
#include <errno.h>
#include <config.h>
#include "checksum.h"
#include "ip_fragment.h"
#include "scan.h"
#include "tcp.h"
#include "util.h"
#include "nids.h"
#include "ring_buffer.h"

/*********/
#include <stdio.h>
#include <unistd.h>
#include <stdarg.h>
#include <search.h>
#include <signal.h>

#include "../../config.h"
#include "linux_compat.h"
#include "ndpi_main.h"

#if defined(__OpenBSD__)
#include <sys/socket.h>
#endif
/*********/
#ifdef HAVE_LIBGTHREAD_2_0
#include <glib.h>
#endif
#include <fcntl.h>

#define  linux
#ifdef __linux__
extern int set_all_promisc();
#endif

#define DEFAULT_SNAPLEN      2048
#define DEFAULT_DEVICE     "p6p1"
#define NO_ZC_BUFFER_LEN     9000
#define int_ntoa(x)	inet_ntoa(*((struct in_addr *)&x))

extern int ip_options_compile(unsigned char *);
extern int raw_init();
static void nids_syslog(int, int, struct ip *, void *);
static int nids_ip_filter(struct ip *, int);

static struct proc_node *ip_frag_procs;
static struct proc_node *ip_procs;
static struct proc_node *udp_procs;

struct proc_node *tcp_procs;
static int linktype;
static pcap_t *desc = NULL;

#ifdef HAVE_LIBGTHREAD_2_0

/* async queue for multiprocessing - mcree */
static GAsyncQueue *cap_queue;

/* items in the queue */
struct cap_queue_item {
     void *data;
     struct pcap_pkthdr hdr;
     struct tuple5 Five_attr;
     bpf_u_int32 caplen;
};

/* marks end of queue */
static struct cap_queue_item EOF_item;

/* error buffer for glib calls */
static GError *gerror = NULL;

#endif

char nids_errbuf[PCAP_ERRBUF_SIZE];
struct pcap_pkthdr * nids_last_pcap_header = NULL;
u_char *nids_last_pcap_data = NULL;
u_int nids_linkoffset = 0;
struct tuple5  *Five_attr;

struct ring_buffer_holder   rb_holder[MAX_RING_BUFFER_COUNT]={NULL};
int buffer_id;
/*************测试使用代码***********/
bpf_u_int32 hdr_packets,IPFRAG_packets,IPFRAGL_packets = 0;;

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

struct nids_prm nids_params = {
    2080,			/* n_tcp_streams*4:同一时刻捕获的TCP数据包是此值的3/4 */
    512,			/* n_hosts*4：存储IP碎片信息的hash表大小 */
    "eth0",			/* device */
    NULL,			/* filename */
    336,			/* sk_buff_size*4：数据接口sk_buff的大小，用来排队使用 */
    -1,				/* dev_addon */
    nids_syslog,		/* syslog() */
    LOG_ALERT,			/* syslog_level */
    256,			/* scan_num_hosts */
    3000,			/* scan_delay */
    10,				/* scan_num_ports */
    nids_no_mem,		/* no_mem() */
    nids_ip_filter,		/* ip_filter() */
    "tcp",			/* pcap_filter ：过滤规则存放信息*/
    1,				/* promisc */
    0,				/* one_loop_less */
    500,			/* pcap_timeout */
    1,				/* multiproc */
    200000,			/* queue_limit */
    0,				/* tcp_workarounds */
    NULL			/* pcap_desc */
};

// pcap
static char _pcap_error_buffer[PCAP_ERRBUF_SIZE];
static pcap_t *_pcap_handle = NULL;
static int _pcap_datalink_type = 0;
static u_int8_t enable_protocol_guess = 1, verbose = 0;
static u_int32_t guessed_flow_protocols = 0;
static u_int16_t decode_tunnels = 0;
static u_int16_t num_loops = 1;
static u_int8_t shutdown_app = 0;

// detection
static struct ndpi_detection_module_struct *ndpi_struct = NULL;
static u_int32_t detection_tick_resolution = 1000;
static time_t capture_until = 0;

// results
static u_int64_t raw_packet_count = 0;
static u_int64_t ip_packet_count = 0;
static u_int64_t total_wire_bytes = 0, total_ip_bytes = 0, total_discarded_bytes = 0;
static u_int64_t protocol_counter[NDPI_MAX_SUPPORTED_PROTOCOLS + NDPI_MAX_NUM_CUSTOM_PROTOCOLS + 1];
static u_int64_t protocol_counter_bytes[NDPI_MAX_SUPPORTED_PROTOCOLS + NDPI_MAX_NUM_CUSTOM_PROTOCOLS + 1];
static u_int32_t protocol_flows[NDPI_MAX_SUPPORTED_PROTOCOLS + NDPI_MAX_NUM_CUSTOM_PROTOCOLS + 1] = { 0 };

#define GTP_U_V1_PORT   2152
#define	MAX_NDPI_FLOWS	2000000

#ifndef ETH_P_IP
#define ETH_P_IP 0x0800
#endif

static u_int32_t size_id_struct = 0;

// flow tracking
typedef struct ndpi_flow {
  u_int32_t lower_ip;
  u_int32_t upper_ip;
  u_int16_t lower_port;
  u_int16_t upper_port;
  u_int8_t detection_completed, protocol;
  struct ndpi_flow_struct *ndpi_flow;
  char lower_name[32], upper_name[32];

  u_int32_t packets, bytes;
  // result only, not used for flow identification
  u_int32_t detected_protocol;
  char host_server_name[256];

  void *src_id, *dst_id;
} ndpi_flow_t;

#define NUM_ROOTS        512

static u_int32_t size_flow_struct = 0;
static struct ndpi_flow *ndpi_flows_root[NUM_ROOTS] = { NULL };
static u_int32_t ndpi_flow_count = 0;

static unsigned int node_guess_undetected_protocol(struct ndpi_flow *flow) {
  flow->detected_protocol = ndpi_guess_undetected_protocol(ndpi_struct,
							   flow->protocol,
							   ntohl(flow->lower_ip),
							   ntohs(flow->lower_port),
							   ntohl(flow->upper_ip),
							   ntohs(flow->upper_port));

  if(flow->detected_protocol != 0)
    guessed_flow_protocols++;

  return flow->detected_protocol;
}

#if 1
static char* ipProto2Name(u_short proto_id) {
  static char proto[8];

  switch(proto_id) {
  case IPPROTO_TCP:
    return("TCP");
    break;
  case IPPROTO_UDP:
    return("UDP");
    break;
  case IPPROTO_ICMP:
    return("ICMP");
    break;
  case 112:
    return("VRRP");
    break;
  }

  snprintf(proto, sizeof(proto), "%u", proto_id);
  return(proto);
}

static void printFlow(u_int *num, struct ndpi_flow *flow) {
  if(num != NULL) printf("\t%u", ++(*num));

  printf("\t%s %s:%u <-> %s:%u ",
	 ipProto2Name(flow->protocol),
	 flow->lower_name, ntohs(flow->lower_port),
	 flow->upper_name, ntohs(flow->upper_port));

  printf("[proto: %u/%s][%u pkts/%u bytes][%s]\n",
	 flow->detected_protocol,
	 ndpi_get_proto_name(ndpi_struct, flow->detected_protocol),
	 flow->packets, flow->bytes,
	 flow->host_server_name);
}
#endif

static void node_print_unknown_proto_walker(const void *node, ndpi_VISIT which, int depth, void *user_data) {
  struct ndpi_flow *flow = *(struct ndpi_flow**)node;
  u_int *num = (u_int*)user_data;

  if(flow->detected_protocol != 0 /* UNKNOWN */) return;

  if((which == ndpi_preorder) || (which == ndpi_leaf)) /* Avoid walking the same node multiple times */
    printFlow(num, flow);
}

static void node_print_known_proto_walker(const void *node, ndpi_VISIT which, int depth, void *user_data) {
  struct ndpi_flow *flow = *(struct ndpi_flow**)node;
  u_int *num = (u_int*)user_data;
  
  if(flow->detected_protocol == 0 /* UNKNOWN */) return;

  if((which == ndpi_preorder) || (which == ndpi_leaf)) /* Avoid walking the same node multiple times */
    printFlow(num, flow);
}

static void node_proto_guess_walker(const void *node, ndpi_VISIT which, int depth, void *user_data) {
  struct ndpi_flow *flow = *(struct ndpi_flow**)node;
  u_int *num = (u_int*)user_data;

#if 0
  printf("<%d>Walk on node %s (%p)\n",
	 depth,
	 which == preorder?"preorder":
	 which == postorder?"postorder":
	 which == endorder?"endorder":
	 which == leaf?"leaf": "unknown",
	 flow);
#endif

  if((which == ndpi_preorder) || (which == ndpi_leaf)) { /* Avoid walking the same node multiple times */
    if(enable_protocol_guess) {
      if(flow->detected_protocol == 0 /* UNKNOWN */) {
        node_guess_undetected_protocol(flow);
	// printFlow(num, flow);
      }
    }

    protocol_counter[flow->detected_protocol]       += flow->packets;
    protocol_counter_bytes[flow->detected_protocol] += flow->bytes;
    protocol_flows[flow->detected_protocol]++;
  }
}

char* formatTraffic(float numBits, int bits, char *buf) {
  char unit;

  if(bits)
    unit = 'b';
  else
    unit = 'B';

  if(numBits < 1024) {
    snprintf(buf, 32, "%lu %c", (unsigned long)numBits, unit);
  } else if(numBits < 1048576) {
    snprintf(buf, 32, "%.2f K%c", (float)(numBits)/1024, unit);
  } else {
    float tmpMBits = ((float)numBits)/1048576;

    if(tmpMBits < 1024) {
      snprintf(buf, 32, "%.2f M%c", tmpMBits, unit);
    } else {
      tmpMBits /= 1024;

      if(tmpMBits < 1024) {
	snprintf(buf, 32, "%.2f G%c", tmpMBits, unit);
      } else {
	snprintf(buf, 32, "%.2f T%c", (float)(tmpMBits)/1024, unit);
      }
    }
  }

  return(buf);
}

char* formatPackets(float numPkts, char *buf) {
  if(numPkts < 1000) {
    snprintf(buf, 32, "%.2f", numPkts);
  } else if(numPkts < 1000000) {
    snprintf(buf, 32, "%.2f K", numPkts/1000);
  } else {
    numPkts /= 1000000;
    snprintf(buf, 32, "%.2f M", numPkts);
  }

  return(buf);
}

static void printResults(u_int64_t tot_usec)
{
  u_int32_t i;
  u_int64_t total_flow_bytes = 0;

  printf("\nTraffic statistics:\n");
  printf("\tEthernet bytes:     %-13llu (includes ethernet CRC/IFC/trailer)\n",
	 (long long unsigned int)total_wire_bytes);
  printf("\tDiscarded bytes:    %-13llu\n",
	 (long long unsigned int)total_discarded_bytes);
  printf("\tIP packets:         %-13llu of %llu packets total\n",
	 (long long unsigned int)ip_packet_count,
	 (long long unsigned int)raw_packet_count);
  printf("\tIP bytes:           %-13llu (avg pkt size %u bytes)\n",
	 (long long unsigned int)total_ip_bytes,
	 (unsigned int)(total_ip_bytes/raw_packet_count));
  printf("\tUnique flows:       %-13u\n", ndpi_flow_count);

  if(tot_usec > 0) {
    char buf[32], buf1[32];
 long double    t = (float)(ip_packet_count*1000000)/(float)tot_usec;
    long double  b= (float)(total_wire_bytes * 8 *1000000)/(float)tot_usec;

    printf("\tnDPI throughput:    %s pps / %s/sec\n", formatPackets(t, buf), formatTraffic(b, 1, buf1));
  }

  for(i=0; i<NUM_ROOTS; i++)
    ndpi_twalk(ndpi_flows_root[i], node_proto_guess_walker, NULL);

  if(enable_protocol_guess) {
    printf("\tGuessed flow protocols: %-13u\n", guessed_flow_protocols);
  }

  printf("\n\nDetected protocols:\n");
  for (i = 0; i <= ndpi_get_num_supported_protocols(ndpi_struct); i++) {
    if(protocol_counter[i] > 0) {
//      printf("\t%-20s packets: %-13llu bytes: %-13llu "
//	     "flows: %-13u\n",
//	     ndpi_get_proto_name(ndpi_struct, i), (long long unsigned int)protocol_counter[i],
//	     (long long unsigned int)protocol_counter_bytes[i], protocol_flows[i]);
      printf("\t%-20s ", ndpi_get_proto_name(ndpi_struct, i));
      total_flow_bytes += protocol_counter_bytes[i];
    }
  }

  // printf("\n\nTotal Flow Traffic: %llu (diff: %llu)\n", total_flow_bytes, total_ip_bytes-total_flow_bytes);

  if(verbose) {
    u_int num = 0;
    printf("\n");

    for(i=0; i<NUM_ROOTS; i++)
      ndpi_twalk(ndpi_flows_root[i], node_print_known_proto_walker, &num);

    if(protocol_counter[0] > 0) {
      num = 0;
      printf("\n\nUndetected flows:\n");
      for(i=0; i<NUM_ROOTS; i++)
	ndpi_twalk(ndpi_flows_root[i], node_print_unknown_proto_walker, &num);
    }
  }
}

/*******************/
static void debug_printf(u_int32_t protocol, void *id_struct,
			 ndpi_log_level_t log_level,
			 const char *format, ...) {

 printf("[DEBUG]%s\n",format);

}

static void *malloc_wrapper(unsigned long size)
{
  return malloc(size);
}

static void free_wrapper(void *freeable)
{
  free(freeable);
}

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

static struct ndpi_flow *get_ndpi_flow(const u_int8_t version,
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

  /*
    Note: to keep things simple (pcapReader is just a demo app)
    we handle IPv6 a-la-IPv4.
   */
  if(version == 4) {
    if(ipsize < 20)
      return NULL;

    if((iph->ihl * 4) > ipsize || ipsize < ntohs(iph->tot_len)
       || (iph->frag_off & htons(0x1FFF)) != 0)
      return NULL;
  }

  if(iph->saddr < iph->daddr) {
    lower_ip = iph->saddr;
    upper_ip = iph->daddr;
  } else {
    lower_ip = iph->daddr;
    upper_ip = iph->saddr;
  }

  *proto = iph->protocol;
  l4_offset = iph->ihl * 4;
  if(iph->protocol == 6 && l4_packet_len >= 20) {
    // tcp
    tcph = (struct ndpi_tcphdr *) ((u_int8_t *) iph + l4_offset);
//    printf("lower_port is %d, upper_port is %d!!\n", tcph->source, tcph->dest);
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
  } else if(iph->protocol == 17 && l4_packet_len >= 8) {
    // udp
    udph = (struct ndpi_udphdr *) ((u_int8_t *) iph + l4_offset);
    if(iph->saddr < iph->daddr) {
      lower_port = udph->source;
      upper_port = udph->dest;
    } else {
      lower_port = udph->dest;
      upper_port = udph->source;
    }
  } else {
    // non tcp/udp protocols
    lower_port = 0;
    upper_port = 0;
  }

  flow.protocol = iph->protocol;
  flow.lower_ip = lower_ip, flow.upper_ip = upper_ip;
  flow.lower_port = lower_port, flow.upper_port = upper_port;

  if(0)
    printf("[NDPI] [%u][%u:%u <-> %u:%u]\n",
	   iph->protocol, lower_ip, ntohs(lower_port), upper_ip, ntohs(upper_port));

  idx = (lower_ip + upper_ip + iph->protocol + lower_port + upper_port) % NUM_ROOTS;
  ret = ndpi_tfind(&flow, (void*)&ndpi_flows_root[idx], node_cmp);

  if(ret == NULL) {
    if(ndpi_flow_count == MAX_NDPI_FLOWS) {
      printf("ERROR: maximum flow count (%u) has been exceeded\n", MAX_NDPI_FLOWS);
      exit(-1);
    } else {
      struct ndpi_flow *newflow = (struct ndpi_flow*)malloc(sizeof(struct ndpi_flow));

      if(newflow == NULL) {
	printf("[NDPI] %s(1): not enough memory\n", __FUNCTION__);
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

      if((newflow->ndpi_flow = calloc(1, size_flow_struct)) == NULL) {
	printf("[NDPI] %s(2): not enough memory\n", __FUNCTION__);
	return(NULL);
      }

      if((newflow->src_id = calloc(1, size_id_struct)) == NULL) {
	printf("[NDPI] %s(3): not enough memory\n", __FUNCTION__);
	return(NULL);
      }

      if((newflow->dst_id = calloc(1, size_id_struct)) == NULL) {
	printf("[NDPI] %s(4): not enough memory\n", __FUNCTION__);
	return(NULL);
      }

      ndpi_tsearch(newflow, (void*)&ndpi_flows_root[idx], node_cmp); /* Add */

      ndpi_flow_count += 1;

      if(0) {
	printf("%u\t", ndpi_flow_count);
	printFlow(&ndpi_flow_count, newflow);
      }
      
      *src = newflow->src_id, *dst = newflow->dst_id;
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

static struct ndpi_flow *get_ndpi_flow6(const struct ndpi_ip6_hdr *iph6,
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
}

static void setupDetection(void)
{
  NDPI_PROTOCOL_BITMASK all;

  // init global detection structure
  ndpi_struct = ndpi_init_detection_module(detection_tick_resolution, malloc_wrapper, free_wrapper, debug_printf);
  if(ndpi_struct == NULL) {
    printf("ERROR: global structure initialization failed\n");
    exit(-1);
  }
  // enable all protocols
  NDPI_BITMASK_SET_ALL(all);
  ndpi_set_protocol_detection_bitmask2(ndpi_struct, &all);

  // allocate memory for id and flow tracking
  size_id_struct = ndpi_detection_get_sizeof_ndpi_id_struct();
  size_flow_struct = ndpi_detection_get_sizeof_ndpi_flow_struct();

  // clear memory for results
  memset(protocol_counter, 0, sizeof(protocol_counter));
  memset(protocol_counter_bytes, 0, sizeof(protocol_counter_bytes));
  memset(protocol_flows, 0, sizeof(protocol_flows));

//  if(_protoFilePath != NULL)             //代刚于20140622日修改，不需要输入文件，流式分析
//    ndpi_load_protocols_file(ndpi_struct, _protoFilePath);

  raw_packet_count = ip_packet_count = total_ip_bytes = total_wire_bytes = 0;
  ndpi_flow_count = 0;
}

static void free_ndpi_flow(struct ndpi_flow *flow) {
  if(flow->ndpi_flow) { ndpi_free(flow->ndpi_flow); flow->ndpi_flow = NULL; }
  if(flow->src_id)    { ndpi_free(flow->src_id); flow->src_id = NULL;       }
  if(flow->dst_id)    { ndpi_free(flow->dst_id); flow->dst_id = NULL;       }
}

static void ndpi_flow_freer(void *node) {
  struct ndpi_flow *flow = (struct ndpi_flow*)node;
  free_ndpi_flow(flow);
  ndpi_free(flow);
}


static void terminateDetection(void)
{
  int i;

  for(i=0; i<NUM_ROOTS; i++) {
    ndpi_tdestroy(ndpi_flows_root[i], ndpi_flow_freer);
    ndpi_flows_root[i] = NULL;
  }

  ndpi_exit_detection_module(ndpi_struct, free_wrapper);
}

static unsigned int ndpi_packet_processing(const u_int64_t time,
				      const struct ndpi_iphdr *iph,
				      struct ndpi_ip6_hdr *iph6,
				      u_int16_t ip_offset,
				      u_int16_t ipsize, u_int16_t rawsize, struct tuple5 *Five_attr)
{
  struct ndpi_id_struct *src, *dst;
  struct ndpi_flow *flow;
  struct ndpi_flow_struct *ndpi_flow = NULL;
  struct ndpi_tcphdr *tcph = NULL;
  u_int32_t protocol = 0;
  u_int8_t proto;

  if(iph)
    flow = get_ndpi_flow(4, iph, ip_offset, ipsize,
			 ntohs(iph->tot_len) - (iph->ihl * 4),
			 &src, &dst, &proto, NULL);
  else
    flow = get_ndpi_flow6(iph6, ip_offset, &src, &dst, &proto);

  if(flow != NULL) {
    ip_packet_count++;
    total_wire_bytes += rawsize + 24 /* CRC etc */, total_ip_bytes += rawsize;
    ndpi_flow = flow->ndpi_flow;
    flow->packets++, flow->bytes += rawsize;
  } else {
    return(0);
  }

  if(flow->detection_completed) return(0);

  protocol = (const u_int32_t)ndpi_detection_process_packet(ndpi_struct, ndpi_flow,
							    iph ? (uint8_t *)iph : (uint8_t *)iph6,
							    ipsize, time, src, dst);

  flow->detected_protocol = protocol;


  if((flow->detected_protocol != NDPI_PROTOCOL_UNKNOWN)
//     || ((proto == IPPROTO_UDP) && (flow->packets > 8))
     || ((proto == IPPROTO_TCP) && (flow->packets > 4))) {
    flow->detection_completed = 1;

#if 0
    if(flow->ndpi_flow->l4.tcp.host_server_name[0] != '\0')
      printf("%s\n", flow->ndpi_flow->l4.tcp.host_server_name);
#endif

    snprintf(flow->host_server_name, sizeof(flow->host_server_name), "%s", flow->ndpi_flow->host_server_name);
    free_ndpi_flow(flow);
//    Five_attr->four_attr.source = ntohs(flow->lower_port);
//    Five_attr->four_attr.dest = ntohs(flow->upper_port);
//    Five_attr->four_attr.saddr = flow->lower_ip;
//    Five_attr->four_attr.daddr = flow->upper_ip;
//    Five_attr->protocol = flow->detected_protocol;
//   printf("Five_attr.protocol is %d!!!!!!!!!!!!\n", Five_attr->protocol); 
//   printFlow(NULL, flow);
    if(1) {
      char buf1[32], buf2[32];

      if(enable_protocol_guess) {
        if(flow->detected_protocol == 0 /* UNKNOWN */) {
          protocol = node_guess_undetected_protocol(flow);
        }
      }
     tcph = (struct ndpi_tcphdr *)((u_int8_t *)iph + 4 * iph->ihl);
     Five_attr->four_attr.source = ntohs(tcph->source);
     Five_attr->four_attr.dest = ntohs(tcph->dest);
     Five_attr->four_attr.saddr = iph->saddr;
     Five_attr->four_attr.daddr = iph->daddr;
     Five_attr->protocol = flow->detected_protocol;
//   printf("Five_attr.source is %d!!\n", Five_attr->source);
//   printf("Five_attr.dest is %d!!!!!!!!!!!!\n", Five_attr->dest); 
//   printf("flow->lower_ip is %d!!!!!!!!!!!!\n", flow->lower_ip); 
//   printf("flow->upper_ip is %d!!!!!!!!!!!!\n", flow->upper_ip); 
//   printf("flow->lower_port is %d!!!!!!!!!!!!\n", ntohs(flow->lower_port)); 
//   printf("flow->upper_port is %d!!!!!!!!!!!!\n", ntohs(flow->upper_port)); 
//     printf("Five_attr.protocol is %d!!!!!!!!!!!!\n", Five_attr->protocol); 
//      printFlow(NULL, flow);
    }
  }

  return 0;
}

static void pcap_packet_callback(u_char * args, const struct pcap_pkthdr *header, const u_char * packet, struct tuple5 *Five_attr)
{
  const struct ndpi_ethhdr *ethernet;
  struct ndpi_iphdr *iph;
  struct ndpi_ip6_hdr *iph6;
  u_int64_t time;
  static u_int64_t lasttime = 0;
  u_int16_t type, ip_offset, ip_len;
  u_int16_t frag_off = 0;
  u_int8_t proto = 0;

  raw_packet_count++;

  if((capture_until != 0) && (header->ts.tv_sec >= capture_until)) {
//    if(_pcap_handle != NULL)
//      pcap_breakloop(_pcap_handle);

    return;
  }

  time = ((uint64_t) header->ts.tv_sec) * detection_tick_resolution +
    header->ts.tv_usec / (1000000 / detection_tick_resolution);
  if(lasttime > time) {
    // printf("\nWARNING: timestamp bug in the pcap file (ts delta: %llu, repairing)\n", lasttime - time);
    time = lasttime;
  }
  lasttime = time;

  if(linktype == DLT_NULL) {   
    if(ntohl(*((u_int32_t*)packet)) == 2)
      type = ETH_P_IP;
    else 
      type = 0x86DD; /* IPv6 */

    ip_offset = 4;
  } else if(linktype == DLT_EN10MB) {
    ethernet = (struct ndpi_ethhdr *) packet;
    ip_offset = sizeof(struct ndpi_ethhdr);
    type = ntohs(ethernet->h_proto);
  } else if(linktype == 113 /* Linux Cooked Capture */) {
    type = (packet[14] << 8) + packet[15];
    ip_offset = 16;
  } else
    return;

  while(1) {
    if(type == 0x8100 /* VLAN */) {
      type = (packet[ip_offset+2] << 8) + packet[ip_offset+3];
      ip_offset += 4;
    } else if(type == 0x8847 /* MPLS */) {
      u_int32_t label = ntohl(*((u_int32_t*)&packet[ip_offset]));

      type = 0x800, ip_offset += 4;

      while((label & 0x100) != 0x100) {
	ip_offset += 4;
	label = ntohl(*((u_int32_t*)&packet[ip_offset]));
      }
    } else
      break;
  }

  iph = (struct ndpi_iphdr *) &packet[ip_offset];

  // just work on Ethernet packets that contain IP
  if(type == ETH_P_IP && header->caplen >= ip_offset) {
    frag_off = ntohs(iph->frag_off);

    proto = iph->protocol;
    if(header->caplen < header->len) {
      static u_int8_t cap_warning_used = 0;
      if(cap_warning_used == 0) {
	printf("\n\nWARNING: packet capture size is smaller than packet size, DETECTION MIGHT NOT WORK CORRECTLY\n\n");
	cap_warning_used = 1;
      }
    }
  }

  if(iph->version == 4) {
    ip_len = ((u_short)iph->ihl * 4);
    iph6 = NULL;

    if((frag_off & 0x3FFF) != 0) {
      static u_int8_t ipv4_frags_warning_used = 0;

    v4_frags_warning:
      if(ipv4_frags_warning_used == 0) {
	printf("\n\nWARNING: IPv4 fragments are not handled by this demo (nDPI supports them)\n");
	ipv4_frags_warning_used = 1;
      }
      
      total_discarded_bytes +=  header->len;
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
      printf("\n\nWARNING: only IPv4/IPv6 packets are supported in this demo (nDPI supports both IPv4 and IPv6), all other packets will be discarded\n\n");
      ipv4_warning_used = 1;
    }

    total_discarded_bytes +=  header->len;
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
	  // printf("WARNING: not good (packet_id=%u)!\n", (unsigned int)raw_packet_count);
	  goto v4_warning;
	}
      }
    }
  }
  
  // process the packet
  ndpi_packet_processing(time, iph, iph6, ip_offset, header->len - ip_offset, header->len, Five_attr);
}
/*******************/
/****ring_buffer****/
/**Creation of ring Buffer**/

int rbuff_create (int irb_qlength)
{
    int irb_id = check_for_first_free_ring_buffer(); // Checks for the very first free 
                                                // ring buffer in the holder array
    if (irb_id == -1)
        return -1; // No Free ring buffers

    int ring_buffer_ID=create_ring_buffer(irb_id, irb_qlength); // Create new ring buffer in the holder with ID received
        if(ring_buffer_ID<0)
            return -1; // Failed to create ring buffer
    if((init_lock_for_sync(irb_id, irb_qlength))==-1)
	return -1;       
    return ring_buffer_ID; // Returning the ring buffer ID(index) in success
    
}

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

int  create_ring_buffer(int iringbuffer_ID, int iringbuffer_qlength)
{
    rb_holder[iringbuffer_ID].rb_base = (struct ring_buffer_node_tag*)calloc(
                                        iringbuffer_qlength,
                                        sizeof(struct ring_buffer_node_tag));
    if ( rb_holder[iringbuffer_ID].rb_base == NULL)
        return -1;
    pthread_mutex_init(&rb_holder[iringbuffer_ID].mutex,NULL);
    rb_holder[iringbuffer_ID].irb_qlength          = iringbuffer_qlength; 
    rb_holder[iringbuffer_ID].irb_head_index       = 0; 
    rb_holder[iringbuffer_ID].irb_tail_index       = 0;
    rb_holder[iringbuffer_ID].irb_buffer_end_index = iringbuffer_qlength-1; // need justification

    return iringbuffer_ID;//returning buffer header ID
}

    
init_lock_for_sync(int iringbuffer_ID, int iringbuffer_qlength)//initializing the semaphore lock_object
{
    if(sem_init(&rb_holder[iringbuffer_ID].empty_buffer,0,iringbuffer_qlength) == -1) 
    {
	goto lebel;
    }
	
    if(sem_init(&rb_holder[iringbuffer_ID].full_buffer,0,0) == -1) 
    {   
	 sem_destroy(&rb_holder[iringbuffer_ID].empty_buffer);
  	 goto lebel;
    }
    return 0; 
    lebel : free(rb_holder[iringbuffer_ID].rb_base);//freeing alocated memory on initialisation failure...
    return -1;
}



/**Putting data on ring buffer**/

int rbuff_putdata(int iringbuffer_ID, void *pData, struct pcap_pkthdr *hdr, struct tuple5 *five_attr)
{
//	printf("++ rbuff_putdata \n");
    if(sem_wait(&rb_holder[iringbuffer_ID].empty_buffer)==-1)
    {
       	printf("sem_wait is failed to lock empty_buffer");
       	return -1;
    }
    rb_holder[iringbuffer_ID].rb_base[rb_holder[iringbuffer_ID].irb_tail_index].pdata_buffer = (char *)malloc(hdr->len);    // Allocating memory for the ode
                
    if(rb_holder[iringbuffer_ID].rb_base[rb_holder[iringbuffer_ID].irb_tail_index].pdata_buffer == NULL) // Checks Memory  Allocated
    return -1;

//    if(memcpy(&rb_holder[iringbuffer_ID].rb_base[rb_holder[iringbuffer_ID].irb_tail_index].pkthdr,hdr,sizeof(hdr))!=NULL)
    rb_holder[iringbuffer_ID].rb_base[rb_holder[iringbuffer_ID].irb_tail_index].pkthdr.len = hdr->len;
    rb_holder[iringbuffer_ID].rb_base[rb_holder[iringbuffer_ID].irb_tail_index].pkthdr.caplen = hdr->caplen;
    rb_holder[iringbuffer_ID].rb_base[rb_holder[iringbuffer_ID].irb_tail_index].pkthdr.ts.tv_sec = hdr->ts.tv_sec;
    rb_holder[iringbuffer_ID].rb_base[rb_holder[iringbuffer_ID].irb_tail_index].pkthdr.ts.tv_sec = hdr->ts.tv_usec;
    rb_holder[iringbuffer_ID].rb_base[rb_holder[iringbuffer_ID].irb_tail_index].five_attr.four_attr.source = five_attr->four_attr.source;
    rb_holder[iringbuffer_ID].rb_base[rb_holder[iringbuffer_ID].irb_tail_index].five_attr.four_attr.dest = five_attr->four_attr.dest;
    rb_holder[iringbuffer_ID].rb_base[rb_holder[iringbuffer_ID].irb_tail_index].five_attr.four_attr.saddr = five_attr->four_attr.saddr;
    rb_holder[iringbuffer_ID].rb_base[rb_holder[iringbuffer_ID].irb_tail_index].five_attr.four_attr.daddr = five_attr->four_attr.daddr;
    rb_holder[iringbuffer_ID].rb_base[rb_holder[iringbuffer_ID].irb_tail_index].five_attr.protocol = five_attr->protocol;
//       printf("h->caplen is %d!!\n", rb_holder[iringbuffer_ID].rb_base[rb_holder[iringbuffer_ID].irb_tail_index].pkthdr.caplen);
    if(memcpy(rb_holder[iringbuffer_ID].rb_base[rb_holder[iringbuffer_ID].irb_tail_index].pdata_buffer,pData,hdr->len)==NULL)
    {   
        free(rb_holder[iringbuffer_ID].rb_base[rb_holder[iringbuffer_ID].irb_tail_index].pdata_buffer);
        rb_holder[iringbuffer_ID].rb_base=NULL;
        return -1;
    }
    // Incrementing Tail pointer and assumes never co-inside with head pointer.

    if(rb_holder[iringbuffer_ID].irb_tail_index >= rb_holder[iringbuffer_ID].irb_qlength -1 )
    {
        rb_holder[iringbuffer_ID].irb_tail_index = 0;
    }
    else
    {
        rb_holder[iringbuffer_ID].irb_tail_index++;
    }
    if(sem_post(&rb_holder[iringbuffer_ID].full_buffer)==-1)
    {
        printf("sem_post is failed to unlock in empty_full");
        return -1;
    }
    //release the semaphore count on successfull write.
//	printf("-- rbuff_putdata \n");
    return 1;
    
}

/**Getting data from ring buffer**/


int rbuff_getdata(int iringbuffer_ID, void *pData, struct pcap_pkthdr *hdr, struct tuple5 *five_attr)
{
    if(sem_wait(&rb_holder[iringbuffer_ID].full_buffer)==-1)
    {
        	printf("sem_wait is failed to lock full_buffer");
        	return -1;
    }

    if((rb_holder[iringbuffer_ID].rb_base)==NULL) // Checks for Valid Ring Buffer
        return -1;

    int temp_index;    	
    pthread_mutex_lock(&rb_holder[iringbuffer_ID].mutex);
    temp_index = rb_holder[iringbuffer_ID].irb_head_index;
    //updating head pointer
//    printf("h->caplen is %d!!\n", rb_holder[iringbuffer_ID].irb_head_index);
    if(rb_holder[iringbuffer_ID].irb_head_index >= rb_holder[iringbuffer_ID].irb_qlength -1)

    {
        rb_holder[iringbuffer_ID].irb_head_index=0;
    }
    else        
    {
        rb_holder[iringbuffer_ID].irb_head_index++;
    }
//  sem_post(&rb_holder[iringbuffer_ID].sem_obj); //lock on empty buffer condition

    if(sem_post(&rb_holder[iringbuffer_ID].empty_buffer)==-1)
     {
       	printf("sem_post is failed to unlock");
       	return -1;
     }
    	
    pthread_mutex_unlock(&rb_holder[iringbuffer_ID].mutex);
    
//    if(idatasize < 
//        rb_holder[iringbuffer_ID].rb_base[rb_holder[iringbuffer_ID].irb_head_index].irb_node_data_size)
//        return -1; // Return with Fail if data buffer is small compared to actual data

//    printf("hdr->caplen is %d!!\n", rb_holder[iringbuffer_ID].rb_base[temp_index].pkthdr.len);
        // copying data
    if(NULL == memcpy(pData,rb_holder[iringbuffer_ID].rb_base[temp_index].pdata_buffer,
                rb_holder[iringbuffer_ID].rb_base[temp_index].pkthdr.len))
        return -1;
    
    hdr->len = rb_holder[iringbuffer_ID].rb_base[temp_index].pkthdr.len;
    hdr->caplen = rb_holder[iringbuffer_ID].rb_base[temp_index].pkthdr.caplen;
    hdr->ts.tv_sec = rb_holder[iringbuffer_ID].rb_base[temp_index].pkthdr.ts.tv_sec;
    hdr->ts.tv_usec = rb_holder[iringbuffer_ID].rb_base[temp_index].pkthdr.ts.tv_usec;
    five_attr->four_attr.source = rb_holder[iringbuffer_ID].rb_base[temp_index].five_attr.four_attr.source;
    five_attr->four_attr.dest = rb_holder[iringbuffer_ID].rb_base[temp_index].five_attr.four_attr.dest;
    five_attr->four_attr.saddr = rb_holder[iringbuffer_ID].rb_base[temp_index].five_attr.four_attr.saddr;
    five_attr->four_attr.daddr = rb_holder[iringbuffer_ID].rb_base[temp_index].five_attr.four_attr.daddr;
    five_attr->protocol = rb_holder[iringbuffer_ID].rb_base[temp_index].five_attr.protocol;
//    printf("hdr.caplen is %d!!\n", hdr->len);
    // freeing buffer
//    int temp=rb_holder[iringbuffer_ID].irb_head_index;
    free(rb_holder[iringbuffer_ID].rb_base[temp_index].pdata_buffer);
    rb_holder[iringbuffer_ID].rb_base[temp_index].pdata_buffer = NULL;
    rb_holder[iringbuffer_ID].rb_base[temp_index].pkthdr.len = 0;
    rb_holder[iringbuffer_ID].rb_base[temp_index].pkthdr.caplen = 0;
    rb_holder[iringbuffer_ID].rb_base[temp_index].pkthdr.ts.tv_sec = 0;
    rb_holder[iringbuffer_ID].rb_base[temp_index].pkthdr.ts.tv_usec = 0;
    
    return 1; // return size  in terms of byte successfully Written to buffer
}

/**Destroying Ring Buffer**/

int rbuff_destroy(int iringbuffer_ID)
{
    int i;
    
    if((rb_holder[iringbuffer_ID].rb_base)==NULL) // Checks for Valid Ring Buffer
        return -1;
        
    for(i=0; i <rb_holder[iringbuffer_ID].irb_qlength ; i++)
    {
        if(rb_holder[iringbuffer_ID].rb_base[i].pdata_buffer != NULL)
        {
            free(rb_holder[iringbuffer_ID].rb_base[i].pdata_buffer);
            
//            rb_holder[iringbuffer_ID].rb_base[i].pkthdr = 0;
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

static int nids_ip_filter(struct ip *x, int len)
{
    (void)x;
    (void)len;
    return 1;
}

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
		   "%s,from %s:%hu to  %s:%hu\n", nids_warnings[errnum],
		   saddr, ntohs(((struct tcphdr *) data)->th_sport), daddr,
		   ntohs(((struct tcphdr *) data)->th_dport));
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

/* called either directly from pcap_hand() or from cap_queue_process_thread()
 * depending on the value of nids_params.multiproc - mcree
 */
static void call_ip_frag_procs(void *data, bpf_u_int32 caplen, struct pcap_pkthdr *hdr, struct tuple5 *Five_attr)
{
    struct proc_node *i;
    for (i = ip_frag_procs; i; i = i->next)
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
FILE * outfile;
int fd;

void nids_pcap_handler(u_char *par, struct pcap_pkthdr *hdr, u_char *data, struct tuple5 *Five_attr)
{
    u_char *data_aligned;
    hdr_packets++;
#ifdef HAVE_LIBGTHREAD_2_0
    struct cap_queue_item *qitem;
#endif
#ifdef DLT_IEEE802_11
    unsigned short fc;
    int linkoffset_tweaked_by_prism_code = 0;
    int linkoffset_tweaked_by_radio_code = 0;
#endif
//printf("hdr->len is %d!!\n",hdr->len); 
//printf("hdr->caplen is %d!!\n",hdr->caplen); 
//fd = open("test.txt", O_RDWR|O_CREAT|O_APPEND);
//write(fd,data,hdr->len); // we print the newly arrived data
//outfile = fopen("temp.txt", "a+" );
//fwrite( data, sizeof(unsigned char), hdr->len, outfile);
//fclose(outfile);
//close(fd);
//write(2,data,hdr->len);
//    printf("nids_pcap_handler packets is %d!!\n",hdr->len); 
    /*
     * Check for savagely closed TCP connections. Might
     * happen only when nids_params.tcp_workarounds is non-zero;
     * otherwise nids_tcp_timeouts is always NULL.
     */
    if (NULL != nids_tcp_timeouts)
      tcp_check_timeouts(&hdr->ts);

    nids_last_pcap_header = hdr;
    nids_last_pcap_data = data;
    (void)par; /* warnings... */
    switch (linktype) {
    case DLT_EN10MB:
//	printf("linktype DLT_EN10MB is selected!!\n");
	if (hdr->caplen < 14)
	    return;
	/* Only handle IP packets and 802.1Q VLAN tagged packets below. */
	if (data[12] == 8 && data[13] == 0) {
	    /* Regular ethernet */
//            printf("This is Regular ethernet!!\n");
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
//	printf("linktype DLT_PRISM_HEADER is selected!!\n");
	nids_linkoffset = 144; //sizeof(prism2_hdr);
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
    if (hdr->caplen < nids_linkoffset){
    printf("!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!\n"); 
	return;}
//    printf("hdr->caplen is %d!!\n",hdr->caplen); 

/*
* sure, memcpy costs. But many EXTRACT_{SHORT, LONG} macros cost, too. 
* Anyway, libpcap tries to ensure proper layer 3 alignment (look for
* handle->offset in pcap sources), so memcpy should not be called.
*/
#ifdef LBL_ALIGN
    printf("LBL_ALIGN is defined!!\n");
    if ((unsigned long) (data + nids_linkoffset) & 0x3) {
	data_aligned = alloca(hdr->caplen - nids_linkoffset + 4);
	data_aligned -= (unsigned long) data_aligned % 4;
	memcpy(data_aligned, data + nids_linkoffset, hdr->caplen - nids_linkoffset);
    } else 
#endif
  data_aligned = data + nids_linkoffset;
  
#if 1
 #ifdef HAVE_LIBGTHREAD_2_0
     if(nids_params.multiproc) { 
        /* 
         * Insert received fragment into the async capture queue.
         * We hope that the overhead of memcpy 
         * will be saturated by the benefits of SMP - mcree
         */
//        printf("nids_params.multiproc have num!!\n");
        qitem=malloc(sizeof(struct cap_queue_item));
        if (qitem && (qitem->data=malloc(hdr->caplen - nids_linkoffset))) {
          qitem->caplen=hdr->caplen - nids_linkoffset;
          memcpy(qitem->data,data_aligned,qitem->caplen);
          qitem->Five_attr.four_attr.source = Five_attr->four_attr.source;
          qitem->Five_attr.four_attr.dest = Five_attr->four_attr.dest;
          qitem->Five_attr.four_attr.saddr = Five_attr->four_attr.saddr;
          qitem->Five_attr.four_attr.daddr = Five_attr->four_attr.daddr;
          qitem->Five_attr.protocol = Five_attr->protocol;
          qitem->hdr.len = hdr->len;
          qitem->hdr.caplen = hdr->caplen;
          qitem->hdr.ts.tv_sec = hdr->ts.tv_sec;
          qitem->hdr.ts.tv_usec = hdr->ts.tv_usec;
//          memcpy(qitem->Five_attr, Five_attr, sizeof(struct tuple5));
          g_async_queue_lock(cap_queue);
          /* ensure queue does not overflow */
//          printf("nids_params.queue_limit is %d!!\n",nids_params.queue_limit);
          if(g_async_queue_length_unlocked(cap_queue) > nids_params.queue_limit) {
	    /* queue limit reached: drop packet - should we notify user via syslog? */
	    free(qitem->data);
	    free(qitem);
            printf("nids_params.queue_limit is %d!!\n",1);
	    } else {
	    /* insert packet to queue */
	    g_async_queue_push_unlocked(cap_queue,qitem);
          }
//        printf("nids_params.queue_limit is %d!!\n",3);
          g_async_queue_unlock(cap_queue);
	}
     } else { /* user requested simple passthru - no threading */
//        printf("HAVE_LIBGTHREAD_2_0 is defined!!\n");
//        printf("hdr->caplen is %d!!\n",hdr->caplen);
//        printf("nids_linkoffset is %d!!\n",nids_linkoffset);
        call_ip_frag_procs(data_aligned,hdr->caplen - nids_linkoffset, hdr, Five_attr);
     }
 #else
//     printf("HAVE_LIBGTHREAD_2_0 is not defined!!\n");
     call_ip_frag_procs(data_aligned,hdr->caplen - nids_linkoffset, hdr, Five_attr);
 #endif
#endif
}

int pkts_num = 0;
void process_cap_packets(u_char * par, struct pcap_pkthdr *hdr, u_char * data)
{
   struct pcap_pkthdr pkthdr;
   char ret;

//   pcap_packet_callback(NULL, hdr, data, Five_attr);
//   printf("Five_attr.source111 is %d!!\n", Five_attr->source);
//   printf("Five_attr.dest111 is %d!!!!!!!!!!!!\n", Five_attr->dest); 
//   printf("Five_attr.saddr111 is %d!!!!!!!!!!!!\n", Five_attr->saddr); 
 //  printf("hdr->len is %d!!!!!!!!!!!!\n", hdr->len); 
  // printf("hdr->caplen is %d!!!!!!!!!!!!\n", hdr->caplen); 
   //printResults(0);
   pkthdr.len = hdr->len;
   pkthdr.caplen = hdr->caplen;
   pkthdr.ts.tv_sec = hdr->ts.tv_sec;
   pkthdr.ts.tv_usec = hdr->ts.tv_usec;
   ret = rbuff_putdata(buffer_id, data, &pkthdr, Five_attr);
   if(ret > 0){;
//      Five_attr->four_attr.source = 0;
//      Five_attr->four_attr.dest = 0;
//      Five_attr->four_attr.saddr = 0;
//      Five_attr->four_attr.daddr = 0;
//      Five_attr->protocol = 0;
//     ;}else
   }

}

void* packet_consumer_thread(void* _id) 
{

  long thread_id = (long)_id;
  u_char buffer[NO_ZC_BUFFER_LEN];
  u_char *buffer_p = buffer;

  /* printf("packet_consumer_thread(%lu)\n", thread_id); */
  struct pcap_pkthdr *hdr;
  struct tuple5 *nfive_attr;
  hdr = malloc(sizeof(struct pcap_pkthdr));
  nfive_attr = malloc(sizeof(struct tuple5));
  while(1){

    if(rbuff_getdata(buffer_id, buffer, hdr, nfive_attr)>0){
        pcap_packet_callback(NULL, hdr, buffer, Five_attr);
     // printf("Five_attr.sourcennn is %d!!\n", nfive_attr->source);
      //printf("Five_attr.destnnn is %d!!!!!!!!!!!!\n", nfive_attr->dest); 
//      printf("Five_attr.saddrnnn is %d!!!!!!!!!!!!\n", nfive_attr->saddr); 
//      printf("Five_attr.daddrnnn is %d!!!!!!!!!!!!\n", nfive_attr->daddr); 
      printf("Five_attr.protocolnnn is %d!!!!!!!!!!!!\n", nfive_attr->protocol); 
//      printf("There is new data,caplennnn is %d!!\n", hdr->len);
//      write(1, buffer, hdr.caplen);
//     pkts_num++;
//     printf("pkts_num is %d!!\n", pkts_num);
       nids_pcap_handler(NULL, hdr, buffer, Five_attr);
    }
  }
  return(NULL);
}

static void gen_ip_frag_proc(u_char * data, int len, struct pcap_pkthdr *hdr, struct tuple5 *Five_attr)
{
    struct proc_node *i;
    struct ip *iph = (struct ip *) data;
    int need_free = 0;
    int skblen;
    void (*glibc_syslog_h_workaround)(int, int, struct ip *, void*)=
        nids_params.syslog;
    if (!nids_params.ip_filter(iph, len)){
	return;}
//    IPFRAG_packets++;
//    printf("nids_params.sk_buff numbers-1 is %d!!\n",IPFRAG_packets);  

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

//    IPFRAGL_packets++;

//    printf("nids_params.sk_buff numbers-6 is %d!!\n",IPFRAGL_packets);  

    switch (ip_defrag_stub((struct ip *) data, &iph)) {
    case IPF_ISF:
	return;
    case IPF_NOTF:
	need_free = 0;
//        printf("There is no ip_frag packet in!!!!!!!!!!!!\n"); 
	iph = (struct ip *) data;
	break;
    case IPF_NEW:
	need_free = 1;
//        printf("There is a new ip_frag packet in!!!!!!!!!!!!\n");   
	break;
    default:;
    }
    skblen = ntohs(iph->ip_len) + 16;
    if (!need_free)
	skblen += nids_params.dev_addon;
//    printf("skblen before is %d!!\n",skblen);   
    skblen = (skblen + 15) & ~15;
    skblen += nids_params.sk_buff_size;
//    printf("skblen after is %d!!\n",skblen);   
//    write(2,iph,skblen); 
    for (i = ip_procs; i; i = i->next)
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

static void process_udp(char *data)
{
    struct proc_node *ipp = udp_procs;
    struct ip *iph = (struct ip *) data;
    struct udphdr *udph;
    struct tuple4 addr;
    int hlen = iph->ip_hl << 2;
    int len = ntohs(iph->ip_len);
    int ulen;
    if (len - hlen < (int)sizeof(struct udphdr))
	return;
    udph = (struct udphdr *) (data + hlen);
    ulen = ntohs(udph->UH_ULEN);
    if (len - hlen < ulen || ulen < (int)sizeof(struct udphdr))
	return;
    /* According to RFC768 a checksum of 0 is not an error (Sebastien Raveau) */
    if (udph->uh_sum && my_udp_check
	((void *) udph, ulen, iph->ip_src.s_addr,
	 iph->ip_dst.s_addr)) return;
    addr.source = ntohs(udph->UH_SPORT);
    addr.dest = ntohs(udph->UH_DPORT);
    addr.saddr = iph->ip_src.s_addr;
    addr.daddr = iph->ip_dst.s_addr;
    while (ipp) {
	ipp->item(&addr, ((char *) udph) + sizeof(struct udphdr),
		  ulen - sizeof(struct udphdr), data);
	ipp = ipp->next;
    }
}
static void gen_ip_proc(u_char * data, int skblen, struct pcap_pkthdr *hdr, struct tuple5 *Five_attr)
{
    switch (((struct ip *) data)->ip_p) {
    case IPPROTO_TCP:
	process_tcp(data, skblen, hdr, Five_attr);
	break;
#if 0
    case IPPROTO_UDP:
          printf("nids_params.queue_limit is %d!!\n",5);
	process_udp((char *)data);
	break;
    case IPPROTO_ICMP:
          printf("nids_params.queue_limit is %d!!\n",6);
	if (nids_params.n_tcp_streams)
	    process_icmp(data);
	break;
#endif
    default:
	break;
    }
}
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

void nids_register_udp(void (*x))
{
    register_callback(&udp_procs, x);
}

void nids_unregister_udp(void (*x))
{
    unregister_callback(&udp_procs, x);
}

void nids_register_ip(void (*x))
{
    register_callback(&ip_procs, x);
}

void nids_unregister_ip(void (*x))
{
    unregister_callback(&ip_procs, x);
}

void nids_register_ip_frag(void (*x))
{
    register_callback(&ip_frag_procs, x);
}

void nids_unregister_ip_frag(void (*x))
{
    unregister_callback(&ip_frag_procs, x);
}

static int open_live()
{
    char *device;
    int promisc = 1;

    if (nids_params.device == NULL)
	nids_params.device = pcap_lookupdev(nids_errbuf);
    if (nids_params.device == NULL)
	return 0;

    device = nids_params.device;
    if (!strcmp(device, "all"))
	device = "any";
    else
	promisc = (nids_params.promisc != 0);
    printf("promisc is %d!!\n", promisc);
    if ((desc = pcap_open_live(DEFAULT_DEVICE, 16384, promisc,
			       nids_params.pcap_timeout, nids_errbuf)) == NULL)
	return 0;
#ifdef __linux__
//    printf("XXXXXXXXXX\n");
    if (!strcmp(device, "any") && nids_params.promisc
	&& !set_all_promisc()) {
	nids_errbuf[0] = 0;
	strncat(nids_errbuf, strerror(errno), sizeof(nids_errbuf) - 1);
	return 0;
    }
#endif
    if (!raw_init()) {
	nids_errbuf[0] = 0;
	strncat(nids_errbuf, strerror(errno), sizeof(nids_errbuf) - 1);
	return 0;
    }
    return 1;
}

#ifdef HAVE_LIBGTHREAD_2_0
char i;
#define START_CAP_QUEUE_PROCESS_THREAD() \
    if(nids_params.multiproc) { /* threading... */ \
	    if(!(g_thread_create_full((GThreadFunc)cap_queue_process_thread,NULL,0,FALSE,TRUE,G_THREAD_PRIORITY_NORMAL,&gerror))) { \
	      strcpy(nids_errbuf, "thread: "); \
	      strncat(nids_errbuf, gerror->message, sizeof(nids_errbuf) - 8); \
	      return 0; \
	    }; \
    }

#define STOP_CAP_QUEUE_PROCESS_THREAD() \
    if(nids_params.multiproc) { /* stop the capture process thread */ \
	 g_async_queue_push(cap_queue,&EOF_item); \
    }


/* thread entry point 
 * pops capture queue items and feeds them to
 * the ip fragment processors - mcree
 */
static void cap_queue_process_thread()
{
     struct cap_queue_item *qitem;
     
     while(1) { /* loop "forever" */
	  qitem=g_async_queue_pop(cap_queue);
	  if (qitem==&EOF_item) break; /* EOF item received: we should exit */
//          printf("nids_params.queue_limit is %d!!\n",4);
	  call_ip_frag_procs(qitem->data, qitem->caplen, &qitem->hdr, &qitem->Five_attr);
	  free(qitem->data);
	  free(qitem);
     }
     g_thread_exit(NULL);
}

#else

#define START_CAP_QUEUE_PROCESS_THREAD()
#define STOP_CAP_QUEUE_PROCESS_THREAD()

#endif

int nids_init()
{
    hdr_packets = 0;
    IPFRAG_packets = 0;
    IPFRAGL_packets = 0;
    /* free resources that previous usages might have allocated */
    nids_exit();
#if 0
    u_int32_t flags = 0;
    char path[256] = { 0 };
    flags |= PF_RING_DNA_SYMMETRIC_RSS;  /* Note that symmetric RSS is ignored by non-DNA drivers */
    pd = pfring_open(DEFAULT_DEVICE, DEFAULT_SNAPLEN, flags);  //20140423,添加pfring打开函数
    if(pd == NULL) {
	    printf("Pfring open is failed!!\n");
            fprintf(stderr, "pfring_open error [%s] (pf_ring not loaded or perhaps you use quick mode and have already a socket bound to %s ?)\n",
	    strerror(errno), DEFAULT_DEVICE);
            return(-1);
    } else {
    	u_int32_t version;

    	pfring_set_application_name(pd, "pfcount");
    	pfring_version(pd, &version);

    	printf("Using PF_RING v.%d.%d.%d\n",
	   	(version & 0xFFFF0000) >> 16,
	   	(version & 0x0000FF00) >> 8,
	   	version & 0x000000FF);
    }
    pfring_set_application_stats(pd, "Statistics not yet computed: please try again...");
    if(pfring_get_appl_stats_file_name(pd, path, sizeof(path)) != NULL)
      fprintf(stderr, "Dumping statistics on %s\n", path);

    if (pfring_enable_ring(pd) != 0) {
      printf("Unable to enable ring :-(\n");
      pfring_close(pd);
      return(-1);
    }
#endif
    Five_attr = malloc(sizeof(struct tuple5));
#if 1
    if (nids_params.pcap_desc)
        desc = nids_params.pcap_desc;
    else if (nids_params.filename) {
	if ((desc = pcap_open_offline(nids_params.filename,
				      nids_errbuf)) == NULL)
	    return 0;
    } else if (!open_live())
	return 0;
#endif
    if (nids_params.pcap_filter != NULL) {
	u_int mask = 0;
	struct bpf_program fcode;

	if (pcap_compile(desc, &fcode, nids_params.pcap_filter, 1, 0xFFFFFF00) <
	    0) return 0;
	if (pcap_setfilter(desc, &fcode) == -1)
	    return 0;
    }
    switch ((linktype = pcap_datalink(desc))) {
#ifdef DLT_IEEE802_11
#ifdef DLT_PRISM_HEADER
    case DLT_PRISM_HEADER:
#endif
#ifdef DLT_IEEE802_11_RADIO
    case DLT_IEEE802_11_RADIO:
#endif
    case DLT_IEEE802_11:
	/* wireless, need to calculate offset per frame */
	break;
#endif
#ifdef DLT_NULL
    case DLT_NULL:
        nids_linkoffset = 4;
        break;
#endif        
    case DLT_EN10MB:
		printf("Enter into DLT_EN10MB!!!!!!!!!!!!\n"); 
	nids_linkoffset = 14;
	break;
    case DLT_PPP:
	nids_linkoffset = 4;
	break;
	/* Token Ring Support by vacuum@technotronic.com, thanks dugsong! */
    case DLT_IEEE802:
	nids_linkoffset = 22;
	break;

    case DLT_RAW:
    case DLT_SLIP:
	nids_linkoffset = 0;
	break;
#define DLT_LINUX_SLL   113
    case DLT_LINUX_SLL:
	nids_linkoffset = 16;
	break;
#ifdef DLT_FDDI
    case DLT_FDDI:
        nids_linkoffset = 21;
        break;
#endif        
#ifdef DLT_PPP_SERIAL 
    case DLT_PPP_SERIAL:
        nids_linkoffset = 4;
        break;
#endif        
    default:
	strcpy(nids_errbuf, "link type unknown");
	return 0;
    }
    if (nids_params.dev_addon == -1) {
	if (linktype == DLT_EN10MB)
	    nids_params.dev_addon = 16;
	else
	    nids_params.dev_addon = 0;
    }
    if (nids_params.syslog == nids_syslog)
	openlog("libnids", 0, LOG_LOCAL0);

    init_procs();
    tcp_init(nids_params.n_tcp_streams);
    ip_frag_init(nids_params.n_hosts);
    scan_init();

    if(nids_params.multiproc) {
#ifdef HAVE_LIBGTHREAD_2_0
	 g_thread_init(NULL);
	 cap_queue=g_async_queue_new();
#else
	 strcpy(nids_errbuf, "libnids was compiled without threads support");
	 return 0;        
#endif
    }

    buffer_id = rbuff_create (200000);
    if(buffer_id < 0)
      fprintf(stderr, "ring_buffer_creat failed\n");
    setupDetection();

    return 1;
 }

int nids_run()
{
    if (!desc) {
	strcpy(nids_errbuf, "Libnids not initialized");
	return 0;
    }
    pthread_t my_thread;
    long i;

    for(i=0; i<1; i++)
      pthread_create(&my_thread, NULL, packet_consumer_thread, (void*)i);

//    for(i=0; i<num_threads; i++)
//      pthread_join(my_thread, NULL);

    START_CAP_QUEUE_PROCESS_THREAD(); /* threading... */
    pcap_loop(desc, -1, (pcap_handler)process_cap_packets, NULL);
//    pfring_loop(pd, nids_pcap_handler, (u_char*)NULL, 1);
    /* FIXME: will this code ever be called? Don't think so - mcree */
    STOP_CAP_QUEUE_PROCESS_THREAD(); 
    nids_exit();
    return 0;
}

void nids_exit()
{
    if (!desc) {
        strcpy(nids_errbuf, "Libnids not initialized");
	return;
    }
#ifdef HAVE_LIBGTHREAD_2_0
    if (nids_params.multiproc) {
    /* I have no portable sys_sched_yield,
       and I don't want to add more synchronization...
    */
      while (g_async_queue_length(cap_queue)>0) 
        usleep(1000);
    }
#endif
    tcp_exit();
    ip_frag_exit();
    scan_exit();
    strcpy(nids_errbuf, "loop: ");
    strncat(nids_errbuf, pcap_geterr(desc), sizeof nids_errbuf - 7);
    if (!nids_params.pcap_desc)
        pcap_close(desc);
    desc = NULL;

    free(ip_procs);
    free(ip_frag_procs);
}

int nids_getfd()
{
    if (!desc) {
	strcpy(nids_errbuf, "Libnids not initialized");
	return -1;
    }
    return pcap_get_selectable_fd(desc);
}

int nids_next()
{
    struct pcap_pkthdr h;
    char *data;

    if (!desc) {
	strcpy(nids_errbuf, "Libnids not initialized");
	return 0;
    }
    if (!(data = (char *) pcap_next(desc, &h))) {
	strcpy(nids_errbuf, "next: ");
	strncat(nids_errbuf, pcap_geterr(desc), sizeof(nids_errbuf) - 7);
	return 0;
    }
    /* threading is quite useless (harmful) in this case - should we do an API change?  */
    START_CAP_QUEUE_PROCESS_THREAD();
    nids_pcap_handler(0, &h, (u_char *)data, Five_attr);
    STOP_CAP_QUEUE_PROCESS_THREAD();
    return 1;
}

int nids_dispatch(int cnt)
{
    int r;

    if (!desc) {
	strcpy(nids_errbuf, "Libnids not initialized");
	return -1;
    }
    START_CAP_QUEUE_PROCESS_THREAD(); /* threading... */
    if ((r = pcap_dispatch(desc, cnt, (pcap_handler) nids_pcap_handler,
                                    NULL)) == -1) {
	strcpy(nids_errbuf, "dispatch: ");
	strncat(nids_errbuf, pcap_geterr(desc), sizeof(nids_errbuf) - 11);
    }
    STOP_CAP_QUEUE_PROCESS_THREAD(); 
    return r;
}
