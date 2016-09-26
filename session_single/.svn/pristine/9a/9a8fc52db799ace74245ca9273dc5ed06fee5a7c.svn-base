/**********************log.h文件开始**************************/
/*---------------------------------------------------------------
 * log.h - 记录函数定义 
*---------------------------------------------------------------
*/

#ifndef __LOG_H
#define __LOG_H

#include<stdio.h>
#include<semaphore.h>

/* 记录的最大长度 */
#define LOGLINE_MAX 1024
/* 记录的等级 */
#define DEBUG 1
#define INFO 2
#define WARN 3
#define ERROR 5
#define FATAL 5
/* 记录的类型 */
#define LOG_TRUNC 1<<0
#define LOG_NODATE 1<<1
#define LOG_NOLF 1<<2
#define LOG_NOLVL 1<<3
#define LOG_DEBUG 1<<4
#define LOG_STDERR 1<<5
#define LOG_NOTID 1<<6

typedef struct
{
	int fd;
	sem_t sem;
	int flags;
}log_t;

//log_t *log;

int lprintf(log_t *log, unsigned int level, char *fmt, ...);

log_t *log_open(char *fname, int flags);

void log_close(log_t *log);
#endif
