CFLAGS = -Wall -g

mc1: mc1.o
	gcc $(CFLAGS) mc1.o -lm -o mc1
mc1.o: mc1.c
	gcc $(CFLAGS) -c mc1.c
clean:
	rm -f *.o mc1 
