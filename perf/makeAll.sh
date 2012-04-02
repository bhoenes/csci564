#!/bin/bash
make clean
make PROGRAM='bchAtlas.out' CXXFLAGS='-g -Wall'
rm -f *.o
make PROGRAM='bchAtlas_o1.out' CXXFLAGS='-g -Wall -O1'
rm -f *.o
make PROGRAM='bchAtlas_o2.out' CXXFLAGS='-g -Wall -O2'
rm -f *.o
make PROGRAM='bchAtlas_o3.out' CXXFLAGS='-g -Wall -O3'
rm -f *.o
make PROGRAM='bchAtlas_o3fun.out' CXXFLAGS='-g -Wall -O3 -funroll-loops'

