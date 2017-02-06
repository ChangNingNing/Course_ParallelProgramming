all: assign_stack.c
	gcc -std=c99 -O2 assign_stack.c -o assign_stack_uni
	gcc -std=c99 -O2 -fopenmp assign_stack.c -o assign_stack_omp
	gcc -std=c99 -O2 -fopenmp assign_private.c -o assign_private_omp
	gcc -std=c99 -O2 -fopenmp assign_heap.c -o assign_heap_omp
