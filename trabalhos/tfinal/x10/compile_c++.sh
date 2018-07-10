#!/bin/bash
    
for i in *.x10; 
do 
    bin/x10c++ -O -o ${i%.x10} $i
    echo "$i -> ${i%.x10}"
done	
