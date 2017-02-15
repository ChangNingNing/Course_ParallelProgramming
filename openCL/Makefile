CC = gcc
CFLAGS = -std=c99
INCDIRS = -I"C:\Program Files\NVIDIA GPU Computing Toolkit\CUDA\v8.0\include"
LIBS = "C:\Program Files\NVIDIA Corporation\OpenCL\OpenCL.dll"

all: getPlatformID-cl getPlatformInfo-cl getDeviceID-cl getDeviceInfo-cl vectorAdd-cl

%-cl: %.c
	$(CC) $(CFLAGS) $(INCDIRS) $< $(LIBS) -o $@


#all:
#	gcc test.c "C:\Windows\SysWOW64\OpenCL.dll" -I"C:\Program Files\NVIDIA GPU Computing Toolkit\CUDA\v8.0\include"
#try:
#	gcc test.c "C:\Program Files\NVIDIA Corporation\OpenCL\OpenCL.dll" -I"C:\Program Files\NVIDIA GPU Computing Toolkit\CUDA\v8.0\include"
