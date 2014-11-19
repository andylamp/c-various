CC = cc
CFLAGS = -Wall -g -lm -D_GNU_SOURCE

all: p1 p2 p3
 
p1: p1.c
		$(CC) $(CFLAGS) p1.c -o fancyPhoneGen

p2: p2.c
		$(CC) $(CFLAGS) p2.c -o runRabbit

p3: p3.c
		$(CC) $(CFLAGS) p3.c -o toolBoxFTW

clean: 
	rm -rf *.~out *.out *.core *.~core ~* *.~c *.~o *.o
