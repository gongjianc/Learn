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
#include "thread_pool.h"
#include "session_hash.h"
#include "http_parser.h"
#include <assert.h>
#include <stdarg.h>
#include <fcntl.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/time.h>
#include <time.h>
#include "session_buf.h"

#define  OVECCOUNT 30    /* should be a multiple of 3 */
#define  EBUFLEN 128
#define  BUFLEN 1024 
//#include "pfring.h"
//#include "pfutils.c"

#define int_ntoa(x)	inet_ntoa(*((struct in_addr *)&x))
#define KEY     (key_t)0x1fff

#define THD_NUM 100
long long    packets_num,session_num;
static pthread_mutex_t lock;
static unsigned exit_cnt;
TpThreadPool *pTp;
int thread_num;
ncc_t *ncc;
four_tuple_t ft;
struct half_stream *hlf;

struct msgbuf {
  long         mtype;
  uint32_t     proto_type;
  four_tuple_t frtt;
  char         f_path[80];
};

struct msgbuf msgque;
int    msgid;

struct session_buffer_holder   sb_holder[MAX_SRING_BUFFER_COUNT]={NULL};
int buffer_id;

char *pat1="POST[\\w\\W]*?(?=\\r\\n\\r\\n)";	
//char *pat1 = "^220[\x09-\x0d -~]*ftp";
//char *pat1="Host.*?/.mail/..*?.(com|cn|net)";//判断是否是webmail
//char *pat1 = "^220.*";	//需要匹配的正则表达式
//char *pat2 = "^(EHLO|HELO).*";

#define MAX_HEADERS 13
#define MAX_ELEMENT_SIZE 2048

#define MIN(a,b) ((a) < (b) ? (a) : (b))

static int num_messages;

static http_parser *parser;
static http_parser_settings *current_pause_parser;

struct message {
  const char *name; // for debugging purposes
  const char *raw;
  enum http_parser_type type;
  enum http_method method;
  int status_code;
  char response_status[MAX_ELEMENT_SIZE];
  char request_path[MAX_ELEMENT_SIZE];
  char request_url[MAX_ELEMENT_SIZE];
  char fragment[MAX_ELEMENT_SIZE];
  char query_string[MAX_ELEMENT_SIZE];
  char body[MAX_ELEMENT_SIZE];
  size_t body_size;
  const char *host;
  const char *userinfo;
  uint16_t port;
  int num_headers;
  enum { NONE=0, FIELD, VALUE } last_header_element;
  char headers [MAX_HEADERS][2][MAX_ELEMENT_SIZE];
  int should_keep_alive;

  const char *upgrade; // upgraded body

  unsigned short http_major;
  unsigned short http_minor;

  int message_begin_cb_called;
  int headers_complete_cb_called;
  int message_complete_cb_called;
  int message_complete_on_eof;
  int body_is_final;
};

static struct message messages[5];
static int currently_parsing_eof;
/* ************************************ */
/* strnlen() is a POSIX.2008 addition. Can't rely on it being available so
 * define it ourselves.
 */
size_t
strnlen(const char *s, size_t maxlen)
{
  const char *p;

  p = memchr(s, '\0', maxlen);
  if (p == NULL)
    return maxlen;

  return p - s;
}

size_t
strlncat(char *dst, size_t len, const char *src, size_t n)
{
  size_t slen;
  size_t dlen;
  size_t rlen;
  size_t ncpy;

  slen = strnlen(src, n);
  dlen = strnlen(dst, len);

  if (dlen < len) {
    rlen = len - dlen;
    ncpy = slen < rlen ? slen : (rlen - 1);
    memcpy(dst + dlen, src, ncpy);
    dst[dlen + ncpy] = '\0';
  }

  assert(len > slen + dlen);
  return slen + dlen;
}

size_t
strlcat(char *dst, const char *src, size_t len)
{
  return strlncat(dst, len, src, (size_t) -1);
}

size_t
strlncpy(char *dst, size_t len, const char *src, size_t n)
{
  size_t slen;
  size_t ncpy;

  slen = strnlen(src, n);

  if (len > 0) {
    ncpy = slen < len ? slen : (len - 1);
    memcpy(dst, src, ncpy);
    dst[ncpy] = '\0';
  }

  assert(len > slen);
  return slen;
}

size_t
strlcpy(char *dst, const char *src, size_t len)
{
  return strlncpy(dst, len, src, (size_t) -1);
}

static void
print_error (const char *raw, size_t error_location)
{
  fprintf(stderr, "\n*** %s ***\n\n",
          http_errno_description(HTTP_PARSER_ERRNO(parser)));

  int this_line = 0, char_len = 0;
  size_t i, j, len = strlen(raw), error_location_line = 0;
  for (i = 0; i < len; i++) {
    if (i == error_location) this_line = 1;
    switch (raw[i]) {
      case '\r':
        char_len = 2;
        fprintf(stderr, "\\r");
        break;

      case '\n':
        char_len = 2;
        fprintf(stderr, "\\n\n");

        if (this_line) goto print;

        error_location_line = 0;
        continue;

      default:
        char_len = 1;
        fputc(raw[i], stderr);
        break;
    }
    if (!this_line) error_location_line += char_len;
  }

  fprintf(stderr, "[eof]\n");

 print:
  for (j = 0; j < error_location_line; j++) {
    fputc(' ', stderr);
  }
  fprintf(stderr, "^\n\nerror location: %u\n", (unsigned int)error_location);
}

int
request_url_cb (http_parser *p, const char *buf, size_t len)
{
  assert(p == parser);
  strlncat(messages[num_messages].request_url,
           sizeof(messages[num_messages].request_url),
           buf,
           len);
  return 0;
}

int
header_field_cb (http_parser *p, const char *buf, size_t len)
{
  assert(p == parser);
  struct message *m = &messages[num_messages];

  if (m->last_header_element != FIELD)
    m->num_headers++;

  strlncat(m->headers[m->num_headers-1][0],
           sizeof(m->headers[m->num_headers-1][0]),
           buf,
           len);

  m->last_header_element = FIELD;

  return 0;
}

