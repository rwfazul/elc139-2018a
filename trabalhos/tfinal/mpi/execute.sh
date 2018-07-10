#!/bin/bash

# args = <nproc> <wsize> <repeat>

# make

# wsize = 1.000.000
./run_tests_mpi.sh 2 1000000 1000
./run_tests_mpi.sh 2 1000000 2000

./run_tests_mpi.sh 4 1000000 1000
./run_tests_mpi.sh 4 1000000 2000

./run_tests_mpi.sh 8 1000000 1000
./run_tests_mpi.sh 8 1000000 2000

# wsize = 10.000.000
./run_tests_mpi.sh 2 10000000 1000
./run_tests_mpi.sh 2 10000000 2000

./run_tests_mpi.sh 4 10000000 1000
./run_tests_mpi.sh 4 10000000 2000

./run_tests_mpi.sh 8 10000000 1000
./run_tests_mpi.sh 8 10000000 2000
