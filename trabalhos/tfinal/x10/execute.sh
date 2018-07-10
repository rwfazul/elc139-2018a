#!/bin/bash

# args = <nact> <wsize> <repeat>

# C++ backend
# ./compile_c++.sh
# wsize = 1.000.000
./run_tests_c++.sh 2 1000000 1000
./run_tests_c++.sh 2 1000000 2000
./run_tests_c++.sh 4 1000000 1000
./run_tests_c++.sh 4 1000000 2000
./run_tests_c++.sh 8 1000000 1000
./run_tests_c++.sh 8 1000000 2000
# wsize = 10.000.000
./run_tests_c++.sh 2 10000000 1000
./run_tests_c++.sh 2 10000000 2000
./run_tests_c++.sh 4 10000000 1000
./run_tests_c++.sh 4 10000000 2000
./run_tests_c++.sh 8 10000000 1000
./run_tests_c++.sh 8 10000000 2000

# Java backend
# ./compile_java.sh
# wsize = 1.000.000
./run_tests_java.sh 2 1000000 1000
./run_tests_java.sh 2 1000000 2000
./run_tests_java.sh 4 1000000 1000
./run_tests_java.sh 4 1000000 2000
./run_tests_java.sh 8 1000000 1000
./run_tests_java.sh 8 1000000 2000
# wsize = 10.000.000
./run_tests_java.sh 2 10000000 1000
./run_tests_java.sh 2 10000000 2000
./run_tests_java.sh 4 10000000 1000
./run_tests_java.sh 4 10000000 2000
./run_tests_java.sh 8 10000000 1000
./run_tests_java.sh 8 10000000 2000
