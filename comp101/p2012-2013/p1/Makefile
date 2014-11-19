CC = cc
CFLAGS = -O3 -Wall -D_GNU_SOURCE -D_POSIX_C_SOURCE=200112L

all: p1 p2

p1: p1.c
	$(CC) $(CFLAGS) p1.c -o p1

p2: p2.c
	$(CC) $(CFLAGS) p2.c -o p2

clean:
	rm -rf p1 p2 *.swp *.swp~ *.bak *.bak~ *.dSYM ~* *.c~ *.h~ *.o *.o~ *.core~ *.core
