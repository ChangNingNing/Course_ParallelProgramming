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
		printf("Platform name %s\n", buffer);

		// Platform Vendor
		clGetPlatformInfo(platform_id[i], CL_PLATFORM_VENDOR, MAXB, buffer, &length);
		printf("Platform vendor %s\n", buffer);

		// Platform Version
		clGetPlatformInfo(platform_id[i], CL_PLATFORM_VERSION, MAXB, buffer, &length);
		printf("OpenCL version %s\n", buffer);

		// Platform Profile
		clGetPlatformInfo(platform_id[i], CL_PLATFORM_PROFILE, MAXB, buffer, &length);
		printf("Platform profile %s\n", buffer);

		// Platform Extensions
		clGetPlatformInfo(platform_id[i], CL_PLATFORM_EXTENSIONS, MAXB, buffer, &length);
		printf("Platform Extensions %s\n", buffer);

		cl_device_id devices[MAXDEVICE];
		cl_uint device_id_got;
		clGetDeviceIDs(platform_id[i], CL_DEVICE_TYPE_ALL, MAXDEVICE, devices, &device_id_got);
		printf("Threr are %d devices\n", device_id_got);
		clGetDeviceIDs(platform_id[i], CL_DEVICE_TYPE_CPU, MAXDEVICE, devices, &device_id_got);
		printf("Threr are %d CPU devices\n", device_id_got);
		clGetDeviceIDs(platform_id[i], CL_DEVICE_TYPE_GPU, MAXDEVICE, devices, &device_id_got);
		printf("Threr are %d GPU devices\n", device_id_got);
		clGetDeviceIDs(platform_id[i], CL_DEVICE_TYPE_ACCELERATOR, MAXDEVICE, devices, &device_id_got);
		printf("Threr are %d accelerator devices\n", device_id_got);
		clGetDeviceIDs(platform_id[i], CL_DEVICE_TYPE_DEFAULT, MAXDEVICE, devices, &device_id_got);
		printf("Threr are %d default devices\n", device_id_got);
	}
	return 0;
}