int
header_value_cb (http_parser *p, const char *buf, size_t len)
{
  assert(p == parser);
  struct message *m = &messages[num_messages];

  strlncat(m->headers[m->num_headers-1][1],
           sizeof(m->headers[m->num_headers-1][1]),
           buf,
           len);

  m->last_header_element = VALUE;

  return 0;
}

void
check_body_is_final (const http_parser *p)
{
  if (messages[num_messages].body_is_final) {
    fprintf(stderr, "\n\n *** Error http_body_is_final() should return 1 "
                    "on last on_body callback call "
                    "but it doesn't! ***\n\n");
    assert(0);
    abort();
  }
  messages[num_messages].body_is_final = http_body_is_final(p);
}

int
body_cb (http_parser *p, const char *buf, size_t len)
{
  assert(p == parser);
  strlncat(messages[num_messages].body,
           sizeof(messages[num_messages].body),
           buf,
           len);
  messages[num_messages].body_size += len;
  check_body_is_final(p);
 // printf("body_cb: '%s'\n", requests[num_messages].body);
  return 0;
}

int
count_body_cb (http_parser *p, const char *buf, size_t len)
{
  assert(p == parser);
  assert(buf);
  messages[num_messages].body_size += len;
  check_body_is_final(p);
  return 0;
}

int
message_begin_cb (http_parser *p)
{
  assert(p == parser);
  messages[num_messages].message_begin_cb_called = TRUE;
  return 0;
}

int
headers_complete_cb (http_parser *p)
{
  assert(p == parser);
  messages[num_messages].method = parser->method;
  messages[num_messages].status_code = parser->status_code;
  messages[num_messages].http_major = parser->http_major;
  messages[num_messages].http_minor = parser->http_minor;
  messages[num_messages].headers_complete_cb_called = TRUE;
  messages[num_messages].should_keep_alive = http_should_keep_alive(parser);
  return 0;
}

int
message_complete_cb (http_parser *p)
{
  assert(p == parser);
  if (messages[num_messages].should_keep_alive != http_should_keep_alive(parser))
  {
    fprintf(stderr, "\n\n *** Error http_should_keep_alive() should have same "
                    "value in both on_message_complete and on_headers_complete "
                    "but it doesn't! ***\n\n");
    assert(0);
    abort();
  }

  if (messages[num_messages].body_size &&
      http_body_is_final(p) &&
      !messages[num_messages].body_is_final)
  {
    fprintf(stderr, "\n\n *** Error http_body_is_final() should return 1 "
                    "on last on_body callback call "
                    "but it doesn't! ***\n\n");
    assert(0);
    abort();
  }

  messages[num_messages].message_complete_cb_called = TRUE;

  messages[num_messages].message_complete_on_eof = currently_parsing_eof;

  num_messages++;
  return 0;
}

int
response_status_cb (http_parser *p, const char *buf, size_t len)
{
  assert(p == parser);
  strlncat(messages[num_messages].response_status,
           sizeof(messages[num_messages].response_status),
           buf,
           len);
  return 0;
}
static http_parser_settings settings =
  {.on_message_begin = message_begin_cb
  ,.on_header_field = header_field_cb
  ,.on_header_value = header_value_cb
  ,.on_url = request_url_cb
  ,.on_status = response_status_cb
  ,.on_body = body_cb
  ,.on_headers_complete = headers_complete_cb
  ,.on_message_complete = message_complete_cb
  };

static http_parser_settings settings_count_body =
  {.on_message_begin = message_begin_cb
  ,.on_header_field = header_field_cb
  ,.on_header_value = header_value_cb
  ,.on_url = request_url_cb
  ,.on_status = response_status_cb
  ,.on_body = count_body_cb
  ,.on_headers_complete = headers_complete_cb
  ,.on_message_complete = message_complete_cb
  };

/* These dontcall_* callbacks exist so that we can verify that when we're
 * paused, no additional callbacks are invoked */
int
dontcall_message_begin_cb (http_parser *p)
{
  if (p) { } // gcc
  fprintf(stderr, "\n\n*** on_message_begin() called on paused parser ***\n\n");
  abort();
}

int
dontcall_header_field_cb (http_parser *p, const char *buf, size_t len)
{
  if (p || buf || len) { } // gcc
  fprintf(stderr, "\n\n*** on_header_field() called on paused parser ***\n\n");
  abort();
}

int
dontcall_header_value_cb (http_parser *p, const char *buf, size_t len)
{
  if (p || buf || len) { } // gcc
  fprintf(stderr, "\n\n*** on_header_value() called on paused parser ***\n\n");
  abort();
}

int
dontcall_request_url_cb (http_parser *p, const char *buf, size_t len)
{
  if (p || buf || len) { } // gcc
  fprintf(stderr, "\n\n*** on_request_url() called on paused parser ***\n\n");
  abort();
}

int
dontcall_body_cb (http_parser *p, const char *buf, size_t len)
{
  if (p || buf || len) { } // gcc
  fprintf(stderr, "\n\n*** on_body_cb() called on paused parser ***\n\n");
  abort();
}

int
dontcall_headers_complete_cb (http_parser *p)
{
  if (p) { } // gcc
  fprintf(stderr, "\n\n*** on_headers_complete() called on paused "
                  "parser ***\n\n");
  abort();
}

int
dontcall_message_complete_cb (http_parser *p)
{
  if (p) { } // gcc
  fprintf(stderr, "\n\n*** on_message_complete() called on paused "
                  "parser ***\n\n");
  abort();
}

int
dontcall_response_status_cb (http_parser *p, const char *buf, size_t len)
{
  if (p || buf || len) { } // gcc
  fprintf(stderr, "\n\n*** on_status() called on paused parser ***\n\n");
  abort();
}

static http_parser_settings settings_dontcall =
{
   .on_message_begin = dontcall_message_begin_cb
   ,.on_header_field = dontcall_header_field_cb
   ,.on_header_value = dontcall_header_value_cb
   ,.on_url = dontcall_request_url_cb
   ,.on_status = dontcall_response_status_cb
   ,.on_body = dontcall_body_cb
   ,.on_headers_complete = dontcall_headers_complete_cb
   ,.on_message_complete = dontcall_message_complete_cb
};

