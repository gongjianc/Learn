SOURSE:=$(wildcard *.c);
OBJS:=$(patsubst %.c,%.o,$SOURSE);
ELF:=main

main:$(OBJS)
	gcc -o $(ELF) $(OBJS)


.PNNOY:clean
clean:
	rm -f $(ELF)
