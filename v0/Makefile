CFLAGS = -Wall -g

mc0: mc0.o
	gcc $(CFLAGS) mc0.o -lm -o mc0
mc0.o: mc0.c
	gcc $(CFLAGS) -c mc0.c
clean:
	rm -f *.o mc0 
