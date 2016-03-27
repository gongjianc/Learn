INC_DIR:=../include
SRC_DIR:=../src
CC:=g++
CPPFLAGS:=-std=c++11 -g
LIBS:=-lpthread
INC_FILE:=$(addprefix -I , $(INC_DIR))
SRC_FILE:=$(wildcard ../src/*.cc)
TARGET:=../bin/server

$(TARGET):$(SRC_FILE)
	$(CC) -o $@ $^ $(CPPFLAGS) $(INC_FILE) 

clean:
	rm -rf $(TARGET)
