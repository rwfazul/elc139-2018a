#!/bin/bash

# args = <wsize> <repeat>

# make

# wsize = 1.000.000
./run_tests_sequential.sh 1000000 1000
./run_tests_sequential.sh 1000000 2000

# wsize = 10.000.000
./run_tests_sequential.sh 10000000 1000
./run_tests_sequential.sh 10000000 2000
