#define CL_USE_DEPRECATED_OPENCL_2_0_APIS
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <CL/cl.h>

#define N (1<<25)
#define MAXGPU 10
#define MAXK 1024
#define MAXLOG 4096
#define DEVICENUM 3
#define NANO2SECOND 1000000000.0

cl_uint A[N], B[N], C[N], D[N], E[N], F[N], G[N];

int main(int argc, char *argv[]){
	printf("Hello, OpenCL\n");
	cl_int status;

	/* get platform */
	cl_platform_id platform_id;
	cl_uint platform_id_got;
	status = clGetPlatformIDs(1, &platform_id, &platform_id_got);
	assert(status == CL_SUCCESS && platform_id_got == 1);
	printf("%d platform found\n", platform_id_got);

	/* get device */
	cl_device_id GPU[MAXGPU];
	cl_uint GPU_id_got;
	status = clGetDeviceIDs(platform_id, CL_DEVICE_TYPE_GPU, MAXGPU, GPU, &GPU_id_got);
	assert(status == CL_SUCCESS && GPU_id_got >= DEVICENUM);
	printf("There are %d GPU device\n", GPU_id_got);

	/* get context */
	cl_context context = clCreateContext(NULL, DEVICENUM, GPU, NULL, NULL, &status);
	assert(status == CL_SUCCESS);

	/* command queue */
	cl_command_queue commandQueue[DEVICENUM];
	for(int d=0; d<DEVICENUM; d++){
		commandQueue[d] = clCreateCommandQueue(context, GPU[d], CL_QUEUE_PROFILING_ENABLE, &status);
		assert(status == CL_SUCCESS);
	}

	/* Kernel source */
	FILE *kernelfp = fopen("vectorAdd-dependency.cl", "r");
	assert(kernelfp != NULL);
	char kernelBuffer[MAXK];
	const char *constKernelSource = kernelBuffer;
	size_t kernelLength = fread(kernelBuffer, 1, MAXK, kernelfp);
	printf("The size of kernel source is %zu\n", kernelLength);
	cl_program program = clCreateProgramWithSource(context, 1, &constKernelSource, &kernelLength, &status);
	assert(status == CL_SUCCESS);

	/* build program */
	status = clBuildProgram(program, DEVICENUM, GPU, NULL, NULL, NULL);
	if(status != CL_SUCCESS){
		char log[MAXLOG];
		size_t logLength;
		for(int d=0; d<DEVICENUM; d++){
			clGetProgramBuildInfo(program, GPU[d], CL_PROGRAM_BUILD_LOG, MAXLOG, log, &logLength);
			puts(log);
		}
		exit(-1);
	}
	printf("Build Program Completes\n");

	/* create kernel */
	cl_kernel kernel = clCreateKernel(program, "add", &status);
	assert(status == CL_SUCCESS);
	printf("Build kernel completes\n");

	/* vectors */
	for(int i=0; i<N; i++){
		A[i] = D[i] = i;
		B[i] = E[i] = N - i;
	}

	/* create buffer1 */
	cl_mem bufferA = clCreateBuffer(context, CL_MEM_READ_ONLY | CL_MEM_COPY_HOST_PTR,
						N * sizeof(cl_uint), A, &status);
	assert(status == CL_SUCCESS);
	cl_mem bufferB = clCreateBuffer(context, CL_MEM_READ_ONLY | CL_MEM_COPY_HOST_PTR,
						N * sizeof(cl_uint), B, &status);
	assert(status == CL_SUCCESS);
	cl_mem bufferC = clCreateBuffer(context, CL_MEM_READ_WRITE,
						N * sizeof(cl_uint), NULL, &status);
	assert(status == CL_SUCCESS);

	/* create buffer2 */
	cl_mem bufferD = clCreateBuffer(context, CL_MEM_READ_ONLY | CL_MEM_COPY_HOST_PTR,
						N * sizeof(cl_uint), D, &status);
	assert(status == CL_SUCCESS);
	cl_mem bufferE = clCreateBuffer(context, CL_MEM_READ_ONLY | CL_MEM_COPY_HOST_PTR,
						N * sizeof(cl_uint), E, &status);
	assert(status == CL_SUCCESS);
	cl_mem bufferF = clCreateBuffer(context, CL_MEM_READ_WRITE,
						N * sizeof(cl_uint), NULL, &status);
	assert(status == CL_SUCCESS);

	/* create buffer3 */
	cl_mem bufferG = clCreateBuffer(context, CL_MEM_WRITE_ONLY | CL_MEM_USE_HOST_PTR,
						N * sizeof(cl_uint), G, &status);
	assert(status == CL_SUCCESS);
	printf("Build buffers completes\n");	

	/* shape */
	size_t globalThreads[] = {(size_t)N};
	size_t localThreads[] = {256};

	/* ABC */
	cl_event events[3];
	status = clSetKernelArg(kernel, 0, sizeof(cl_mem), (void *)&bufferA);
	assert(status == CL_SUCCESS);
	status = clSetKernelArg(kernel, 1, sizeof(cl_mem), (void *)&bufferB);
	assert(status == CL_SUCCESS);
	status = clSetKernelArg(kernel, 2, sizeof(cl_mem), (void *)&bufferC);
	assert(status == CL_SUCCESS);
	status = clEnqueueNDRangeKernel(commandQueue[0], kernel, 1, NULL,
				globalThreads, localThreads, 0, NULL, &events[0]);
	assert(status == CL_SUCCESS);

	/* DEF */
	status = clSetKernelArg(kernel, 0, sizeof(cl_mem), (void *)&bufferD);
	assert(status == CL_SUCCESS);
	status = clSetKernelArg(kernel, 1, sizeof(cl_mem), (void *)&bufferE);
	assert(status == CL_SUCCESS);
	status = clSetKernelArg(kernel, 2, sizeof(cl_mem), (void *)&bufferF);
	assert(status == CL_SUCCESS);
	status = clEnqueueNDRangeKernel(commandQueue[1], kernel, 1, NULL,
				globalThreads, localThreads, 0, NULL, &events[1]);
	assert(status == CL_SUCCESS);

	/* CFG */
	status = clSetKernelArg(kernel, 0, sizeof(cl_mem), (void *)&bufferC);
	assert(status == CL_SUCCESS);
	status = clSetKernelArg(kernel, 1, sizeof(cl_mem), (void *)&bufferF);
	assert(status == CL_SUCCESS);
	status = clSetKernelArg(kernel, 2, sizeof(cl_mem), (void *)&bufferG);
	assert(status == CL_SUCCESS);
	status = clEnqueueNDRangeKernel(commandQueue[2], kernel, 1, NULL,
				globalThreads, localThreads, 2, events, &events[2]);
	assert(status == CL_SUCCESS);
	
	/* wait for events */
	status = clWaitForEvents(1, &events[2]);
	assert(status == CL_SUCCESS);
	printf("All three kernels complete/\n");

	/* get result */
	status = clEnqueueReadBuffer(commandQueue[2], bufferG, CL_TRUE,
					0, N*sizeof(cl_uint), G, 0, NULL, NULL);

	/* get base */
	cl_long base;
	status = clGetEventProfilingInfo(events[0], CL_PROFILING_COMMAND_QUEUED,
					sizeof(cl_ulong), &base, NULL);
	assert(status == CL_SUCCESS);

	/* get time */
	for(int d=0; d<DEVICENUM; d++){
		cl_long timeEnterQueue, timeSubmit, timeStart, timeEnd;
		status = clGetEventProfilingInfo(events[d], CL_PROFILING_COMMAND_QUEUED,
					sizeof(cl_ulong), &timeEnterQueue, NULL);
		assert(status == CL_SUCCESS);		
		status = clGetEventProfilingInfo(events[d], CL_PROFILING_COMMAND_SUBMIT,
					sizeof(cl_ulong), &timeSubmit, NULL);
		assert(status == CL_SUCCESS);		
		status = clGetEventProfilingInfo(events[d], CL_PROFILING_COMMAND_START,
					sizeof(cl_ulong), &timeStart, NULL);
		assert(status == CL_SUCCESS);		
		status = clGetEventProfilingInfo(events[d], CL_PROFILING_COMMAND_END,
					sizeof(cl_ulong), &timeEnd, NULL);
		assert(status == CL_SUCCESS);

		/* print time */
		printf("\nDevice %d\n", d);
		printf("kernel entered queue at %f\n", (timeEnterQueue - base) / NANO2SECOND);
		printf("kernel submitted to device at %f\n", (timeSubmit - base) / NANO2SECOND);
		printf("kernel started at %f\n", (timeStart - base) / NANO2SECOND);
		printf("kernel ended at %f\n", (timeEnd - base) / NANO2SECOND);
		printf("kernel queued time %f seconds\n", (timeSubmit - timeEnterQueue) / NANO2SECOND);
		printf("kernel submission time %f seconds\n", (timeStart - timeSubmit) / NANO2SECOND);
		printf("kernel execution time %f seconds\n", (timeEnd - timeStart) / NANO2SECOND);
	}

	/* Check and Free */
	for(int i=0; i<N; i++)
		assert(G[i] == A[i]+B[i]+D[i]+E[i]);
	printf("Answer correct\n");

	clReleaseContext(context);
	for(int d=0; d<DEVICENUM; d++)
		clReleaseCommandQueue(commandQueue[d]);
	clReleaseProgram(program);
	clReleaseKernel(kernel);
	clReleaseMemObject(bufferA);
	clReleaseMemObject(bufferB);
	clReleaseMemObject(bufferC);
	clReleaseMemObject(bufferD);
	clReleaseMemObject(bufferE);
	clReleaseMemObject(bufferF);
	clReleaseMemObject(bufferG);
	return 0;
}
