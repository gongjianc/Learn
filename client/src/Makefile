ELF:=../bin/client
OBJ:=client.c pool_n.c
$(ELF):$(OBJ)
	 gcc $^ -o $@ -I../include

.PHONY:clean
clean:
	rm -rf ../bin/client
