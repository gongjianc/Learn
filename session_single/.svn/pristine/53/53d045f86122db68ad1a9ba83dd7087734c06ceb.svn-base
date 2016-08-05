JAVAFLAG1 =  -I /usr/java/jdk1.8.0_05/include -I /usr/java/jdk1.8.0_05/include/linux 
JAVAFLAG2 =  -L /usr/java/jdk1.8.0_05/lib/ -L /usr/java/jdk1.8.0_05/jre/lib/amd64/server/ -L /root/libchar/libchardet-0.0.4/src/.libs
JAVAFLAG3 = -ljvm
FTPLD = -Wall -lm -lpcap -lzlog -luuid  -lpcre -pthread -ldl -lsqlite3 -lpython2.6 -ljansson -lgmime-2.6 -lchardet
GMIME_LIB=-I /usr/local/include/gmime-2.6/ -I/usr/local/include/glib-2.0 -I/usr/local/lib/glib-2.0/include -L/usr/local/lib/ -lgthread-2.0 -lglib-2.0  -lgmime-2.6 -I /root/libchar/libchardet-0.0.4/src

CC = gcc
TARGET = all
all:extractmain.o fileparse.o  ftp_extract.o   gmime_smtp.o  smtp_dissect.o  smtpfunc.o  update_json.o 
gmime_smtp.o:
	gcc -c gmime_smtp.c -Wall  -g $(JAVAFLAG1) $(JAVAFLAG2) $(JAVAFLAG3) $(INCLUDEGLAG) $(GMIME_LIB)
beap_hash.o:
	gcc -c -Wall beap_hash.c -g $(JAVAFLAG1) $(JAVAFLAG2) $(JAVAFLAG3) $(INCLUDEGLAG) $(GMIME_LIB)
smtp_dissect.o: 
	gcc -c -Wall smtp_dissect.c -g $(JAVAFLAG1) $(JAVAFLAG2) $(JAVAFLAG3) $(INCLUDEGLAG) $(GMIME_LIB)
smtpfunc.o:
	gcc -c -Wall smtpfunc.c -g $(JAVAFLAG1) $(JAVAFLAG2) $(JAVAFLAG3) $(INCLUDEGLAG) $(GMIME_LIB)
fileparse.o: 
	gcc -c -Wall fileparse.c -g $(JAVAFLAG1) $(JAVAFLAG2) $(JAVAFLAG3) $(INCLUDEGLAG) $(GMIME_LIB)
ftp_extract.o:
	gcc -c -Wall ftp_extract.c -g $(JAVAFLAG1) $(JAVAFLAG2) $(JAVAFLAG3) $(INCLUDEGLAG) $(GMIME_LIB)
update_json.o: 
	gcc -c -Wall update_json.c -g $(JAVAFLAG1) $(JAVAFLAG2) $(JAVAFLAG3) $(INCLUDEGLAG) $(GMIME_LIB)
extractmain.o: 
	gcc -c -Wall extractmain.c -g $(JAVAFLAG1) $(JAVAFLAG2) $(JAVAFLAG3) $(INCLUDEGLAG) $(GMIME_LIB)
clean:
	rm -f runextract *.o *.tmp core.*
