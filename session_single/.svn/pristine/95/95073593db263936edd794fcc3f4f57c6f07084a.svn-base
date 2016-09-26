#ifndef  SESSION_BUF_H
#define  SESSION_BUF_H

#if 0
#define TCPIPPACK
#endif


#include <stdio.h>

#ifndef TCPIPPACK
#include <pcap.h>
#endif

#include <semaphore.h>
#include <pthread.h>
#define MAX_SRING_BUFFER_COUNT 50

/** 
 * Structure of Each node for a ring buffer 
 * 五元组信息 
 *
 * **/
typedef struct five_attr 
{
    unsigned int ip_src;     //原IP
    unsigned int ip_dst;     //目的IP
    unsigned short port_src;   //源端口
    unsigned short port_dst;   //目的端口
    unsigned int protocol;   //本文件网络传输的协议
} session_ft;             //五元组信息

/* 
 *ftp协议解析信息
 *
 * */
typedef struct session_buffer_node_tag
{
    session_ft  session_five_attr;  //五元组信息
    char        strname[128];       //本地存储文件名
    char        strpath[128];       //文件路径
	char        orgname[128];       //网络上传输的原始文件名
	FILE        *old_file;
	void        *attr;

}SESSION_BUFFER_NODE;


/**Decleration of Header Structure**/

int sbuff_putdata(int iringbuffer_ID, SESSION_BUFFER_NODE ftt);

#endif
