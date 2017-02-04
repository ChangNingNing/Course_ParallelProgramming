#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#define MAXN 20

int n;

int ok(int position[], int next, int test){
	for(int i=0; i<next; i++){
		if(position[i] == test || abs(test-position[i]) == next-i)
			return 0;
	}
	return 1;
}

int queen(int position[], int next){
	if(next >= n) return 1;
	int sum = 0;
	for(int i=0; i<n; i++){
		if(ok(position, next, i)){
			position[next] = i;
			sum += queen(position, next + 1);
		}
	}
	return sum;
}

void *goQueen(void *pos){
	int *position = (int *)pos;
	int *num = (int *)malloc(sizeof(int));
	*num = queen(position, 1);
	pthread_exit(num);
}

int main(int argc, char *argv[]){
	assert(argc == 2);
	n = atoi(argv[1]);
	assert(n <= MAXN);

	pthread_t threads[MAXN];
	pthread_attr_t attr;
	pthread_attr_init(&attr);
	pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);

	for(int i=0; i<n; i++){
		int *position = (int *)calloc(n, sizeof(int));
		assert(position != NULL);
		position[0] = i;
		int error = pthread_create(&threads[i], &attr, goQueen, (void *)position);
		assert(error == 0);
	}
	
	int numSolution = 0;
	for(int i=0; i<n; i++){
		int *num;
		pthread_join(threads[i], (void **)&num);
		numSolution += *num;
	}
	printf("%d\n", numSolution);
	pthread_attr_destroy(&attr);
	pthread_exit(NULL);
	return 0;
}
