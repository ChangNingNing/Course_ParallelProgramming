#!/bin/bash
echo stack_uni
time ./assign_stack_uni.exe
echo stack_omp
time ./assign_stack_omp.exe
echo private_omp
time ./assign_private_omp.exe
echo heap_omp
time ./assign_heap_omp.exe
