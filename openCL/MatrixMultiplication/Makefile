CC = gcc
CFLAGS = -std=c99
INCDIRS = -I"C:\Program Files\NVIDIA GPU Computing Toolkit\CUDA\v8.0\include"
LIBS = "C:\Program Files\NVIDIA Corporation\OpenCL\OpenCL.dll"

all: matrixMul-cl matrixMul-time-cl matrixMul-time-copy-cl matrixMul-time-use-cl matrixMul-time-copy-local-cl

%-cl: %.c
	$(CC) $(CFLAGS) $(INCDIRS) $< $(LIBS) -o $@

