#ifndef NDLP_HTTP_WHITELIST_H
#define NDLP_HTTP_WHITELIST_H

//³õÊ¼»¯¾ä±ú£¬Ê§°Ü·µ»ØNULL
//void* initWhiteList(const char *addr, unsigned int port, const char *db, const char *user, const char *passwd);
//int initWhiteListEx(void **handle, const char *addr, unsigned int port, const char *db, const char *user, const char *passwd);
////³É¹¦·µ»Ø1£¬Ê§°Ü·µ»Ø0
//int isInWhiteListEml(void *handle, const char *src, const char *dst);
//int isInWhiteListIp(void *handle, const char *src, const char *dst);
//int isInWhiteListEmlPS(void *handle, const char *src);
//int isInWhiteListEmlPD(void *handle, const char *dst);
//int isInWhiteListIpPS(void *handle, const char *src);
//int isInWhiteListIpPD(void *handle, const char *dst);
//int isInWhiteListUrlP(void *handle, const char *src);
//int updateWhiteList(void *handle);
//
////ÊÍ·Å¾ä±ú
//void releaseWhiteList(void *handle);

extern void *handle;

void init_whitelist();
void update_whitelist();

int isInIpListPS(char *src);
int isInEmlListPD(char *src);

int isInEmlListPS(char *src);
int isInEmlList(const char *src, const char *dst);

int isInUrlList(char *src);


#endif // WHITELIST_H
