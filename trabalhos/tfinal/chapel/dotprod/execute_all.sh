#!/bin/bash

#./run_tests_alt1.sh 1000000 1000
#./run_tests_alt1.sh 1000000 2000

#./run_tests_alt2.sh 1000000 1000
#./run_tests_alt2.sh 1000000 2000

#./run_tests_ntasks.sh 2 1000000 1000
#./run_tests_ntasks.sh 2 1000000 2000
#./run_tests_ntasks.sh 4 1000000 1000
#./run_tests_ntasks.sh 4 1000000 2000
#./run_tests_ntasks.sh 8 1000000 1000
#./run_tests_ntasks.sh 8 1000000 2000

./run_tests_alt1.sh 10000000 1000
./run_tests_alt1.sh 10000000 2000

./run_tests_alt2.sh 10000000 1000
./run_tests_alt2.sh 10000000 2000

./run_tests_ntasks.sh 2 10000000 1000
./run_tests_ntasks.sh 2 10000000 2000
./run_tests_ntasks.sh 4 10000000 1000
./run_tests_ntasks.sh 4 10000000 2000
./run_tests_ntasks.sh 8 10000000 1000
./run_tests_ntasks.sh 8 10000000 2000
