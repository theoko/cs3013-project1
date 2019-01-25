CFLAGS = -pthread -Wall -g 

all: mc0 mc1 mc2

mc2: v2/mc2.o
	gcc $(CFLAGS) v2/mc2.o -lm -o v2/mc2
mc2.o: v2/mc2.c
	gcc $(CFLAGS) -c v2/mc2.c
<<<<<<< HEAD

mc1: mc1.o
=======
mc1: v1/mc1.o
>>>>>>> 233957749f037990500b3d8799955d834cdc3f59
	gcc $(CFLAGS) v1/mc1.o -lm -o v1/mc1
mc1.o: v1/mc1.c
	gcc $(CFLAGS) -c v1/mc1.c
<<<<<<< HEAD

mc0: mc0.o
=======
mc0: v0/mc0.o
>>>>>>> 233957749f037990500b3d8799955d834cdc3f59
	gcc $(CFLAGS) v0/mc0.o -lm -o v0/mc0
mc0.o: v0/mc0.c
	gcc $(CFLAGS) -c v0/mc0.c

clean:
	rm -f v0/mc0.o v1/mc1.o v2/mc2.o v0/mc0 v1/mc1 v2/mc2
