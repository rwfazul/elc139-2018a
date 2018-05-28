#/bin/bash

# Para funcionamento correto o retorno da execucao de mpi_dotprod deve retornar (printar) apenas o tempo de execucao (numero)

ARGC=$#    # testes param.
MAX_ARGS=3 # <num_processos> <worksize> <repetitions>

RunTests () {
	file="mpi_dotprod"
	start=0
	end=30 #30 execucoes para cada configuracao de teste
	time_finish=0
	
	for (( i = $start; i < $end; i++ )) 
	do
		mpiexec -np $1 mpi_dotprod $2 $3 >> out-$1-$2-$3.txt
		tempo=`sed -n '$p' out-$1-$2-$3.txt`
		time_finish=`echo $tempo + $time_finish | bc`
	done
	
	# echo "num.  exec. (tests): " $end
	# echo "total time: " $time_finish

	media=$(echo $time_finish/$end | bc -l)
	echo "media: " $media
}

if [ $ARGC -ne $MAX_ARGS ]; then 
    echo -e "Uso: $0 <num_processos> <worksize> <repetitions>\n">&2 #testes param.
else
    # make
    RunTests $1 $2 $3 # $1 = num_processos, $2 = tam_vets, $3 = repeticoes
fi
