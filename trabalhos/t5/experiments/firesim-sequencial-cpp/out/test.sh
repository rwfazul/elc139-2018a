#/bin/bash

start=0
end=3


for (( i = $start; i < $end; i++ )) 
do
	./firesim 30 5000 101 >> out.txt
done