/* These pause_* callbacks always pause the parser and just invoke the regular
 * callback that tracks content. Before returning, we overwrite the parser
 * settings to point to the _dontcall variety so that we can verify that
 * the pause actually did, you know, pause. */
int
pause_message_begin_cb (http_parser *p)
{
  http_parser_pause(p, 1);
  *current_pause_parser = settings_dontcall;
  return message_begin_cb(p);
}

int
pause_header_field_cb (http_parser *p, const char *buf, size_t len)
{
  http_parser_pause(p, 1);
  *current_pause_parser = settings_dontcall;
  return header_field_cb(p, buf, len);
}

int
pause_header_value_cb (http_parser *p, const char *buf, size_t len)
{
  http_parser_pause(p, 1);
  *current_pause_parser = settings_dontcall;
  return header_value_cb(p, buf, len);
}

int
pause_request_url_cb (http_parser *p, const char *buf, size_t len)
{
  http_parser_pause(p, 1);
  *current_pause_parser = settings_dontcall;
  return request_url_cb(p, buf, len);
}

int
pause_body_cb (http_parser *p, const char *buf, size_t len)
{
  http_parser_pause(p, 1);
  *current_pause_parser = settings_dontcall;
  return body_cb(p, buf, len);
}

int
pause_headers_complete_cb (http_parser *p)
{
  http_parser_pause(p, 1);
  *current_pause_parser = settings_dontcall;
  return headers_complete_cb(p);
}

int
pause_message_complete_cb (http_parser *p)
{
  http_parser_pause(p, 1);
  *current_pause_parser = settings_dontcall;
  return message_complete_cb(p);
}

int
pause_response_status_cb (http_parser *p, const char *buf, size_t len)
{
  http_parser_pause(p, 1);
  *current_pause_parser = settings_dontcall;
  return response_status_cb(p, buf, len);
}

static http_parser_settings settings_pause =
  {.on_message_begin = pause_message_begin_cb
  ,.on_header_field = pause_header_field_cb
  ,.on_header_value = pause_header_value_cb
  ,.on_url = pause_request_url_cb
  ,.on_status = pause_response_status_cb
  ,.on_body = pause_body_cb
  ,.on_headers_complete = pause_headers_complete_cb
  ,.on_message_complete = pause_message_complete_cb
  };

/************************************************************************************************/
#if 0
/************************************************************************************************/
int Post_Con_Len(char *StoPT ,int f)
{
	char 		pattern[100] = "(?<=Content-Length:|Content-Length: )\\d*?(?=\\r\\n)";
        pcre 		*re2;
        const char 	*error;
        int 		erroffset;
        int 		ovector[OVECCOUNT];
        int 		rc, i;
	int		x;
	char		tem[30];
	int		len=strlen(StoPT);
	re2	= 	pcre_compile(pattern, 0, &error, &erroffset, NULL); 
	if 		(re2 == NULL) 
	{
                printf("PCRE compilation failed at offset %d: %s\n", erroffset, error);
                return -1;
        }
	memset(ovector,0,OVECCOUNT*sizeof(int));	
	rc=0;
	rc = pcre_exec(re2, NULL, StoPT, len,0, 0, ovector, OVECCOUNT);
	if (rc < 0) 
	{
	        if (rc == PCRE_ERROR_NOMATCH)
		{
			printf("Sorry, no more match ...\n");
			return	-1;
		}
	        else
		{
	    		printf("Matching error %d\n", rc);
	        	return	-1;
		}
	}
        char *substring_start = StoPT+ ovector[0];
        int substring_length = ovector[1] - ovector[0];
	sprintf(tem,"%.*s",substring_length, substring_start); //just first string
	x=atoi(tem);
	printf("%d\n",x);
	return(x);
}

/************************************************************************************************/

int mch(char *var,char * pattern,int len)
{
        pcre 		*re;
        const char 	*error;
        int 		erroffset;
        int 		ovector[OVECCOUNT];
		int 		Rloca=0;	/*to record the offset location each match*/
        int 		rc, i;
		int 		c=0;	/*to record the offset location each match*/
		int 		f=1;
#if 0
		static	char	StoPT[100][40000] = {};
		static	char	StoMB[100][40000] = {};
		static	int 	StoCL[100]={0};
#endif
        re = pcre_compile(pattern, 0, &error, &erroffset, NULL);
        if (re == NULL) {
                printf("PCRE compilation failed at offset %d: %s\n", erroffset, error);
                return 1;
        }
/*****************************************************************************************************/
/****************************************Extract POST RL HDR******************************************/
		/*clean ovector */
		memset(ovector,0,OVECCOUNT*sizeof(int));	
		rc=0;
		rc = pcre_exec(re, NULL, var, len,c, 0, ovector, OVECCOUNT);
		if (rc < 0) {
		        if (rc == PCRE_ERROR_NOMATCH) 
					printf("Sorry, no more match ...\n");
		        else    
					printf("Matching error %d\n", rc);
		        return 1;
		}	
		else
		{
		#if 0
			Rloca=ovector[1];			
			char *substring_start = var + ovector[0];		        
			int substring_length = ovector[1] - ovector[0];			
			sprintf(StoPT[f],"%.*s",substring_length, substring_start); //just first string			
			StoCL[f]=Post_Con_Len(StoPT[f],f);			
			sprintf(StoMB[f],"%.*s",StoCL[f],var+ovector[1]+4);			
			printf("StoMB[%d]=%s  \n ",f,StoMB[f]);			
			f++;
		#endif
//			exit_cnt++;
//			printf("\nOK, has matched,the match number is %d\n\n",exit_cnt);
		}
#if 0
	       for (i = 0; i < rc; i++)
	       {
		        char *substring_start = src + ovector[2*i];
		        int substring_length = ovector[2*i+1] - ovector[2*i];
		        printf("\n%d\n\n%.*s\n", f, substring_length, substring_start);
			sprintf(str,"%.*s",substring_length, substring_start); //just first string
			strcpy(sto2[f], str);
//			printf ("\nsto2[%d]=\n%s\n", f, sto2[f]);
			f++;
		}
#endif
		c=ovector[((rc-1)*2+1)];
/************************************************************************************************/
/************************************************************************************************/	
        return 0;
}
#endif

