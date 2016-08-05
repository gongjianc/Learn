CFLAG=-lpcre -ljansson -lpcap -luuid -pthread -lzlog
CC=gcc
TARGET=extract

all:${TARGET}

extract: *.c
	$(CC)  -c  $^ $(CFLAG)

clean:
	rm -f *.o

#	ls | grep  "-" | xargs rm -rf 
