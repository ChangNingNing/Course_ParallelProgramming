#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
#include <assert.h>
#define MAXN 20

int ok(int position[], int next, int test){
	for(int i=0; i<next; i++){
		if(position[i] == test || abs(test-position[i]) == next-i)
			return 0;
	}
	return 1;
}

int queen(int position[], int next, int n){
	if(next >= n) return 1;
	int sum = 0;
	for(int i=0; i<n; i++){
		if(ok(position, next, i)){
			position[next] = i;
			sum += queen(position, next+1, n);
		}
	}
	return sum;
}

int main(int argc, char *argv[]){
	assert(argc == 2);
	int n = atoi(argv[1]);
	assert(n <= MAXN);
	int position[MAXN];
	int numSolution = 0;
#pragma omp parallel for private (position) reduction(+ : numSolution)
	for(int i=0; i<n; i++){
		position[0] = i;
		numSolution += queen(position, 1, n);
	}
	printf("total # of solution = %d\n", numSolution);
	return 0;
}
