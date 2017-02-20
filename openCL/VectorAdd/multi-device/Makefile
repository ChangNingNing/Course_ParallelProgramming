CC = gcc
CFLAGS = -std=c99
INCDIRS = -I"C:\Program Files\NVIDIA GPU Computing Toolkit\CUDA\v8.0\include"
LIBS = "C:\Program Files\NVIDIA Corporation\OpenCL\OpenCL.dll"

all: vectorAdd-dependency-cl vectorAdd-dependency-correct-cl

%-cl: %.c
	$(CC) -fopenmp -L/usr/local/cuda-8.0/lib64 $(CFLAGS) -O2 $< -lOpenCL -o $@

clean:
	rm *-cl

