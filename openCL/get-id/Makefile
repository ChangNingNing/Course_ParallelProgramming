CC = gcc
CFLAGS = -std=c99
INCDIRS = -I"C:\Program Files\NVIDIA GPU Computing Toolkit\CUDA\v8.0\include"
LIBS = "C:\Program Files\NVIDIA Corporation\OpenCL\OpenCL.dll"

all: get-global-id-cl get-global-local-id-cl

%-cl: %.c
	$(CC) $(CFLAGS) $(INCDIRS) $< $(LIBS) -o $@

