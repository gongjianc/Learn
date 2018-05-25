#include <unistd.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <mysql/mysql.h>
#include <string.h>

#include "whitelist.h"
#include "plog.h"
#include "msg_utils.h"

//using namespace std;


#define MAX_TO_LEN 1024
//#define   PERM S_IRUSR | S_IWUSR   

void *handle = NULL;


void connect_mysql_db()
{
    MYSQL mysql;
    if(!mysql_real_connect(&mysql, "127.0.0.1", "root", "tmdlp", "ws_dlp", 0, NULL, 0)){

    }
}



void init_whitelist()
{
    initWhiteListEx(&handle, "127.0.0.1", 3306, "ws_dlp", "root", "tmdlp");
    /* return handle; */
}

void update_whitelist()
{
    updateWhiteList(handle);
}

int isInIpListPS(char *src)
{
    if(handle){
        ;
        /* log_printf(ZLOG_LEVEL_DEBUG, "============== HANDLE NOT NULL ======"); */
    }else
        log_printf(ZLOG_LEVEL_ERROR, "============== HANDLE IS NULL ======");

    if(recv_msg_from_daemon(10203) == 0)
        update_whitelist();

    if(isInWhiteListIpPS(handle, src))
        return 0;
    return -1;
}

int isInEmlListPS(char *src)
{
    if(handle){
        ;
        /* log_printf(ZLOG_LEVEL_DEBUG, "============== HANDLE NOT NULL ======"); */
    }else
        log_printf(ZLOG_LEVEL_DEBUG, "============== HANDLE IS NULL ======");

    if(recv_msg_from_daemon(10203) == 0)
        update_whitelist();

    if(isInWhiteListEmlPS(handle, src))
        return 0;

    return -1;
}

int isInEmlListPD(char *src)
{
    if(handle){
        ;
        /* log_printf(ZLOG_LEVEL_DEBUG, "============== HANDLE NOT NULL ======"); */
    }else
        log_printf(ZLOG_LEVEL_DEBUG, "============== HANDLE IS NULL ======");

    if(recv_msg_from_daemon(10203) == 0)
        update_whitelist();

    if(src == NULL)
        return -1;

    int i = 0;
    int j = 0;

    char res[128] = {'\0'};
    while(src[i] != '\0' && i < MAX_TO_LEN){
        if(src[i] == '<' || src[i] == '>'){
            i++;
            continue;
        }
        if(src[i] == ','){
            if(isInWhiteListEmlPD(handle, res))
                return 0;
            bzero(res, sizeof(res));
            j = 0;
            i++;
            continue;
        }
        res[j++] = src[i++];
    }
    if(j > 0){
        if(isInWhiteListEmlPD(handle, res))
            return 0;
    }

    return -1;
}

int isInEmlList(const char *src, const char *dst)
{
    if(handle){
        ;
        /* log_printf(ZLOG_LEVEL_DEBUG, "============== HANDLE NOT NULL ======"); */
    }else
        log_printf(ZLOG_LEVEL_DEBUG, "============== HANDLE IS NULL ======");

    if(recv_msg_from_daemon(10203) == 0)
        update_whitelist();

    if(isInWhiteListEml(handle, src, dst))
        return 0;

    return -1;
}

int isInUrlList(char *src)
{
    if(handle){
        ;
        /* log_printf(ZLOG_LEVEL_DEBUG, "============== HANDLE NOT NULL ======"); */
    }else
        log_printf(ZLOG_LEVEL_DEBUG, "============== HANDLE IS NULL ======");

    if(recv_msg_from_daemon(10203) == 0)
        update_whitelist();

    if(isInWhiteListUrlP(handle, src))
        return 0;

    return -1;
}

