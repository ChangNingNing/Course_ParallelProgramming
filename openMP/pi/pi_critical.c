#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
#define N 100000000

int main(){
	double x, area = 0;
	#pragma omp parallel for private(x)
	for(int i=0; i<N; i++){
		x = ((double)i + 0.5) / N;
		#pragma omp critical
		area += 4.0 / (1.0 + x * x);
	}
	double pi = area / N;
	printf("pi = %llf\n", pi);
	return 0;
}
