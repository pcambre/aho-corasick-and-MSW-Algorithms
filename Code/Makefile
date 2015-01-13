OBJS =  MSWNode.o MSWGraph.o AhoCorasickNode.o AhoCorasickGraph.o QueueNode.o Queue.o Main.o
CC = g++
CFLAGS = -Wall -O3 -c
LFLAGS = -Wall
STACKFLAGS = -Wl,-stack_size,0x10000000,-stack_addr,0xc0000000

pdg_default_stack : $(OBJS)
	$(CC) $(LFLAGS) $(OBJS) -o pdg

pdg : $(OBJS)
	$(CC) $(LFLAGS) $(OBJS) -o pdg $(STACKFLAGS)

AhoCorasickNode.o : AhoCorasickNode.h
	$(CC) $(CFLAGS) AhoCorasickNode.cpp
	
MSWNode.o : MSWNode.h types.h Const.h
	$(CC) $(CFLAGS) MSWNode.cpp

MSWGraph.o : MSWGraph.h
	$(CC) $(CFLAGS) MSWGraph.cpp
	
AhoCorasickGraph.o : AhoCorasickGraph.h types.h Queue.h Queue.h
	$(CC) $(CFLAGS) AhoCorasickGraph.cpp
	
QueueNode.o : QueueNode.h AhoCorasickNode.h
	$(CC) $(CFLAGS) QueueNode.cpp
	
Queue.o : Queue.h QueueNode.h
	$(CC) $(CFLAGS) Queue.cpp
	
Main.o : AhoCorasickGraph.h MSWGraph.h Const.h
	$(CC) $(CFLAGS) Main.cpp

clean:
	\rm *.o pdg