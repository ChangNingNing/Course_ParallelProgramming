#!/bin/bash
for((i=5; i<=15; i+=1))
do
	echo $i
	echo queen
	time ./queen.exe $i
	echo queen_pthread
	time ./queen_pthread.exe $i
	echo queen_pthread_mutex
	time ./queen_pthread_mutex.exe $i
done
