#/bin/bash

# Para funcionamento correto o retorno da execucao de $file deve retornar (printar) apenas o tempo de execucao (numero)

ARGC=$#    # testes param.
MAX_ARGS=3 # <rows> <columns> <iterations>
PROGRAM="mandelbrot"	
INPUT_FILE="mandelbrot.in" 

RunTests () {
    start=0
    end=30 #30 execucoes para cada configuracao de teste
    time_finish=0
    output_file="mandelbrot_$1-$2-$3.out"
	
    for (( i = $start; i < $end; i++ )) 
    do
        ./$PROGRAM < $INPUT_FILE >> $output_file
        tempo=`sed -n '$p' $output_file`
        time_finish=`echo $tempo + $time_finish | bc`
    done
	
    # echo "num.  exec. (tests): " $end
    # echo "total time: " $time_finish

    media=$(echo $time_finish/$end | bc -l)
    echo "media: " $media
}


if [ $ARGC -ne $MAX_ARGS ]; then 
    echo -e "Uso: $0 <max_rows> <max_columns> <iterations>\n">&2 #testes param.
else
    # make
    echo -e "$1\n$2\n$3" > $INPUT_FILE
    RunTests $1 $2 $3 # $1 = rows, $2 columns, $3 = iterations
fi
