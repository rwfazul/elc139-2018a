#!/bin/bash
    
for i in *.chpl; 
do 
    chpl -o ${i%.chpl} $i --fast
    echo "$i -> ${i%.chpl}"
done	
