CC=cc
CXX=CC
CCFLAGS= -g -std=c99 -D_POSIX_C_SOURCE=200809L -Wall -Werror -pthread -lrt
OBJS = candykids.o bbuff.o stats.o

candykids: $(OBJS)
	$(CC) -o candykids $(OBJS) $(CCFLAGS)

candykids.o: candykids.c bbuff.h stats.h
	$(CC) -c $(CCFLAGS) candykids.c

bbuff.o: bbuff.c bbuff.h
	$(CC) -c bbuff.c

stats.o: stats.c stats.h
	$(CC) -c stats.c

clean:
	rm -f core *.o candykids