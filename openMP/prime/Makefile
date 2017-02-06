all: prime.c
	gcc -std=c99 -O2 prime.c -o prime_uni
	gcc -std=c99 -O2 -fopenmp prime.c -o prime_omp
	gcc -std=c99 -O2 -fopenmp prime_inner.c -o prime_inner
	gcc -std=c99 -O2 -fopenmp prime_nowait.c -o prime_nowait

