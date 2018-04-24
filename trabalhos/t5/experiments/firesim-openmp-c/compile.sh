#/bin/bash

set -v
gcc -fopenmp -o burn_rec burn_rec.c -Wall
gcc -fopenmp -o burn_me burn_me.c -Wall
