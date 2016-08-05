/*
 * sessionlist.h - session tracking headers
 *
 * 2009, 2010 Mike Schiffman <mschiffm@cisco.com> 
 *
 * Copyright (c) 2010 by Cisco Systems, Inc.
 * All rights reserved.
 * Based off of tcpxtract by Nicholas Harbour.
 */

/* This file is part of
   Tcpxtract, a sniffer that extracts files based on headers
   by Nick Harbour
*/

#ifndef HASH_H
#define HASH_H

#include <pcap.h>
#include <sys/types.h>
#include <inttypes.h>
#include "search.h"
//#include "extract.h"
#include <unistd.h>
#include <time.h>

#include <sys/ioctl.h>
#include <sys/resource.h>
#include <termios.h>

#define FNV_PRIME         0x811C9DC5
#define SESSION_THRESHOLD 30	    /** a session will stale out in 30s */
#define NFEX_HT_SIZE      99991	    /** randomly chosen largish prime */
#ifndef FILENAME_BUFFER_SIZE
#define FILENAME_BUFFER_SIZE 4096
#endif

struct four_tuple {
	uint32_t ip_src;
	uint32_t ip_dst;
	uint16_t port_src;
	uint16_t port_dst;
};
typedef struct four_tuple four_tuple_t;

/** statistics */
struct session_statistics {
	uint32_t total_packets;	/* total packets seen */
	uint32_t packet_bytes;	/* total packets seen */
	uint64_t total_bytes;	/* total bytes read */
	uint32_t packet_errors;	/* packet-level errors */
	uint32_t ss_nc;		/* hash table: non-chained entries */
	uint32_t ss_ic;		/* hash table: chained entries */
	uint32_t ss_lc;		/* hash table: longest chain */
	struct timeval ts_start;	/* total uptime timestamp */
	struct timeval ts_last;	/* last file extracted timestamp */
};
typedef struct session_statistics ss_stats_t;

struct session_table_node {
	four_tuple_t ft;	/* four tuple information */
	uint16_t   protocol_type;	/* this session protocol information */
	char        fn[128];	/* origin file name */
	time_t timestamp;	/* the last time a packet was seen */
	ss_stats_t ssptr_list;	/* current search threads */
	char *data;
	struct session_table_node *next;	/* next entry in the list */
	struct session_table_node *prev;	/* prev entry in the list */
};
typedef struct session_table_node ss_node_t;

/** statistics */
struct nfex_statistics {
	uint32_t ht_entries;	/* hash table: number of entries */
	uint32_t ht_nc;		/* hash table: non-chained entries */
	uint32_t ht_ic;		/* hash table: chained entries */
	uint32_t ht_lc;		/* hash table: longest chain */
};
typedef struct nfex_statistics n_stats_t;

/** monolithic opaque control context, everything imporant is here */
struct nfex_control_context {
	ss_node_t *ht[NFEX_HT_SIZE];	/* our hash table of sessions */
	ss_node_t *session;	/* current session in focus */
	uint16_t flags;		/* control context flags */
	n_stats_t n_stats;	/* stats */
	char errbuf[PCAP_ERRBUF_SIZE];	/* bad things reported here */
};
typedef struct nfex_control_context ncc_t;

/** session table functions */
ss_node_t *ht_insert(four_tuple_t * ft, ncc_t * ncc);
ss_node_t *ht_find(four_tuple_t * ft, ncc_t * ncc);
ss_node_t *ht_get(four_tuple_t * ft, ncc_t * ncc, uint32_t hash_num);
uint32_t ht_hash(four_tuple_t * ft);
uint32_t ht_count_extracts(ncc_t * ncc);
void ht_session_setnum(four_tuple_t * ft, ncc_t * ncc, uint32_t num);
void ht_dump(ncc_t * ncc);
void ht_free(ncc_t * ncc);
void ht_tabledown(four_tuple_t * ft, ncc_t * ncc, uint32_t hash_num);
void ht_status(ncc_t * ncc);
void ht_expire_session(ncc_t * ncc);
int delete_node(ncc_t * pHashTbl, ss_node_t * p, four_tuple_t * ft);

#endif /* HASH_H */
