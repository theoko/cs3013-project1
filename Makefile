CFLAGS = -pthread -Wall -g 

all: mc0 mc1 mc2

mc2: mc2.o
	gcc $(CFLAGS) v2/mc2.o -lm -o v2/mc2
mc2.o: mc2.c
	gcc $(CFLAGS) -c v2/mc2.c

mc1: mc1.o
	gcc $(CFLAGS) v1/mc1.o -lm -o v1/mc1
mc1.o: mc1.c
	gcc $(CFLAGS) -c v1/mc1.c

mc0: mc0.o
	gcc $(CFLAGS) v0/mc0.o -lm -o v0/mc0
mc0.o: mc0.c
	gcc $(CFLAGS) -c v0/mc0.c

clean:
	rm -f v0/mc0.o v1/mc1.o v2/mc2.o v0/mc0 v1/mc1 v2/mc2
