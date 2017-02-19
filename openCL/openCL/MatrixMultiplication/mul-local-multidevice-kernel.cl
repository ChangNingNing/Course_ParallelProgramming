#define N 1024
#define Blk 64
#define BSIDE (N / Blk)
#define DEVICENUM 2

__kernel void mul(__global int A[N/DEVICENUM][N], __global int B[N][N], __global int C[N/DEVICENUM][N]){
	int globalRow = get_global_id(0);
	int globalCol = get_global_id(1);
	int localRow = get_local_id(0);
	int localCol = get_local_id(1);
	__local int ALocal[BSIDE][BSIDE];
	__local int BLocal[BSIDE][BSIDE];

	int sum = 0;
	for(int block = 0; block < Blk; block++){
		ALocal[localRow][localCol] = A[globalRow][block*BSIDE+localCol];
		BLocal[localRow][localCol] = B[block*BSIDE+localRow][globalCol];
		barrier(CLK_LOCAL_MEM_FENCE);
		/* inner */
		for(int k=0; k<BSIDE; k++)
			sum += ALocal[localRow][k] * BLocal[k][localCol];
		barrier(CLK_LOCAL_MEM_FENCE);
	}
	C[globalRow][globalCol] = sum;
}
