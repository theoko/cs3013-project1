CFLAGS = -Wall -g

ls1: ls1.o
	gcc $(CFLAGS) ls1.o -lm -o ls1
ls1.o: ls1.c
	gcc $(CFLAGS) -c ls1.c
clean:
	rm -f *.o ls1 
