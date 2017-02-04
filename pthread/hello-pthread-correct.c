#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#define NUM_THREADS 10

void *printHello(void *thread_id){
	int tid = *((int *)thread_id);
	printf("printHello: tid = %d\n", tid);
	pthread_exit(NULL);
}

int main(int argc, char *argv[]){
	pthread_t threads[NUM_THREADS];
	int threadIndex[NUM_THREADS];
	for(int t=0; t<NUM_THREADS; t++){
		printf("main: create thread %d\n", t);
		threadIndex[t] = t;
		int rc = pthread_create(&threads[t], NULL, printHello, (void *)(threadIndex + t));
		
		if(rc){
			printf("main: error code %d\n", rc);
			exit(-1);
		}
	}
	for(int t=0; t<NUM_THREADS; t++)
		pthread_join(threads[t], NULL);
	pthread_exit(NULL);
	return 0;
}
