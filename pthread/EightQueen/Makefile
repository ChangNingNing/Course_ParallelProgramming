all: queen.c queen_pthread.c
	gcc -std=c99 -O2 queen.c -o queen
	gcc -std=c99 -O2 queen_pthread.c -lpthread -o queen_pthread
	gcc -std=c99 -O2 queen_pthread_mutex.c -lpthread -o queen_pthread_mutex
