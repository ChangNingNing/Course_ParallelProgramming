CC = gcc
CFLAGS = -std=c99
INCDIRS = -I"C:\Program Files\NVIDIA GPU Computing Toolkit\CUDA\v8.0\include"
LIBS = "C:\Program Files\NVIDIA Corporation\OpenCL\OpenCL.dll"

all: vectorAdd-cl vectorAdd-nofetchC-cl

%-cl: %.c
	$(CC) $(CFLAGS) $(INCDIRS) $< $(LIBS) -o $@

