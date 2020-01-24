CC=gcc
CFLAGS=-g
TARGET: test.exe

OBJS=graph.o	\
     topologies.o \
     net.o
     
test.exe: test_topology.o ${OBJS}
	${CC} ${CFLAGS} test_topology.o ${OBJS} -o test.exe

test_topology.o: test_topology.c
	${CC} ${CFLAGS} -c test_topology.c -o test_topology.o

graph.o: graph.c
	${CC} ${CFLAGS} -c -I intrusive_list -I . graph.c -o graph.o

topologies.o: topologies.c
	${CC} ${CFLAGS} -c -I intrusive_list -I . topologies.c -o topologies.o

net.o: net.c
	${CC} ${CFLAGS} -c -I intrusive_list -I . net.c -o net.o

clean:
	rm *o
	rm *exe
