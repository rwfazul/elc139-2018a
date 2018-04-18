#/bin/bash

# g++ -std=c++11 ThreadDemoABC.cpp -pthread -o ThreadDemoABC -Wall
set -v
g++ -std=c++11 -fopenmp -o ThreadOmpABC ThreadOmpABC.cpp -pthread -Wall
