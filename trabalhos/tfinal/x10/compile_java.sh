#!/bin/bash
    
for i in *.x10; 
do 
    bin/x10c $i
    echo "Compiled: $i"
done	
