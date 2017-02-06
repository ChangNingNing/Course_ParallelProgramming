#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <omp.h>
#define N 900000000

char notPrime[N] = {0};
int nPrime = 0;

int main(){
	int bound = sqrt(N);
	for(int i=2; i<=bound; i++){
		if(!notPrime[i]){
			#pragma omp parallel for
			for(int j=i*2; j<N; j+=i)
				notPrime[j] = 1;
		}
	}
	#pragma omp parallel for reduction(+ : nPrime)
	for(int i=2; i<N; i++)
		if(!notPrime[i]) nPrime++;

	printf("# of primes is %d\n", nPrime);
	return 0;
}
