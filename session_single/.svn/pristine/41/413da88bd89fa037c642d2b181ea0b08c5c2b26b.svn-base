#
# Makefile for libnids samples.
#
# Dug Song <dugsong@monkey.org>

srcdir		= .


CC		= gcc
CFLAGS		= -g -O2 -D_BSD_SOURCE -DHAVE_PF_RING
LDFLAGS		= 

DPI_INC         = -I../../src/include/

PCAP_CFLAGS	= -I/usr/local/include
PCAPLIB		= -L/usr/local/lib -lpcap

LNET_CFLAGS	= $(DPI_INC) -I/usr/local/include -D_BSD_SOURCE -D__BSD_SOURCE -D__FAVOR_BSD -DHAVE_NET_ETHERNET_H  -D_SVID_SOURCE 
LNETLIB		= -lnet

LIBS_CFLAGS	= -I../src $(PCAP_CFLAGS) $(LNET_CFLAGS)
LIBS		= -L../src -lndpi -lnids $(PCAPLIB) $(LNETLIB) -lgthread-2.0 -lnsl -pthread #-lpcre

all: session_hash http_parser thread_pool tsqueue overflows session_cap sniff 
 
static shared: all

.c.o:
	$(CC) -c $(CFLAGS) -I. $(LIBS_CFLAGS) $<

session_hash: session_hash.o
	$(CC) -fPIC $(CFLAGS) -I. $(LIBS_CFLAGS) -c session_hash.c -o $@

http_parser: http_parser.o
	$(CC) -fPIC $(CFLAGS) -I. $(LIBS_CFLAGS) -c http_parser.c -o $@

thread_pool: thread_pool.o
	$(CC) $(CFLAGS)  $(LIBS) -c thread_pool.c  -o $@

tsqueue: tsqueue.o
	$(CC) $(CFLAGS) $(LIBS) -c tsqueue.c -o $@

overflows: overflows.o
	$(CC) -o $@ overflows.o $(LDFLAGS) $(LIBS)

session_cap: session_cap.o 
	$(CC) -o $@ session_cap.o thread_pool.o tsqueue.o session_hash.o http_parser.o $(LDFLAGS) $(LIBS)
#	$(CC) -o $@ printall.o $(LDFLAGS) $(LIBS)

sniff: sniff.o
	$(CC) -o $@ sniff.o $(LDFLAGS) $(LIBS)

static shared install installshared:
	@true

clean:
	rm -f *.o *~ overflows session_cap sniff

# EOF