void
parser_init (enum http_parser_type type)
{
  num_messages = 0;

  assert(parser == NULL);

  parser = malloc(sizeof(http_parser));

  http_parser_init(parser, type);

  memset(&messages, 0, sizeof messages);

}

void
parser_free ()
{
  assert(parser);
  free(parser);
  parser = NULL;
}

size_t parse (const char *buf, size_t len)
{
  size_t nparsed;
  currently_parsing_eof = (len == 0);
  nparsed = http_parser_execute(parser, &settings, buf, len);
  return nparsed;
}

size_t parse_count_body (const char *buf, size_t len)
{
  size_t nparsed;
  currently_parsing_eof = (len == 0);
  nparsed = http_parser_execute(parser, &settings_count_body, buf, len);
  return nparsed;
}
size_t parse_pause (const char *buf, size_t len)
{
  size_t nparsed;
  http_parser_settings s = settings_pause;

  currently_parsing_eof = (len == 0);
  current_pause_parser = &s;
  nparsed = http_parser_execute(parser, current_pause_parser, buf, len);
  return nparsed;
}

static inline int
check_str_eq (const struct message *m,
              const char *prop,
              const char *expected,
              const char *found) {
  if ((expected == NULL) != (found == NULL)) {
    printf("\n*** Error: %s in '%s' ***\n\n", prop, m->name);
    printf("expected %s\n", (expected == NULL) ? "NULL" : expected);
    printf("   found %s\n", (found == NULL) ? "NULL" : found);
    return 0;
  }
  if (expected != NULL && 0 != strcmp(expected, found)) {
    printf("\n*** Error: %s in '%s' ***\n\n", prop, m->name);
    printf("expected '%s'\n", expected);
    printf("   found '%s'\n", found);
    return 0;
  }
  return 1;
}

static inline int
check_num_eq (const struct message *m,
              const char *prop,
              int expected,
              int found) {
  if (expected != found) {
    printf("\n*** Error: %s in '%s' ***\n\n", prop, m->name);
    printf("expected %d\n", expected);
    printf("   found %d\n", found);
    return 0;
  }
  return 1;
}

