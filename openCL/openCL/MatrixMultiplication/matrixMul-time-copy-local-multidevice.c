#define CL_USE_DEPRECATED_OPENCL_2_0_APIS

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <CL/cl.h>

#define N 1024
#define Blk 64
#define BSIDE (N / Blk)
#define MAXGPU 10
#define MAXK 1024
#define MAXLOG 4096
#define DEVICENUM 2
#define ITEMPERDEVICE (N * N / DEVICENUM)
#define NANO2SECOND 1000000000.0

cl_uint A[N][N], B[N][N], C[N][N];

int main(int argc, char *argv[]){
	printf("Hello, OpenCL\n");
	cl_int status;
	cl_platform_id platform_id;
	cl_uint platform_id_got;
	status = clGetPlatformIDs(1, &platform_id, &platform_id_got);
	assert(status == CL_SUCCESS && platform_id_got == 1);
	printf("%d platform found\n", platform_id_got);

	cl_device_id GPU[MAXGPU];
	cl_uint GPU_id_got;
	status = clGetDeviceIDs(platform_id, CL_DEVICE_TYPE_GPU, MAXGPU, GPU, &GPU_id_got);
	assert(status == CL_SUCCESS && GPU_id_got >= DEVICENUM);
	printf("There are %d GPU devices\n", GPU_id_got);

	/* get context */
	cl_context context = clCreateContext(NULL, DEVICENUM, GPU, NULL, NULL, &status);
	assert(status == CL_SUCCESS);

	/* get command queue */
	cl_command_queue commandQueue[DEVICENUM];
	for(int device=0; device<DEVICENUM; device++){
		commandQueue[device] = clCreateCommandQueue(context, GPU[device], CL_QUEUE_PROFILING_ENABLE, &status);
		assert(status == CL_SUCCESS);
	}

	/* kernel source */
	FILE *kernelfp = fopen(argv[1], "r");
	assert(kernelfp != NULL);
	char kernelBuffer[MAXK];
	const char *constKernelSource = kernelBuffer;
	size_t kernelLength = fread(kernelBuffer, 1, MAXK, kernelfp);
	printf("The size of kernel source is %zu\n", kernelLength);
	cl_program program =
		clCreateProgramWithSource(context, 1, &constKernelSource, &kernelLength, &status);
	assert(status == CL_SUCCESS);

	/* build program */
	status = clBuildProgram(program, DEVICENUM, GPU, NULL, NULL, NULL);
	if(status != CL_SUCCESS){
		char log[MAXLOG];
		size_t logLength;
		for(int device=0; device<DEVICENUM; device++){
			clGetProgramBuildInfo(program, GPU[device], CL_PROGRAM_BUILD_LOG, MAXLOG, log, &logLength);
			puts(log);
		}
		exit(-1);	
	}
	printf("Build program completes\n");

	/* create kernel */
	cl_kernel kernel = clCreateKernel(program, "mul", &status);
	assert(status == CL_SUCCESS);
	printf("Build kernel completes\n");

	/* vector */
	for(int i=0; i<N; i++)
		for(int j=0; j<N; j++){
			A[i][j] = i + j;
			B[i][j] = i - j;
		}

	/* create buffer */
	cl_mem bufferA[DEVICENUM];
	for(int device=0; device<DEVICENUM; device++){
		bufferA[device] = clCreateBuffer(context, CL_MEM_READ_ONLY | CL_MEM_COPY_HOST_PTR,
			ITEMPERDEVICE * sizeof(cl_uint), ((cl_uint *)A)+device*ITEMPERDEVICE, &status);
		assert(status == CL_SUCCESS);
	}
	cl_mem bufferB = 
		clCreateBuffer(context, CL_MEM_READ_ONLY | CL_MEM_COPY_HOST_PTR,
						N*N * sizeof(cl_uint), B, &status);
	assert(status == CL_SUCCESS);
	cl_mem bufferC[DEVICENUM];
	for(int device=0; device<DEVICENUM; device++){
		bufferC[device] = clCreateBuffer(context, CL_MEM_WRITE_ONLY | CL_MEM_USE_HOST_PTR,
			ITEMPERDEVICE * sizeof(cl_uint), ((cl_uint *)C)+device*ITEMPERDEVICE, &status);
		assert(status == CL_SUCCESS);
	}
	printf("Build buffers completes\n");

	/* NDRange */
	size_t globalThreads[] = {(size_t)(N / DEVICENUM), (size_t)N};
	size_t localThreads[] = {BSIDE, BSIDE};
	cl_event events[DEVICENUM];

	/* set argument */
	for(int device=0; device<DEVICENUM; device++){
		status = clSetKernelArg(kernel, 0, sizeof(cl_mem), (void*)&bufferA[device]);
		assert(status == CL_SUCCESS);
		status = clSetKernelArg(kernel, 1, sizeof(cl_mem), (void*)&bufferB);
		assert(status == CL_SUCCESS);
		status = clSetKernelArg(kernel, 2, sizeof(cl_mem), (void*)&bufferC[device]);
		assert(status == CL_SUCCESS);
		printf("Set kernel arguments completes of device %d\n", device);

		/* start kernel */
		status = clEnqueueNDRangeKernel(commandQueue[device], kernel, 2, NULL,
					globalThreads, localThreads, 0, NULL, &events[device]);
		assert(status == CL_SUCCESS);
		printf("Specify the shape of the domain completes.\n");
	}

	/* wait for event */
	clWaitForEvents(DEVICENUM, events);
	printf("Kernel excution completes.\n");

	/* get base */
	cl_long base;
	status = clGetEventProfilingInfo(events[0], CL_PROFILING_COMMAND_QUEUED, sizeof(cl_ulong), &base, NULL);
	assert(status == CL_SUCCESS);

	/* get time */
	for(int device=0; device<DEVICENUM; device++){
		cl_long timeEnterQueue, timeSubmit, timeStart, timeEnd;
		status = clGetEventProfilingInfo(events[device], CL_PROFILING_COMMAND_QUEUED,
					sizeof(cl_ulong), &timeEnterQueue, NULL);
		assert(status == CL_SUCCESS);
		status = clGetEventProfilingInfo(events[device], CL_PROFILING_COMMAND_SUBMIT,
					sizeof(cl_ulong), &timeSubmit, NULL);
		assert(status == CL_SUCCESS);
		status = clGetEventProfilingInfo(events[device], CL_PROFILING_COMMAND_START,
					sizeof(cl_ulong), &timeStart, NULL);
		assert(status == CL_SUCCESS);
		status = clGetEventProfilingInfo(events[device], CL_PROFILING_COMMAND_END,
					sizeof(cl_ulong), &timeEnd, NULL);
		assert(status == CL_SUCCESS);

		/* print time */
		printf("\ndevice %d\n", device);
		printf("kernel enter queue time at %f\n", (timeEnterQueue - base) / NANO2SECOND);
		printf("kernel submited to device at %f\n", (timeSubmit - base) / NANO2SECOND);
		printf("kernel started at %f\n", (timeStart - base) / NANO2SECOND);
		printf("kernel ended at %f\n", (timeEnd - base) / NANO2SECOND);
		printf("kernel queued time %f seconds\n", (timeSubmit - timeEnterQueue) / NANO2SECOND);
		printf("kernel submission time %f seconds\n", (timeStart - timeSubmit) / NANO2SECOND);
		printf("kernel execution time %f seconds\n", (timeEnd - timeStart) / NANO2SECOND);
	}

	/* get result */
	for(int device=0; device<DEVICENUM; device++){
		status = clEnqueueReadBuffer(commandQueue[device], bufferC[device], CL_TRUE, 0,
					ITEMPERDEVICE*sizeof(cl_uint), ((cl_uint *)C)+device*ITEMPERDEVICE, 0, NULL, NULL);
		assert(status == CL_SUCCESS);
	}
	
	/* check and free */
	for(int i=0; i<N; i++)
		for(int j=0; j<N; j++){
			int sum = 0;
			for(int k=0; k<N; k++)
				sum += A[i][k] * B[k][j];
			assert(C[i][j] == sum);
		}
	printf("Answer correct.\n");

	clReleaseContext(context);
	for(int device=0; device<DEVICENUM; device++){
		clReleaseCommandQueue(commandQueue[device]);
		clReleaseMemObject(bufferA[device]);
		clReleaseMemObject(bufferC[device]);
	}
	clReleaseProgram(program);
	clReleaseKernel(kernel);
	clReleaseMemObject(bufferB);
	return 0;
}
