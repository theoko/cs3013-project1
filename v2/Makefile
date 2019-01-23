CFLAGS = -Wall -g

all: mc2

mc2: mc2.o
	gcc $(CFLAGS) mc2.o -lm -o mc2
mc2.o: mc2.c
	gcc $(CFLAGS) -c mc2.c
clean:
	rm -f *.o mc2
