#!/bin/sh
make clean
make
cp extractmain.o fileparse.o  ftp_extract.o   gmime_smtp.o  smtp_dissect.o  smtpfunc.o  update_json.o extractmain.h fileparse.h ftp_extract.h gmime_smtp.h smtp_dissect.h  smtpfunc.h  update_json.h  ../../
