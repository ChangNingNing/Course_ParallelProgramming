#!/bin/bash
for((i=5; i<=15; i+=1))
do
	echo $i
	echo queen
	time ./queen.exe $i
	echo queen_omp
	time ./queen_omp.exe $i
done
