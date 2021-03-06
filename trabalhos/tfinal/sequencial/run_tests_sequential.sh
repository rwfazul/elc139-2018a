#/bin/bash

# Para funcionamento correto o retorno da execucao de $PROGRAM deve retornar (printar) apenas o tempo de execucao (numero)

ARGC=$#    # testes param.
MAX_ARGS=2 # <wsize> <repeat>
PROGRAM="sequential_dotprod"

RunTests () {
    start=0
    end=30 #10 execucoes para cada configuracao de teste
    time_finish=0
    output_file="$PROGRAM-$1-$2.out"
	
    for (( i = $start; i < $end; i++ )) 
    do
        ./$PROGRAM $1 $2 >> $output_file
        tempo=`sed -n '$p' $output_file`
        time_finish=`echo $tempo + $time_finish | bc`
    done
	
    # echo "num.  exec. (tests): " $end
    # echo "total time: " $time_finish

    media=$(echo $time_finish/$end | bc -l)
    echo "media: " $media >> $output_file
}

if [ $ARGC -ne $MAX_ARGS ]; then 
    echo -e "Uso: $0 <wsize> <repeat>\n">&2 #testes param.
else
    # make
    RunTests $1 $2 # $1 = worksize, $2 = repetitions
fi
