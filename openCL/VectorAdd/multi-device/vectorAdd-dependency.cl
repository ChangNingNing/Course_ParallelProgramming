__kernel void add(__global int *A, __global int *B, __global int *C){
	int idx = get_global_id(0);
	for(int i=0; i<4096; i++)
		C[idx] = A[idx] + B[idx] + i;
//	if(idx == 0)
//		printf("idx = %d; %d + %d = %d\n", idx, A[idx], B[idx], C[idx]);
}
