SOURCES:=$(wildcard *.c)
OBJS:=$(patsubst %.c,%.o,$(SOURCES))
ELF:=main 
CFLAGS:=-Wall -g
$(ELF):$(OBJS)
	gcc $(CFLAGS) -o $(ELF) $(OBJS) 
main.o:main.c
	gcc $(CFLAGS) -c $< -o $@
fun.o:fun.c
	gcc $(CFLAGS) -c $< -o $@

.PHONY:clean

clean:
	rm -f $(ELF) $(OBJS)
