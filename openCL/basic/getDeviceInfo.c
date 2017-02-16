#include <stdio.h>
#include <assert.h>
#include <CL/cl.h>
#define MAXB 256
#define MAXPLATFORM 5
#define MAXDEVICE 10

int main(int argc, char *argv[]){
	printf("Hello OpenCL\n");
	cl_platform_id platform_id[MAXPLATFORM];
	cl_uint platform_id_got;
	clGetPlatformIDs(MAXPLATFORM, platform_id, &platform_id_got);
	printf("%d platform found\n", platform_id_got);
	
	for(int i=0; i<platform_id_got; i++){
		char buffer[MAXB];
		size_t length;

		// Platform Name
		clGetPlatformInfo(platform_id[i], CL_PLATFORM_NAME, MAXB, buffer, &length);
		buffer[length] = '\0';
		printf("Platform name %s\n", buffer);

		// Platform Vendor
		clGetPlatformInfo(platform_id[i], CL_PLATFORM_VENDOR, MAXB, buffer, &length);
		buffer[length] = '\0';
		printf("Platform vendor %s\n", buffer);

		// Platform Version
		clGetPlatformInfo(platform_id[i], CL_PLATFORM_VERSION, MAXB, buffer, &length);
		buffer[length] = '\0';
		printf("OpenCL version %s\n", buffer);

		// Platform Profile
		clGetPlatformInfo(platform_id[i], CL_PLATFORM_PROFILE, MAXB, buffer, &length);
		buffer[length] = '\0';
		printf("Platform profile %s\n", buffer);

		// Platform Extensions
		clGetPlatformInfo(platform_id[i], CL_PLATFORM_EXTENSIONS, MAXB, buffer, &length);
		buffer[length] = '\0';
		printf("Platform Extensions %s\n", buffer);

		cl_device_id devices[MAXDEVICE];
		cl_uint device_id_got;
		clGetDeviceIDs(platform_id[i], CL_DEVICE_TYPE_ALL, MAXDEVICE, devices, &device_id_got);
		printf("Threr are %d devices\n", device_id_got);
		clGetDeviceIDs(platform_id[i], CL_DEVICE_TYPE_ACCELERATOR, MAXDEVICE, devices, &device_id_got);
		printf("Threr are %d accelerator devices\n", device_id_got);
		clGetDeviceIDs(platform_id[i], CL_DEVICE_TYPE_DEFAULT, MAXDEVICE, devices, &device_id_got);
		printf("Threr are %d default devices\n", device_id_got);
		clGetDeviceIDs(platform_id[i], CL_DEVICE_TYPE_CPU, MAXDEVICE, devices, &device_id_got);
		printf("Threr are %d CPU devices\n", device_id_got);
		clGetDeviceIDs(platform_id[i], CL_DEVICE_TYPE_GPU, MAXDEVICE, devices, &device_id_got);
		printf("Threr are %d GPU devices\n", device_id_got);
		
		for(int j=0; j<device_id_got; j++){
			// Device Name
			clGetDeviceInfo(devices[j], CL_DEVICE_NAME, MAXB, buffer, &length);
			buffer[length] = '\0';
			printf("Device name %s\n", buffer);

			// Device Global Memory Size
			cl_ulong number;
			clGetDeviceInfo(devices[j], CL_DEVICE_GLOBAL_MEM_SIZE, sizeof(cl_ulong), &number, NULL);
			printf("Global memory size %lld\n", (long long)number);

			// Device Local Memory Size
			clGetDeviceInfo(devices[j], CL_DEVICE_LOCAL_MEM_SIZE, sizeof(cl_ulong), &number, NULL);
			printf("Local memory size %lld\n", (long long)number);

			// Device # of compute unit
			clGetDeviceInfo(devices[j], CL_DEVICE_MAX_COMPUTE_UNITS, sizeof(cl_ulong), &number, NULL);
			printf("# of compute units %lld\n", (long long)number);

			// Device # of work group size
			clGetDeviceInfo(devices[j], CL_DEVICE_MAX_WORK_GROUP_SIZE, sizeof(cl_ulong), &number, NULL);
			printf("max # of work items in a work group %lld\n", (long long)number);
		}
	}
	return 0;
}
