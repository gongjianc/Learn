INC_DIR:=./include ./include/net ./include/threadpool ./include/timer
SRC_DIR:=./src ./src/net ./src/threadpool ./src/timer
SRCS:=$(wildcard ./src/*.cc) $(wildcard ./src/net/*.cc) $(wildcard ./src/threadpool/*.cc) $(wildcard ./src/timer/*.cc)
OBJS:=$(patsubst %.cc, %.o, $(SRCS))
LIBS:=-lpthread
CXX:=g++
CXXFLAGS:= -w -g -std=c++11 $(addprefix -I, $(INC_DIR)) $(LIBS) -Wno-deprecated

EXE:=./bin/main

$(EXE):$(OBJS)
	$(CXX) -o $(EXE) $(SRCS) $(CXXFLAGS)

clean:
	rm -rf $(EXE)
	rm -rf $(OBJS)
