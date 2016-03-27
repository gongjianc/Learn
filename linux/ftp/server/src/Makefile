ELF:=../bin/server
OBJ:=server.c cmd.c child.c deliver_fd.c pool_n.c
$(ELF):$(OBJ)
	 gcc $^ -o $@ -I../include

.PHONY:clean
clean:
	rm -rf ../bin/server
