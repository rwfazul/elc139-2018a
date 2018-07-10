#/bin/bash

# Para funcionamento correto o retorno da execucao de $PROGRAM deve retornar (printar) apenas o tempo de execucao (numero)

ARGC=$#    # testes param.
MAX_ARGS=3 # <ntasks> <wsize> <repeat>
PROGRAM="dot_prod_ntasks"	

RunTests () {
    start=0
    end=30 #30 execucoes para cada configuracao de teste
    time_finish=0
    output_file="$PROGRAM-$1-$2-$3.out"
	
    for (( i = $start; i < $end; i++ )) 
    do
        ./$PROGRAM $1 $2 $3 >> $output_file
        tempo=`sed -n '$p' $output_file`
        time_finish=`echo $tempo + $time_finish | bc`
    done
	
    # echo "num.  exec. (tests): " $end
    # echo "total time: " $time_finish

    media=$(echo $time_finish/$end | bc -l)
    echo "media: " $media >> $output_file
}

if [ $ARGC -ne $MAX_ARGS ]; then 
    echo -e "Uso: $0 <ntasks> <wsize> <repeat>\n">&2 #testes param.
else
    # ./compile.sh
    RunTests $1 $2 $3 # $1 = ntasks, $2 = worksize, $3 = repetitions
fi
