#!/bin/bash
for((i=1; i<5; i+=1))
do
	echo policy = $i
	./runtime 4 16 $i 1
done
