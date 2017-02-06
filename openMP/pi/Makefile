all: pi_critical.c
	gcc -std=c99 -O2 pi_critical.c -o pi_uni
	gcc -std=c99 -O2 -fopenmp pi_critical.c -o pi_critical
	gcc -std=c99 -O2 -fopenmp pi_reduction.c -o pi_reduction
