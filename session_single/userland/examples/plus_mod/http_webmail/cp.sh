#!/bin/sh
make clean
make
cp http_parser.o  http_IM_parser.o parsertrace.o webmail_attach.o  webmail_parser.o http_netdisk.o read_IM_config.o http_fei_xin.o http_parser.h http_IM_parser.h parsertrace.h webmail_attach.h webmail_parser.h read_IM_config.h ../../