#define MESSAGE_CHECK_STR_EQ(expected, found, prop) \
  if (!check_str_eq(expected, #prop, expected->prop, found->prop)) return 0

#define MESSAGE_CHECK_NUM_EQ(expected, found, prop) \
  if (!check_num_eq(expected, #prop, expected->prop, found->prop)) return 0

#define MESSAGE_CHECK_URL_EQ(u, expected, found, prop, fn)           \
do {                                                                 \
  char ubuf[256];                                                    \
                                                                     \
  if ((u)->field_set & (1 << (fn))) {                                \
    memcpy(ubuf, (found)->request_url + (u)->field_data[(fn)].off,   \
      (u)->field_data[(fn)].len);                                    \
    ubuf[(u)->field_data[(fn)].len] = '\0';                          \
  } else {                                                           \
    ubuf[0] = '\0';                                                  \
  }                                                                  \
                                                                     \
  check_str_eq(expected, #prop, expected->prop, ubuf);               \
} while(0)

int
message_eq (int index, const struct message *expected)
{
  int i;
  struct message *m = &messages[index];

  MESSAGE_CHECK_NUM_EQ(expected, m, http_major);
  MESSAGE_CHECK_NUM_EQ(expected, m, http_minor);

  if (expected->type == HTTP_REQUEST) {
    MESSAGE_CHECK_NUM_EQ(expected, m, method);
  } else {
    MESSAGE_CHECK_NUM_EQ(expected, m, status_code);
    MESSAGE_CHECK_STR_EQ(expected, m, response_status);
  }

  MESSAGE_CHECK_NUM_EQ(expected, m, should_keep_alive);
  MESSAGE_CHECK_NUM_EQ(expected, m, message_complete_on_eof);

  assert(m->message_begin_cb_called);
  assert(m->headers_complete_cb_called);
  assert(m->message_complete_cb_called);


  MESSAGE_CHECK_STR_EQ(expected, m, request_url);

  /* Check URL components; we can't do this w/ CONNECT since it doesn't
   * send us a well-formed URL.
   */
  if (*m->request_url && m->method != HTTP_CONNECT) {
    struct http_parser_url u;

    if (http_parser_parse_url(m->request_url, strlen(m->request_url), 0, &u)) {
      fprintf(stderr, "\n\n*** failed to parse URL %s ***\n\n",
        m->request_url);
      abort();
    }

    if (expected->host) {
      MESSAGE_CHECK_URL_EQ(&u, expected, m, host, UF_HOST);
    }

    if (expected->userinfo) {
      MESSAGE_CHECK_URL_EQ(&u, expected, m, userinfo, UF_USERINFO);
    }

    m->port = (u.field_set & (1 << UF_PORT)) ?
      u.port : 0;

    MESSAGE_CHECK_URL_EQ(&u, expected, m, query_string, UF_QUERY);
    MESSAGE_CHECK_URL_EQ(&u, expected, m, fragment, UF_FRAGMENT);
    MESSAGE_CHECK_URL_EQ(&u, expected, m, request_path, UF_PATH);
    MESSAGE_CHECK_NUM_EQ(expected, m, port);
  }

  if (expected->body_size) {
    MESSAGE_CHECK_NUM_EQ(expected, m, body_size);
  } else {
    MESSAGE_CHECK_STR_EQ(expected, m, body);
  }

  MESSAGE_CHECK_NUM_EQ(expected, m, num_headers);

  int r;
  for (i = 0; i < m->num_headers; i++) {
    r = check_str_eq(expected, "header field", expected->headers[i][0], m->headers[i][0]);
    if (!r) return 0;
    r = check_str_eq(expected, "header value", expected->headers[i][1], m->headers[i][1]);
    if (!r) return 0;
  }

  MESSAGE_CHECK_STR_EQ(expected, m, upgrade);

  return 1;
}

void
test_message (const struct message *message ,int len)
{
//  size_t raw_len = strlen(message->raw);
  size_t raw_len = len;
  size_t msg1len;
  for (msg1len = 0; msg1len < raw_len; msg1len++) {
    parser_init(message->type);
	printf("parse start!!!!\n");

    size_t read;
    const char *msg1 = message->raw;
    const char *msg2 = msg1 + msg1len;
    size_t msg2len = raw_len - msg1len;

    if (msg1len) {
      read = parse(msg1, msg1len);

      if (message->upgrade && parser->upgrade) {
        messages[num_messages - 1].upgrade = msg1 + read;
        goto test;
      }

      if (read != msg1len) {
        print_error(msg1, read);
		printf("read != msg1len!!!!\n");
		parser_free();
		break;
        //abort();
      }
    }


    read = parse(msg2, msg2len);

    if (message->upgrade && parser->upgrade) {
      messages[num_messages - 1].upgrade = msg2 + read;
      goto test;
    }

    if (read != msg2len) {
      print_error(msg2, read);
	  printf("read != msg2len!!!!\n");
	  parser_free();
	  break;
      //abort();
    }

    read = parse(NULL, 0);

    if (read != 0) {
      print_error(message->raw, read);
	  printf("parse error!!!!\n");	  
	  parser_free();
	  break;
//      abort();
    }

  test:

    if (num_messages != 1) {
      printf("\n*** num_messages != 1 after testing '%s' ***\n\n", message->name);
	  parser_free();
	  break;
//      abort();
    }

    if(!message_eq(0, message)) 
	  printf("\n*** num_messages != 1 after testing '%s' ***\n\n", message->name);
	  parser_free();
	  break;
//		abort();
    printf("parse successfull!!!!\n");
    parser_free();
  }  
  printf("parse end!!!!\n");
}

void
test_message_count_body (const struct message *message,int len)
{
  parser_init(message->type);

  size_t read;
  size_t l = len;
  size_t i, toread;
  size_t chunk = 4024;

  for (i = 0; i < l; i+= chunk) {
    toread = MIN(l-i, chunk);
    read = parse_count_body(message->raw + i, toread);
    if (read != toread) {
      print_error(message->raw, read);
      abort();
    }
  }


  read = parse_count_body(NULL, 0);
  if (read != 0) {
    print_error(message->raw, read);
    abort();
  }

  if (num_messages != 1) {
    printf("\n*** num_messages != 1 after testing '%s' ***\n\n", message->name);
    abort();
  }

  if(!message_eq(0, message)) abort();

  parser_free();
}

/****ring_buffer****/
/**Creation of ring Buffer**/

int sbuff_create (int irb_qlength)
{
    int irb_id = check_for_first_free_sring_buffer(); // Checks for the very first free 
                                                // ring buffer in the holder array
    if (irb_id == -1)
        return -1; // No Free ring buffers

    int ring_buffer_ID=create_sring_buffer(irb_id, irb_qlength); // Create new ring buffer in the holder with ID received
        if(ring_buffer_ID<0)
            return -1; // Failed to create ring buffer
    if((init_slock_for_sync(irb_id, irb_qlength))==-1)
	return -1;       
    return ring_buffer_ID; // Returning the ring buffer ID(index) in success
    
}

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
    return -1;
}

int  create_sring_buffer(int iringbuffer_ID, int iringbuffer_qlength)
{
    sb_holder[iringbuffer_ID].rb_base = (struct session_buffer_node_tag *)calloc(
                                        iringbuffer_qlength,
                                        sizeof(struct session_buffer_node_tag));
    if ( sb_holder[iringbuffer_ID].rb_base == NULL)
        return -1;
    pthread_mutex_init(&sb_holder[iringbuffer_ID].mutex,NULL);
    sb_holder[iringbuffer_ID].irb_qlength          = iringbuffer_qlength; 
    sb_holder[iringbuffer_ID].irb_head_index       = 0; 
    sb_holder[iringbuffer_ID].irb_tail_index       = 0;
    sb_holder[iringbuffer_ID].irb_buffer_end_index = iringbuffer_qlength-1; // need justification

    return iringbuffer_ID;//returning buffer header ID
}

    
init_slock_for_sync(int iringbuffer_ID, int iringbuffer_qlength)//initializing the semaphore lock_object
{
    if(sem_init(&sb_holder[iringbuffer_ID].empty_buffer,0,iringbuffer_qlength) == -1) 
    {
	goto lebel;
    }
	
    if(sem_init(&sb_holder[iringbuffer_ID].full_buffer,0,0) == -1) 
    {   
	 sem_destroy(&sb_holder[iringbuffer_ID].empty_buffer);
  	 goto lebel;
    }
    return 0; 
    lebel : free(sb_holder[iringbuffer_ID].rb_base);//freeing alocated memory on initialisation failure...
    return -1;
}



/**Putting data on ring buffer**/

int sbuff_putdata(int iringbuffer_ID, session_ft ft)
{
//	printf("++ rbuff_putdata \n");
    if(sem_wait(&sb_holder[iringbuffer_ID].empty_buffer)==-1)
    {
       	printf("sem_wait is failed to lock empty_buffer");
       	return -1;
    }
//    if(memcpy(&sb_holder[iringbuffer_ID].rb_base[sb_holder[iringbuffer_ID].irb_tail_index].pkthdr,hdr,sizeof(hdr))!=NULL)
//    printf("ft.ip_src is %d,ft.ip_dst is %d,ft.port_src is %d,ft.port_dst is %d!!\n", ft.ip_src, ft.ip_dst, ft.port_src, ft.port_dst);
    sb_holder[iringbuffer_ID].rb_base[sb_holder[iringbuffer_ID].irb_tail_index].session_five_attr.ip_src = ft.ip_src;
    sb_holder[iringbuffer_ID].rb_base[sb_holder[iringbuffer_ID].irb_tail_index].session_five_attr.ip_dst = ft.ip_dst;
    sb_holder[iringbuffer_ID].rb_base[sb_holder[iringbuffer_ID].irb_tail_index].session_five_attr.port_src = ft.port_src;
    sb_holder[iringbuffer_ID].rb_base[sb_holder[iringbuffer_ID].irb_tail_index].session_five_attr.port_dst = ft.port_dst;
    sb_holder[iringbuffer_ID].rb_base[sb_holder[iringbuffer_ID].irb_tail_index].session_five_attr.protocol = ft.protocol;
//       printf("h->caplen is %d!!\n", sb_holder[iringbuffer_ID].rb_base[sb_holder[iringbuffer_ID].irb_tail_index].pkthdr.caplen);
    // Incrementing Tail pointer and assumes never co-inside with head pointer.

    if(sb_holder[iringbuffer_ID].irb_tail_index >= sb_holder[iringbuffer_ID].irb_qlength -1 )
    {
        sb_holder[iringbuffer_ID].irb_tail_index = 0;
    }
    else
    {
        sb_holder[iringbuffer_ID].irb_tail_index++;
    }
    if(sem_post(&sb_holder[iringbuffer_ID].full_buffer)==-1)
    {
        printf("sem_post is failed to unlock in empty_full");
        return -1;
    }
    //release the semaphore count on successfull write.
//	printf("-- rbuff_putdata \n");
    return 1;
    
}

/**Getting data from ring buffer**/


int sbuff_getdata(int iringbuffer_ID, session_ft *hdr)
{
    if(sem_wait(&sb_holder[iringbuffer_ID].full_buffer)==-1)
    {
        	printf("sem_wait is failed to lock full_buffer");
        	return -1;
    }

    if((sb_holder[iringbuffer_ID].rb_base)==NULL) // Checks for Valid Ring Buffer
        return -1;

    int temp_index;    	
    pthread_mutex_lock(&sb_holder[iringbuffer_ID].mutex);
    temp_index = sb_holder[iringbuffer_ID].irb_head_index;
    hdr->ip_src = sb_holder[iringbuffer_ID].rb_base[sb_holder[iringbuffer_ID].irb_head_index].session_five_attr.ip_src;
    hdr->ip_dst = sb_holder[iringbuffer_ID].rb_base[sb_holder[iringbuffer_ID].irb_head_index].session_five_attr.ip_dst;
    hdr->port_src = sb_holder[iringbuffer_ID].rb_base[sb_holder[iringbuffer_ID].irb_head_index].session_five_attr.port_src;
    hdr->port_dst = sb_holder[iringbuffer_ID].rb_base[sb_holder[iringbuffer_ID].irb_head_index].session_five_attr.port_dst;
    hdr->protocol = sb_holder[iringbuffer_ID].rb_base[sb_holder[iringbuffer_ID].irb_head_index].session_five_attr.protocol;
//    sb_holder[iringbuffer_ID].rb_base[temp_index].session_four_attr.ip_src = 0;
//    sb_holder[iringbuffer_ID].rb_base[temp_index].session_four_attr.ip_dst = 0;
//    sb_holder[iringbuffer_ID].rb_base[temp_index].session_four_attr.port_src = 0;
//    sb_holder[iringbuffer_ID].rb_base[temp_index].session_four_attr.port_dst = 0;
    //updating head pointer
//    printf("h->caplen is %d!!\n", sb_holder[iringbuffer_ID].irb_head_index);
    if(sb_holder[iringbuffer_ID].irb_head_index >= sb_holder[iringbuffer_ID].irb_qlength -1)

    {
        sb_holder[iringbuffer_ID].irb_head_index=0;
    }
    else        
    {
        sb_holder[iringbuffer_ID].irb_head_index++;
    }
//  sem_post(&sb_holder[iringbuffer_ID].sem_obj); //lock on empty buffer condition

    if(sem_post(&sb_holder[iringbuffer_ID].empty_buffer)==-1)
     {
       	printf("sem_post is failed to unlock");
       	return -1;
     }
    	
    pthread_mutex_unlock(&sb_holder[iringbuffer_ID].mutex);
    
//    if(idatasize < 
//        sb_holder[iringbuffer_ID].rb_base[sb_holder[iringbuffer_ID].irb_head_index].irb_node_data_size)
//        return -1; // Return with Fail if data buffer is small compared to actual data

//    printf("hdr->caplen is %d!!\n", sb_holder[iringbuffer_ID].rb_base[temp_index].pkthdr.len);
        // copying data
    
//    printf("hdr.ip_src is %d,hdr.ip_dst is %d,hdr.port_src is %d,hdr.port_dst is %d!!\n", hdr->ip_src, hdr->ip_dst, hdr->port_src, hdr->port_dst);
//    printf("hdr.caplen is %d!!\n", hdr->len);
    // freeing buffer
//    int temp=sb_holder[iringbuffer_ID].irb_head_index;
    
    return 1; // return size  in terms of byte successfully Written to buffer
}

/**Destroying Ring Buffer**/

int sbuff_destroy(int iringbuffer_ID)
{
    int i;
    
    if((sb_holder[iringbuffer_ID].rb_base)==NULL) // Checks for Valid Ring Buffer
        return -1;
        
    for(i=0; i <sb_holder[iringbuffer_ID].irb_qlength ; i++)
    {     
       sb_holder[iringbuffer_ID].rb_base[i].session_five_attr.ip_src = 0;
       sb_holder[iringbuffer_ID].rb_base[i].session_five_attr.ip_dst = 0;
       sb_holder[iringbuffer_ID].rb_base[i].session_five_attr.port_src = 0;
       sb_holder[iringbuffer_ID].rb_base[i].session_five_attr.port_dst = 0;
       sb_holder[iringbuffer_ID].rb_base[i].session_five_attr.protocol = 0;
    }
    
    sb_holder[iringbuffer_ID].rb_base        = NULL;
    sb_holder[iringbuffer_ID].irb_qlength    = 0; 
    sb_holder[iringbuffer_ID].irb_head_index = 0; 
    sb_holder[iringbuffer_ID].irb_tail_index = 0;
    sem_destroy(&sb_holder[iringbuffer_ID].empty_buffer);    
    sem_destroy(&sb_holder[iringbuffer_ID].full_buffer);    
    return 1; 
}
/****ring_buffer****/

FILE * outfile;

void session_proc_fun(session_ft *arg)
{
   ss_node_t *p;
   uint16_t hash_num;
   four_tuple_t temp_f;
   int res;
   uint32_t temp_protocol;
   temp_f.ip_src = arg->ip_src;
   temp_f.ip_dst = arg->ip_dst;
   temp_f.port_src = arg->port_src;
   temp_f.port_dst = arg->port_dst;
   temp_protocol = arg->protocol;
#if 0
//   pthread_mutex_lock(&lock);
   if ((p = ht_get(&temp_f, ncc, hash_num)) != NULL) ;
   {
      ss_node_t *temp_p,*num_p;
      four_tuple_t temp_s;
      int res;
      char name[80],fname[30];
      int fd;
      struct tm *ptr;
      struct timeval usec;
      time_t lt;
      uint32_t protocol;
      uint16_t temp_h;
      temp_s = temp_f;
      temp_p = p;
      temp_h = hash_num;
      protocol = temp_protocol;
      lt=time(NULL);
      gettimeofday(&usec, NULL);
      ptr=localtime(&lt);
      strftime(name,80,"%F%H%M%S",ptr);
      sprintf(fname, "%u.txt", usec.tv_usec);
      strcat(name, fname);
      fd = open(name, O_RDWR|O_CREAT|O_APPEND);
#if 1
      for (temp_p; temp_p != NULL; temp_p = temp_p->next){             
#if 0
//      write(2, p->data, p->ssptr_list.packet_bytes);
//	fd = open("test.txt", O_RDWR|O_CREAT|O_APPEND);
	write(fd,temp_p->data,temp_p->ssptr_list.packet_bytes); // we print the newly arrived data
//	outfile = fopen("temp.txt", "a+" );
//        printf("CCCCCCCCCCCCCCCCCCCCCCCCCCC\n");
//      fwrite( p->data, sizeof(unsigned char), p->ssptr_list.packet_bytes, outfile);
//	fseek(outfile , 0, 2);
//	temp_data = p->data;
//	temp_data += p->ssptr_list.packet_bytes;
//	total_bytes += p->ssptr_list.packet_bytes;
//	printf("temp_data addr if %o\n",temp_data);
#endif
//      free(p->data);
//      free(p);
     }
#endif
//     pthread_mutex_lock(&lock);
//     close(fd);
     msgque.mtype = 1;
     msgque.proto_type = protocol;
     msgque.frtt.ip_src = temp_s.ip_src;
     msgque.frtt.ip_dst = temp_s.ip_dst;
     msgque.frtt.port_src = temp_s.port_src;
     msgque.frtt.port_dst = temp_s.port_dst;
     strcpy(msgque.f_path, name);
     ht_tabledown(&temp_s, ncc, temp_h);
//     res = msgsnd(msgid, &msgque, sizeof(struct msgbuf), 0);
//   session_num++;
//     printf("session_num!!!\n");	
//   fclose(outfile);
   }
#endif
}

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
#if 1
   char buf[1024];
   ss_node_t *p;
   session_ft sft;

   strcpy(buf, adres(a_tcp->addr));	// we put conn params into buf
   ft.ip_src = a_tcp->addr.saddr;
   ft.ip_dst = a_tcp->addr.daddr;
   ft.port_src = a_tcp->addr.source;
   ft.port_dst = a_tcp->addr.dest;

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
//   fprintf(stderr, "%s established\n", buf);
     return;
   }
#if 1
   if (a_tcp->nids_state == NIDS_CLOSE) {
   // connection has been closed normally
#if 0
//    if((p=ht_find(&ft, ncc))!=NULL){
	if (thread_num < THD_NUM) {
	  tp_process_job(pTp, proc_fun, &ft);
	  thread_num++;
	} else
	  tp_process_job(pTp, proc_fun, &ft);
//    }
#endif
//      if(a_tcp->addr5.protocol != 0)
//         printf("close a_tcp->protocol is %d!\n", a_tcp->addr5.protocol);
      sft.ip_src = a_tcp->addr.saddr;
      sft.ip_dst = a_tcp->addr.daddr;
      sft.port_src = a_tcp->addr.source;
      sft.port_dst = a_tcp->addr.dest;
      sft.protocol = a_tcp->addr5.protocol;
      sbuff_putdata(buffer_id, sft);
      return;
   }
   if (a_tcp->nids_state == NIDS_RESET) {
   // connection has been closed by RST
   //fprintf(stderr, "%s reset\n", buf);
#if 0
   //if((p=ht_find(&ft, ncc))!=NULL){
     if (thread_num < THD_NUM) {
	tp_process_job(pTp, proc_fun, &ft);
	thread_num++;
     } else
	tp_process_job(pTp, proc_fun, &ft);
//  }
#endif
//      if(a_tcp->addr5.protocol != 0)
//         printf("RESET a_tcp->protocol is %d!\n", a_tcp->addr5.protocol);
      sft.ip_src = a_tcp->addr.saddr;
      sft.ip_dst = a_tcp->addr.daddr;
      sft.port_src = a_tcp->addr.source;
      sft.port_dst = a_tcp->addr.dest;
      sft.protocol = a_tcp->addr5.protocol;
      sbuff_putdata(buffer_id, sft);
      return;
   }
#endif
   if (a_tcp->nids_state == NIDS_DATA) {
   // new data has arrived; gotta determine in what direction
   // and if it's urgent or not
   /** four tuple information aka "a session" */

   /** attempt to add this session to the session table */
#if 0
     if (a_tcp->server.count_new_urg) {
	// new byte of urgent data has arrived 
//	strcat(buf, "(urgent->)");
	buf[strlen(buf) + 1] = 0;
	buf[strlen(buf)] = a_tcp->server.urgdata;
//      write(1,buf,strlen(buf));
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
//        write(2,hlf->data,hlf->count_new); 
#if 0
	//write(2,hlf->data,hlf->count_new); 
	  pthread_mutex_lock(&lock);
	//printf("callbak function is alive\n");		  
	  if ((ncc->session = ht_insert(&ft, ncc)) != NULL) {
//	    ncc->session->ssptr_list.total_packets++;
	    ncc->session->ssptr_list.packet_bytes = hlf->count_new;
//	    ncc->session->ssptr_list.total_bytes += hlf->count_new;
	    ncc->session->ssptr_list.packet_errors = 0;
	    ncc->session->ssptr_list.ss_nc = 0;
	    ncc->session->ssptr_list.ss_ic++;
	    //ncc->session->ssptr_list.ss_lc++;
//	    gettimeofday(&ncc->session->ssptr_list.ts_last, NULL);
//	    printf("?????????????\n");
	    if (ncc->session->data = malloc(hlf->count_new))
	      memcpy(ncc->session->data, hlf->data, hlf->count_new);	//			ncc->session->next->data = &temp_buf;
	    //write(2,hlf->data,hlf->count_new);
	    //printf("this function is still alive!\n");   
	    //fprintf (stderr, "%s data arrived!!\n", buf);
	    //printf("there are new data arrived,the number is %d!!!\n",hlf->count_new);  
	    } else
	      printf("there are NULL hash entry!!!\n");
	    pthread_mutex_unlock(&lock);
#endif
	} else {
	  hlf = &a_tcp->server;	// analogical
#if 0
	  pthread_mutex_lock(&lock);
	  //printf("callbak function is alive\n");		  
	  if ((ncc->session = ht_insert(&ft, ncc)) != NULL) {
//	    ncc->session->ssptr_list.total_packets++;
	    ncc->session->ssptr_list.packet_bytes = hlf->count_new;
//	    ncc->session->ssptr_list.total_bytes += hlf->count_new;
	    ncc->session->ssptr_list.packet_errors = 0;
	    ncc->session->ssptr_list.ss_nc = 0;
	    ncc->session->ssptr_list.ss_ic++;
	    //ncc->session->ssptr_list.ss_lc++;
	    gettimeofday(&ncc->session->ssptr_list.ts_last, NULL);
//	    printf("?????????????\n");
	    if (ncc->session->data = malloc(hlf->count_new))
	      memcpy(ncc->session->data, hlf->data, hlf->count_new);	//			ncc->session->next->data = &temp_buf;
	    //write(2,hlf->data,hlf->count_new);
	  }
	  else
	    printf("there are NULL hash entry!!!\n");
	pthread_mutex_unlock(&lock);
#endif
///    strcat (buf, "(->)");
     }
#if 1
//     pthread_mutex_lock(&lock);
     //printf("callbak function is alive\n");		  
     if ((ncc->session = ht_insert(&ft, ncc)) != NULL) {
//        ncc->session->ssptr_list.total_packets++;
        ncc->session->ssptr_list.packet_bytes = hlf->count_new;
//        ncc->session->ssptr_list.total_bytes += hlf->count_new;
        ncc->session->ssptr_list.packet_errors = 0;
        ncc->session->ssptr_list.ss_nc = 0;
	ncc->session->ssptr_list.ss_ic++;
	//ncc->session->ssptr_list.ss_lc++;
//	gettimeofday(&ncc->session->ssptr_list.ts_last, NULL);
//	printf("?????????????\n");
	if (ncc->session->data = malloc(hlf->count_new)){
	  memcpy(ncc->session->data, hlf->data, hlf->count_new);	//			ncc->session->next->data = &temp_buf;
          ht_session_setnum(&ft, ncc, hlf->count_new);
        }
//	write(2,hlf->data,hlf->count_new);
     }
     else
       printf("there are NULL hash entry!!!\n");
//     pthread_mutex_unlock(&lock);
#endif
    }
#endif
	return;
}

void* session_consumer_thread(void* _id) 
{

  long thread_id = (long)_id;

  /* printf("packet_consumer_thread(%lu)\n", thread_id); */
  session_ft *hdr;
  hdr = malloc(sizeof(session_ft));
  while(1){

    if(sbuff_getdata(buffer_id, hdr)>0){
//      printf("hdr.ip_src is %d,hdr.ip_dst is %d,hdr.port_src is %d,hdr.port_dst is %d!!\n", hdr->ip_src, hdr->ip_dst, hdr->port_src, hdr->port_dst);
//      write(1, buffer, hdr.caplen);
//     pkts_num++;
//     printf("pkts_num is %d!!\n", pkts_num);
       session_proc_fun(hdr);
    }
  }
  return(NULL);
}

int main()
{
	// here we can alter libnids params, for instance:
	// nids_params.n_hosts=256;
	pTp = tp_create(10, THD_NUM);
	int i;
	packets_num = 0;
	session_num = 0;

	thread_num = 0;
	exit_cnt = 0;
    /** gather all the memory we need for a control context */
	ncc = malloc(sizeof(ncc_t));
	if (ncc == NULL) {
		printf("XXXXXXXXXXXXXXXXXXXXXX\n");
//        snprintf(errbuf, PCAP_ERRBUF_SIZE, "malloc(): %s\n", strerror(errno));
		return (NULL);
	}

    /** initialize certain elements of the control context */
	memset(ncc, 0, sizeof(ncc_t));
        
        buffer_id = sbuff_create (200000);
        if(buffer_id < 0)
           fprintf(stderr, "sring_buffer_creat failed\n");

        key_t key = KEY;
        msgid = msgget(key, 0666|IPC_CREAT);
        if(msgid<0)
          printf("msgget!!\n");

	pthread_mutex_init(&lock, NULL);

	tp_init(pTp);
//  srand((int)time(0));

	if (!nids_init()) {
		fprintf(stderr, "%s\n", nids_errbuf);
		exit(1);
	}
	nids_register_tcp(tcp_callback);

        pthread_t my_thread[30];
        long thd_id;

        for(thd_id=0; thd_id<30; thd_id++)
          pthread_create(&my_thread[thd_id], NULL, session_consumer_thread, (void*)i);
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
