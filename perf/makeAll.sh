#!/bin/bash
make clean
make PROGRAM='bchAtlas.out' CXXFLAGS='-Wall'
rm -f *.o
make PROGRAM='bchAtlas_o1.out' CXXFLAGS='-Wall -O1'
rm -f *.o
make PROGRAM='bchAtlas_o2.out' CXXFLAGS='-Wall -O2'
rm -f *.o
make PROGRAM='bchAtlas_o3.out' CXXFLAGS='-Wall -O3'
rm -f *.o
make PROGRAM='bchAtlas_o3fun.out' CXXFLAGS='-Wall -O3 -funroll-loops'

