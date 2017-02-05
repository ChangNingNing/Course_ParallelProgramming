#include <omp.h>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <unistd.h>

int main(int argc, char *argv[]){
	assert(argc == 5);
#ifdef _OPENMP
	omp_set_num_threads(atoi(argv[1]));
#endif
	int n = atoi(argv[2]);
	printf("# of proc = %d\n", omp_get_num_procs());
	printf("# of loop iteration = %d\n", n);

	int policy = atoi(argv[3]);
	int chunk = atoi(argv[4]);
	omp_set_schedule(policy, chunk);

	int elapsedTime = 0;
#pragma omp parallel for firstprivate(elapsedTime) schedule(runtime)
	for(int i=0; i<n; i++){
		sleep(i/2);
		elapsedTime += i/2;
		printf("thread %d i %d elapsed time %d.\n", omp_get_thread_num(), i, elapsedTime);
	}
	return 0;
}
