CC = gcc
CFLAGS = -std=c99
INCDIRS = -I"C:\Program Files\NVIDIA GPU Computing Toolkit\CUDA\v8.0\include"
LIBS = "C:\Program Files\NVIDIA Corporation\OpenCL\OpenCL.dll"
LIBOPENCL = "/usr/local"

all: matrixMul-cl matrixMul-time-cl matrixMul-time-copy-cl matrixMul-time-use-cl matrixMul-time-copy-local-cl matrixMul-time-copy-local-multidevice-cl

%-cl: %.c
	$(CC) -L/usr/local/cuda-8.0/lib64 $(CFLAGS) -O2 $< -lOpenCL -o $@

