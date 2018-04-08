#/bin/bash

# Para funcionamento correto o retorno da execucao de $file deve retornar (printar) apenas o tempo de execucao (numero)

ARGC=$#    # testes param.
MAX_ARGS=4 # <file_name> <nthreads> <worksize> <repetitions>

RunTests () {
	file=$1
	start=0
	end=30 #30 execucoes para cada configuracao de teste
	time_finish=0
	
	for (( i = $start; i < $end; i++ )) 
	do
		./$file $2 $3 $4 >> out-$2-$3-$4.txt
		tempo=`sed -n '$p' out-$2-$3-$4.txt`
		time_finish=`echo $tempo + $time_finish | bc`
	done
	
	# echo "num.  exec. (tests): " $end
	# echo "total time: " $time_finish

	media=$(echo $time_finish/$end | bc -l)
	echo "media: " $media
}

if [ $ARGC -ne $MAX_ARGS ]; then 
    echo -e "Uso: $0 <file_name> <nthreads> <worksize> <repetitions>\n">&2 #testes param.
else
    make
    RunTests $1 $2 $3 $4 # $1 = file_name, $2 = num_threads, $3 tam_vets, $4 = repeticoes
fi
