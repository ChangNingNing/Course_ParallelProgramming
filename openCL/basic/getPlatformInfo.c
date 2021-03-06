#include <stdio.h>
#include <assert.h>
#include <CL/cl.h>
#define MAXB 256
#define MAXPLATFORM 5

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
//		buffer[length] = '\0';
		printf("Platform name %s\n", buffer);

		// Platform Vendor
		clGetPlatformInfo(platform_id[i], CL_PLATFORM_VENDOR, MAXB, buffer, &length);
//		buffer[length] = '\0';
		printf("Platform vendor %s\n", buffer);

		// Platform Version
		clGetPlatformInfo(platform_id[i], CL_PLATFORM_VERSION, MAXB, buffer, &length);
//		buffer[length] = '\0';
		printf("OpenCL version %s\n", buffer);

		// Platform Profile
		clGetPlatformInfo(platform_id[i], CL_PLATFORM_PROFILE, MAXB, buffer, &length);
//		buffer[length] = '\0';
		printf("Platform profile %s\n", buffer);

		// Platform Extensions
		clGetPlatformInfo(platform_id[i], CL_PLATFORM_EXTENSIONS, MAXB, buffer, &length);
//		buffer[length] = '\0';
		printf("Platform Extensions %s\n", buffer);
	}
	return 0;
}
