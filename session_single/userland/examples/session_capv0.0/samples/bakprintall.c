/*
Copyright (c) 1999 Rafal Wojtczuk <nergal@7bulls.com>. All rights reserved.
See the file COPYING for license details.
*/

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/in_systm.h>
#include <arpa/inet.h>
#include <string.h>
#include <stdio.h>
#include "nids.h"
#include <stdlib.h>
#include <unistd.h>
#include <libnet.h>
//#include "thread_pool.h"
//#include "hash.h"

//#include "pfring.h"
//#include "pfutils.c"

#define int_ntoa(x)	inet_ntoa(*((struct in_addr *)&x))

//#define THD_NUM 100 

//static pthread_mutex_t lock;
//static unsigned exit_cnt;
//TpThreadPool *pTp;
//int    thread_num;
//ncc_t  *ncc;
//four_tuple_t ft;
struct half_stream *hlf;

#if 0
void proc_fun(void *arg)
{
	int i;
	uint32_t n;
	char buf[1024];
	ss_node_t *p;
	TpThreadInfo *pTinfo = (TpThreadInfo *) arg;
	four_tuple_t *temp = (four_tuple_t *) pTinfo->arg;
//      pthread_mutex_lock(&pTinfo->thread_lock);
//      pTinfo = (TpThreadInfo *) arg;
//        temp= (four_tuple_t *)pTinfo->arg;
//      strcpy (buf, int_ntoa (temp->ip_src));
//      sprintf (buf + strlen (buf), ",%i,", temp->port_src);
//      strcat (buf, int_ntoa (temp->ip_dst));
//      sprintf (buf + strlen (buf), ",%i", temp->port_dst);
//        fprintf (stderr, "%s proc_fun\n", buf);
//while(1){
//      pthread_mutex_lock(&lock);
	exit_cnt++;
//        while((p=ht_find(temp, ncc)) != NULL)
	{
//        fprintf (stderr, "%s proc_fun start\n", &temp);
//             write(1,p->data,p->ssptr_list.total_bytes); // we print the newly arrived data
		/*添加协议解析处理流程代码 */
		/*添加文件提取处理流程代码 */
//          if(delete_node(ncc, p, temp))
		printf("XXXXXXXXXXXX\n");
//        write(1,p->data,p->ssptr_list.total_files); // we print the newly arrived data          
	}
//        printf("proc_fun ip source number is %d!!!!!!!!!\n",temp->ip_src);
//        printf("proc_fun ip dest number is %d!!!!!!!!!\n",temp->ip_dst);
//        printf("proc_fun ip source port is %d!!!!!!!!!\n",temp->port_src);
//        printf("proc_fun ip dest port is %d!!!!!!!!!\n",temp->port_dst);
//      pthread_mutex_unlock(&pTinfo->thread_lock);
//      pthread_mutex_unlock(&lock);
	if (exit_cnt == THD_NUM)
		exit_cnt = 0;
//              tp_exit(pTp);
//      pthread_mutex_unlock(&pThi->thread_lock);
//  }
}
#endif
// struct tuple4 contains addresses and port numbers of the TCP connections
// the following auxiliary function produces a string looking like
// 10.0.0.1,1024,10.0.0.2,23
char *adres(struct tuple4 addr)
{
	static char buf[256];
	strcpy(buf, int_ntoa(addr.saddr));
	sprintf(buf + strlen(buf), ",%i,", addr.source);
	strcat(buf, int_ntoa(addr.daddr));
	sprintf(buf + strlen(buf), ",%i", addr.dest);
	return buf;
}

