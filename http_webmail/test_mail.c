size_t http_parser_execute(http_parser *parser,
                           const http_parser_settings *settings,
                           const char *data,
                           size_t len);

struct http_parser {
  /** PRIVATE **/
  unsigned int type : 2;         /* enum http_parser_type */
  unsigned int flags : 6;        /* F_* values from 'flags' enum; semi-public */
  unsigned int state : 8;        /* enum state from http_parser.c */
  unsigned int header_state : 8; /* enum header_state from http_parser.c */
  unsigned int index : 8;        /* index into current matcher */

  uint32_t nread;          /* # bytes read in various scenarios */
  uint64_t content_length; /* # bytes in body (0 if no Content-Length header) */

  /** READ-ONLY **/
  unsigned short http_major;
  unsigned short http_minor;
  unsigned int status_code : 16; /* responses only */
  unsigned int method : 8;       /* requests only */
  unsigned int http_errno : 7;

  /* 1 = Upgrade header was present and the parser has exited because of that.
   * 0 = No upgrade header present.
   * Should be checked when http_parser_execute() returns in addition to
   * error checking.
   */
  unsigned int upgrade : 1;

  /** PUBLIC **/
  void *data; /* A pointer to get hook to the "connection" or "socket" object */
};

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

static int currently_parsing_eof;

static struct message messages[5];

//parser->data only有效
static http_parser *parser;
static int num_messages;


void
parser_init (enum http_parser_type type)
{
  num_messages = 0;

  //判断parser，为NULL则abort
  assert(parser == NULL);

  //此处初始化
  //初始化的时候不传入值，这里 
  parser = malloc(sizeof(http_parser));

  http_parser_init(parser, type);

  memset(&messages, 0, sizeof messages);
}


-----------------------------------------------------------------------------
size_t parse (const char *buf, size_t len)
{
  size_t nparsed;
  currently_parsing_eof = (len == 0);
  //执行
  nparsed = http_parser_execute(parser, &settings, buf, len);
  return nparsed;
}
------------------------------------------------------------------------------
void
test_simple (const char *buf, enum http_errno err_expected)
{
  parser_init(HTTP_REQUEST);

  size_t parsed;
  int pass;
  enum http_errno err;

  //这里执行
  parsed = parse(buf, strlen(buf));
  pass = (parsed == strlen(buf));

  err = HTTP_PARSER_ERRNO(parser);
  parsed = parse(NULL, 0);
  pass &= (parsed == 0);

  parser_free();

  /* In strict mode, allow us to pass with an unexpected HPE_STRICT as
   * long as the caller isn't expecting success.
   */
#if HTTP_PARSER_STRICT
  if (err_expected != err && err_expected != HPE_OK && err != HPE_STRICT) {
#else
  if (err_expected != err) {
#endif
    fprintf(stderr, "\n*** test_simple expected %s, but saw %s ***\n\n%s\n",
        http_errno_name(err_expected), http_errno_name(err), buf);
    abort();
  }
}

-----------------------------------------------------------------------
void test(const char *buf, enum http_errno err_expected)
{
	parser_init(HTTP_REQUEST);

	size_t parsed;
	int pass;
	enum http_errno err;

	parsed = parse(buf, strlen(buf));

}

