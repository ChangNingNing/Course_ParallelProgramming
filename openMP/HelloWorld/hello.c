#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
#include <assert.h>

int main(int argc, char *argv[]){
	assert(argc == 2);
	omp_set_num_threads(atoi(argv[1]));
#pragma omp parallel
	{
	printf("Hello, world from thread %d.\n", omp_get_thread_num());
	printf("# of proc = %d\n", omp_get_num_procs());
	printf("# of threads = %d\n", omp_get_num_threads());
	}
	return 0;
}