void tcp_callback(struct tcp_stream *a_tcp, void **this_time_not_needed)
{
	char buf[1024];
//  ss_node_t  *p;
	strcpy(buf, adres(a_tcp->addr));	// we put conn params into buf
//      ft.ip_src   = a_tcp->addr.saddr;
//      ft.ip_dst   = a_tcp->addr.daddr;
//      ft.port_src = 0;
//      ft.port_src = a_tcp->addr.source;
//      ft.port_dst = a_tcp->addr.dest;

	if (a_tcp->nids_state == NIDS_JUST_EST) {
		// connection described by a_tcp is established
		// here we decide, if we wish to follow this stream
		// sample condition: if (a_tcp->addr.dest!=23) return;
		// in this simple app we follow each stream, so..
		a_tcp->client.collect++;	// we want data received by a client
		a_tcp->server.collect++;	// and by a server, too
		a_tcp->server.collect_urg++;	// we want urgent data received by a
		// server
#ifdef WE_WANT_URGENT_DATA_RECEIVED_BY_A_CLIENT
		a_tcp->client.collect_urg++;	// if we don't increase this value,
		// we won't be notified of urgent data
		// arrival
#endif
		fprintf(stderr, "%s established\n", buf);
		return;
	}
#if 1
	if (a_tcp->nids_state == NIDS_CLOSE) {
		// connection has been closed normally
		fprintf(stderr, "%s closing\n", buf);
//        printf("call_back close ip source number is %d!!!!!!!!!\n",ft.ip_src);
//        printf("call_back close ip dest number is %d!!!!!!!!!\n",ft.ip_dst);
//        printf("call_back close ip source port is %d!!!!!!!!!\n",ft.port_src);
//        printf("call_back close ip dest port is %d!!!!!!!!!\n",ft.port_dst);
#if 0
//      if((p=ht_find(&ft, ncc))!=NULL){
		if (thread_num < THD_NUM) {
			tp_process_job(pTp, proc_fun, &ft);
			thread_num++;
		} else
			tp_process_job(pTp, proc_fun, &ft);
//      }
#endif
		return;
	}
	if (a_tcp->nids_state == NIDS_RESET) {
//      ft.ip_src   = a_tcp->addr.saddr;
//      ft.ip_dst   = a_tcp->addr.daddr;
//      ft.port_src = a_tcp->addr.source;
//      ft.port_dst = a_tcp->addr.dest;
		// connection has been closed by RST
		fprintf(stderr, "%s reset\n", buf);
//        printf("call_back RESET ip source number is %d!!!!!!!!!\n",ft.ip_src);
//        printf("call_back RESET ip dest number is %d!!!!!!!!!\n",ft.ip_dst);
//        printf("call_back RESET ip source port is %d!!!!!!!!!\n",ft.port_src);
//        printf("call_back RESET ip dest port is %d!!!!!!!!!\n",ft.port_dst);
#if 0
//      if((p=ht_find(&ft, ncc))!=NULL){
		if (thread_num < THD_NUM) {
			tp_process_job(pTp, proc_fun, &ft);
			thread_num++;
		} else
			tp_process_job(pTp, proc_fun, &ft);
//      }
#endif
		return;
	}
#endif
	if (a_tcp->nids_state == NIDS_DATA) {
//      printf("!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!\n");
		// new data has arrived; gotta determine in what direction
		// and if it's urgent or not
    /** four tuple information aka "a session" */
//      ft.ip_src   = a_tcp->addr.saddr;
//      ft.ip_dst   = a_tcp->addr.daddr;
//      ft.port_src = a_tcp->addr.source;
//      ft.port_dst = a_tcp->addr.dest;

    /** attempt to add this session to the session table */
#if 1
		if (a_tcp->server.count_new_urg) {
			// new byte of urgent data has arrived 
			strcat(buf, "(urgent->)");
			buf[strlen(buf) + 1] = 0;
			buf[strlen(buf)] = a_tcp->server.urgdata;
			write(1, buf, strlen(buf));
			return;
		}
#endif
		// We don't have to check if urgent data to client has arrived,
		// because we haven't increased a_tcp->client.collect_urg variable.
		// So, we have some normal data to take care of.
		if (a_tcp->client.count_new) {
			// new data for client
			if (a_tcp->addr.daddr)
				hlf = &a_tcp->client;	// from now on, we will deal with hlf var,
			// which will point to client side of conn
//        strcat (buf, "(<-)"); // symbolic direction of data
#if 0
			if (thread_num < THD_NUM) {
				tp_process_job(pTp, proc_fun, thread_num);
				thread_num++;
			} else
				tp_process_job(pTp, proc_fun, 0);
#endif
			write(2, hlf->data, hlf->count_new);
#if 0
			pthread_mutex_lock(&lock);
//        printf("callbak function is alive\n");          
			if ((ncc->session = ht_insert(&ft, ncc)) != NULL) {
				ncc->session->ssptr_list.total_packets++;
				ncc->session->ssptr_list.packet_bytes = hlf->count_new;
				ncc->session->ssptr_list.total_bytes += hlf->count_new;
				ncc->session->ssptr_list.packet_errors = 0;
				ncc->session->ssptr_list.ss_nc = 0;
				ncc->session->ssptr_list.ss_ic++;
//        ncc->session->ssptr_list.ss_lc++;
				gettimeofday(&ncc->session->ssptr_list.ts_last, NULL);
//        printf("??????????????????????????????\n");          
				if (ncc->session->data = malloc(hlf->count_new))
					memcpy(ncc->session->data, hlf->data, hlf->count_new);	//          ncc->session->next->data = &temp_buf;
//        printf("this function is still alive\n");   
//        fprintf (stderr, "%s data arrived!!\n", buf);
				printf("there are new data arrived,the number is %d!!!\n", hlf->count_new);
			} else
				printf("there are NULL hash entry!!!\n");

			pthread_mutex_unlock(&lock);
#endif
		} else {
			hlf = &a_tcp->server;	// analogical
//        strcat (buf, "(->)");
		}
//      fprintf(stderr,"%s",buf); // we print the connection parameters
		// (saddr, daddr, sport, dport) accompanied
		// by data flow direction (-> or <-)

//   write(2,hlf->data,hlf->count_new); // we print the newly arrived data

	}
	return;
}

int main()
{
	// here we can alter libnids params, for instance:
	// nids_params.n_hosts=256;
//    pTp= tp_create(10, THD_NUM);
	int i;

//    thread_num = 0;
//    exit_cnt = 0;
    /** gather all the memory we need for a control context */
//    ncc = malloc(sizeof (ncc_t));
//    if (ncc == NULL)
//    {
//        printf("XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX\n");
//        snprintf(errbuf, PCAP_ERRBUF_SIZE, "malloc(): %s\n", strerror(errno));
//        return (NULL);
//    }

    /** initialize certain elements of the control context */
//    memset(ncc, 0, sizeof (ncc_t));

//    pthread_mutex_init(&lock, NULL);
//    tp_init(pTp);
//  srand((int)time(0));

	if (!nids_init()) {
		fprintf(stderr, "%s\n", nids_errbuf);
		exit(1);
	}
	nids_register_tcp(tcp_callback);

//  while(1){
//        usleep(1000);
//      tp_process_job(pTp, proc_fun, i);
//  }
//  free(pTp);
//  fprintf(stderr, "All jobs done!\n");
//  tp_run(pTp);

	nids_run();
	return 0;
}
