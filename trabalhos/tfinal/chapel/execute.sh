#!/bin/bash

# ./compile.sh

# wsize = 1.000.000
# alt1 -> args = <wsize> <repeat>
./run_tests_alt1.sh 1000000 1000
./run_tests_alt1.sh 1000000 2000
# alt2 -> args = <wsize> <repeat>
./run_tests_alt2.sh 1000000 1000
./run_tests_alt2.sh 1000000 2000
# ntasks -> args = <ntasks> <wsize> <repeat>
./run_tests_ntasks.sh 2 1000000 1000
./run_tests_ntasks.sh 2 1000000 2000
./run_tests_ntasks.sh 4 1000000 1000
./run_tests_ntasks.sh 4 1000000 2000
./run_tests_ntasks.sh 8 1000000 1000
./run_tests_ntasks.sh 8 1000000 2000

# wsize = 10.000.000
# alt1 -> args = <wsize> <repeat>
./run_tests_alt1.sh 10000000 1000
./run_tests_alt1.sh 10000000 2000
# alt2 -> args = <wsize> <repeat>
./run_tests_alt2.sh 10000000 1000
./run_tests_alt2.sh 10000000 2000
# ntasks -> args = <ntasks> <wsize> <repeat>
./run_tests_ntasks.sh 2 10000000 1000
./run_tests_ntasks.sh 2 10000000 2000
./run_tests_ntasks.sh 4 10000000 1000
./run_tests_ntasks.sh 4 10000000 2000
./run_tests_ntasks.sh 8 10000000 1000
./run_tests_ntasks.sh 8 10000000 2000
