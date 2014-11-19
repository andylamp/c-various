CC = cc
CFLAGS = -Wall -std=c99 -D_GNU_SOURCE

#DEBUG ?= 0
#ifeq ($(DEBUG),1)
#	CFLAGS += -g
#else
	#CFLAGS +=
#endif

all: p1

p1: p1.c
	$(CC) $(CFLAGS) p1.c -o p1

clean:
	rm -rf *~ ~* *.c~ *.o~ *.o *.core p1
