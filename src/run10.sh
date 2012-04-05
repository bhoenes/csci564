#!/bin/bash
echo "***No Optimization***" > o_.csv
echo "***-O1 Optimization***" > o1.csv
echo "***-O2 Optimization***" > o2.csv
echo "***-03 Optimization***" > o3.csv
echo "***-03 -funroll-loops Optimization***" > o3fun.csv

for (( i=0; i < 10; i++ ))
do
	./bchAtlas.out >> o_.csv
	./bchAtlas_o1.out >> o1.csv
	./bchAtlas_o2.out >> o2.csv
	./bchAtlas_o3.out >> o3.csv
	./bchAtlas_o3fun.out >> o3fun.csv
done
 
