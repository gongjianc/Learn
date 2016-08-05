/*
 * PfcountWL.c
 *
 *  Created on: Aug 21, 2014
 *      Author: siyu
 */
#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <pfring.h>
#include <string.h>
#include <errno.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <linux/if_ether.h>
#include <linux/ip.h>
#include <linux/tcp.h>
#include <getopt.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include<errno.h>
#include"msgqueue.h"
#include <jansson.h>
#include "pfcountWL.h"
#include "log.h"
#include "plog.h"

#include <pcap/pcap.h>
#include <pcap/bpf.h>
#define SNAP_LEN 1518

//char *sips,char *dips
FilterInfo *WL_rules = NULL;
FilterInfo *WL_rules_copy = NULL;
int WaitUpdateRules(void)
{
	printf("WaitUpdateRules  start....\n");
	int ret;
	FilterInfo *temp = NULL;
	char *buf[32]={0};
	while(1){

		ret = receive_msg(buf);
		log_printf(ZLOG_LEVEL_INFO, "WL->update Rules MMMMMM ....\n");
		if (ret == 1) {
			if (temp != NULL) {
				json_decref(temp->JsonRoot);
				free(temp);
				temp = NULL;
			}
		if((WL_rules->use_flag==0)&&(WL_rules->up_flag==0))
		{
		  WL_rules = ReadShareMemory();
		  printf("%s\n",json_dumps(WL_rules->JsonRoot,JSON_PRESERVE_ORDER));
		  WL_rules->use_flag = 0;
		  WL_rules->up_flag = 1;
		}else if((WL_rules_copy->use_flag==0)&&(WL_rules_copy->up_flag==0)){
		    log_printf(ZLOG_LEVEL_INFO, "WL->update Rules ....\n");
		    WL_rules_copy = ReadShareMemory();
			WL_rules_copy->use_flag = 0;
			WL_rules_copy->up_flag = 1;
		}
		} else {
		    log_printf(ZLOG_LEVEL_INFO, "WL->update Rules failed....\n");
			if (ret == 0) {
					WL_rules_copy->use_flag = 0;
					WL_rules_copy->up_flag = 0;
					WL_rules->use_flag = 0;
					WL_rules->up_flag = 0;

					if (temp != NULL) {
						free(temp);
						temp = NULL;
					}
			}
		}
	}

}


/*********************************************************/
int main(){
	if (zlog_init("wl.conf")) {
		printf("init failed");
		return -1;
	}
	ftl_log = zlog_get_category(LOG4C_CATEGORY_FTL);
	err_log = zlog_get_category(LOG4C_CATEGORY_ERR);
	wrn_log = zlog_get_category(LOG4C_CATEGORY_WRN);
	dbg_log = zlog_get_category(LOG4C_CATEGORY_DBG);
	inf_log = zlog_get_category(LOG4C_CATEGORY_INF);
	ntc_log = zlog_get_category(LOG4C_CATEGORY_NTC);
	FilterInfo *tmpRules = NULL;
	int ret;
	pthread_t thread1_id;
	ret=pthread_create(&thread1_id, NULL, (void *)WaitUpdateRules,NULL);
	if (ret){
		log_printf(ZLOG_LEVEL_ERROR,"Create pthread error!");
		return -3;
	}
	/*
	void *status;
	pthread_join(&thread1_id,&status);
	if(*status==1){
		log_printf(ZLOG_LEVEL_ERROR," WaitUpdateRules error");
		return -3;
	}
	*/
	WL_rules=malloc(sizeof(FilterInfo));
	if(WL_rules==NULL){
		log_printf(ZLOG_LEVEL_ERROR,"WL->malloc error[%d]...",__LINE__);
		exit(-1);
	}
	//bzero(rules,sizeof(rules));
	memset(WL_rules,0,sizeof(WL_rules));
	WL_rules_copy=malloc(sizeof(FilterInfo));
	if(WL_rules_copy==NULL){
		log_printf(ZLOG_LEVEL_ERROR,"WL->malloc error[%d]...",__LINE__);
		exit(-1);
	}
	//bzero(rules2,sizeof(rules2));
	memset(WL_rules_copy,0,sizeof(WL_rules_copy));
	while(1){
		printf("rules->use_flag is %d\n",WL_rules->use_flag);
		printf("rules->up_flag is %d\n",WL_rules->up_flag);
		printf("rules2->use_flag is %d\n",WL_rules_copy->use_flag);
		printf("rules2->up_flag is %d\n",WL_rules_copy->up_flag);
		if((WL_rules->use_flag==0) && (WL_rules->up_flag==0) && (WL_rules_copy->use_flag==0) && (WL_rules_copy->up_flag==0))
		{
		  sleep(1);
		  printf("Rules is NULL\n");
		}
		else
		{
		    sleep(1);
			ret = compareIP("192.168.1.13", "192.168.1.112");
			if (ret == 1) {
				continue;
				//return 1;
			}
			printf("******************************************************\n");
		}

	}

	return -1;
}

