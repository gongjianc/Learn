#
# Makefile for the PF_RING API example programs
#
# Try to make both programs written on top of the
# PF_RING library and programs originally written
# on top of PCAP library and linked now to use
# the version with PF_RING aware functionalities.
#

#INCLUDEGLIB = -I /usr/include/glib-2.0 -I /usr/lib64/glib-2.0/include -I/usr/local/include/glib-2.0 -I/usr/local/lib/glib-2.0/include 
INCLUDEGLIB = -I /usr/local/include/glib-2.0 -I/usr/local/lib/glib-2.0/include -I /root/libchar/libchardet-0.0.4/src -I /root/libchar/libiconv-1.14/include
INCLUDEGMIME = -I /usr/local/include/gmime-2.6/ 
INCLUDEGLAG=$(INCLUDEGLIB) $(INCLUDEGMIME) 

#
# PF_RING
#
PFRINGDIR  = ../lib
LIBPFRING  = ${PFRINGDIR}/libpfring.a

#
# DPI
#
DPI_INC    = nDPI/src/include/

#
# XML
#
XML_INC    = -I /usr/include/libxml2

#
# DNA Support
DNA_DEFINE = #-DENABLE_DNA_SUPPORT
#
# TIKA File Parser Support
TIKA_INC = -I /usr/java/jdk1.8.0_05/include -I /usr/java/jdk1.8.0_05/include/linux 
TIKA_LIB = -L /usr/java/jdk1.8.0_05/lib/ -L /usr/java/jdk1.8.0_05/jre/lib/amd64/server/ -L /root/libchar/libchardet-0.0.4/src -L /root/libchar/libiconv-1.14/lib/.libs
TIKA_SLIB = -ljansson -ljvm

#
# PF_RING aware libpcap
#
O_FLAG     = -O3 -DHAVE_PF_RING -D_BSD_SOURCE -DLIBNET_VER=1 -DHAVE_ICMPHDR=1 -DHAVE_TCP_STATES=1 -DHAVE_BSD_UDPHDR=1
EXTRA_LIBS =
PCAPDIR    = ../libpcap-1.1.1-ring
LIBPCAP    = ${PCAPDIR}/libpcap.a  ${EXTRA_LIBS}

#
# Search directories
#
PFRING_KERNEL=../../kernel
INCLUDE    = -I${PFRING_KERNEL}  ${INCLUDEGLAG} -I${PFRING_KERNEL}/plugins -I${PFRINGDIR} -I${PCAPDIR} -I$(DPI_INC) ${TIKA_INC} ${XML_INC} -Ithird-party

#
# C compiler and flags
#
#
# CROSS_COMPILE=arm-mv5sft-linux-gnueabi-
#
CC         = ${CROSS_COMPILE}gcc #--platform=native
CFLAGS     =  ${O_FLAG} -Wall ${INCLUDE} ${DNA_DEFINE}  -D ENABLE_BPF -D HAVE_LIBNUMA -O3  -g
# LDFLAGS  =

#
# User and System libraries
#
LIBS       = ${LIBPCAP} ${LIBPFRING} -lm -luuid -lglib-2.0 -lgmime-2.6 -lpcre -lzlog -pthread -ldl -lsqlite3 -lpython2.6 -ljansson -I /usr/include/libxml2 -L /usr/lib -lxml2  -lpthread  ${LIBPCAP} -lndpi -lnet -lnsl -pthread -lgthread-2.0 -lglib-2.0 -lnuma -lrt -luchardet -liconv

# How to make an object file
%.o: %.c pfutils.c
#	@echo "=*= making object $@ =*="
	${CC} ${CFLAGS} -c $< -o $@

#
# Main targets
#
PFPROGS   = session_cap 

TARGETS   = ${PFPROGS} ${PCAPPROGS}

GMIMESMTP= gmime_smtp.o  smtp_dissect.o smtpfunc.o
FTPDEP=fileparse.o ftp_extract.o update_json.o extractmain.o
HTTPDEP=http_parser.o webmail_parser.o webmail_attach.o parsertrace.o http_IM_parser.o http_netdisk.o read_IM_config.o  http_fei_xin.o
THREADPOOL=
MYLOG=plog.o
WLOBJ=msgqueue.o pfcountFilter.o

SESSIONDEP = session_cap.o checksum.o ip_fragment.o ip_options.o killtcp.o scan.o tcp.o hash.o util.o allpromisc.o -L/usr/local/lib  ${MYLOG} ${WLOBJ} ${THREADPOOL} ${HTTPDEP}  ${FTPDEP} ${GMIMESMTP} ${LIBS} ${TIKA_LIB} ${TIKA_SLIB} -o $@ 
all: ${TARGETS}

session_cap: session_cap.o ${LIBPFRING}
	${CC} ${CFLAGS}  ${SESSIONDEP} 
clean:
	rm -f *.o session_cap extractmain.h fileparse.h ftp_extract.h gmime_smtp.h smtp_dissect.h  smtpfunc.h  update_json.h http_parser.h http_IM_parser.h parsertrace.h webmail_attach.h webmail_parser.h pfcountWL.h msgqueue.h plog.h ip_fragment.h checksum.h  nids.h scan.h tcp.h util.h

