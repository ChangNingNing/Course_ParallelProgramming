#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#define MAXN 20

int n;
int numSolution;
pthread_mutex_t numSolutionLock;

int ok(int position[], int next, int test){
	for(int i=0; i<next; i++){
		if(position[i] == test || abs(test-position[i]) == next-i)
			return 0;
	}
	return 1;
}

void queen(int position[], int next){
	if(next >= n){
		pthread_mutex_lock(&numSolutionLock);
		numSolution++;
		pthread_mutex_unlock(&numSolutionLock);
		return;
	}
	for(int i=0; i<n; i++){
		if(ok(position, next, i)){
			position[next] = i;
			queen(position, next + 1);
		}
	}
	return;
}

void *goQueen(void *pos){
	int *position = (int *)pos;
	queen(position, 1);
	pthread_exit(NULL);
}

int main(int argc, char *argv[]){
	assert(argc == 2);
	n = atoi(argv[1]);
	assert(n <= MAXN);

	pthread_t threads[MAXN];
	pthread_attr_t attr;
	pthread_attr_init(&attr);
	pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);
	pthread_mutex_init(&numSolutionLock, NULL);

	numSolution = 0;
	for(int i=0; i<n; i++){
		int *position = (int *)calloc(n, sizeof(int));
		assert(position != NULL);
		position[0] = i;
		int error = pthread_create(&threads[i], &attr, goQueen, (void *)position);
		assert(error == 0);
	}
	
	for(int i=0; i<n; i++)
		pthread_join(threads[i], NULL);
	printf("%d\n", numSolution);
	pthread_attr_destroy(&attr);
	pthread_mutex_destroy(&numSolutionLock);
	pthread_exit(NULL);
	return 0;
}
