#!/bin/bash
echo prime_uni
time ./prime_uni.exe
echo prime_omp
time ./prime_omp.exe
echo prime_inner
time ./prime_inner.exe
echo prime_nowait
time ./prime_nowait.exe
