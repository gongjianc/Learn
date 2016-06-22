
#pkglib_LTLIBRARIES=srv_webmail_parser.la
INCLUDES = -I$(top_srcdir)
CONFIGDIR = @CONFDIR@
cicapdatadir=@DATADIR@
cicapmoduledir=@MODSDIR@

CFHEADERS = http_parser.h
cicapmodule_LTLIBRARIES=srv_webmail_parser.la

srv_webmail_parser_la_LIBADD = @MODULES_LIBADD@
srv_webmail_parser_la_CFLAGS =  -I../../include/
srv_webmail_parser_la_LDFLAGS= -module -avoid-version

srv_webmail_parser_la_SOURCES = srv_webmail_parser.c http_parser.c

install-data-local:
	$(INSTALL) -m 0644 srv_webmail_parser.conf $(DESTDIR)$(CONFIGDIR)/srv_webmail_parser.conf.default
	if test ! -f $(DESTDIR)$(CONFIGDIR)/srv_webmail_parser.conf.default; then $(INSTALL) -m 0644 srv_webmail_parser.conf.default $(DESTDIR)$(CONFIGDIR)/srv_webmail_parser.conf; fi


EXTRA_DIST=  $(CFHEADERS) srv_webmail_parser.conf
