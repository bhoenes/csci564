#!/bin/bash
echo "No Optimization"
./bchAtlas.out
echo "O1"
./bchAtlas_o1.out
echo "O2"
./bchAtlas_o2.out
echo "O3"
./bchAtlas_o3.out
echo "O3 -funroll-loops"
./bchAtlas_o3fun.out

