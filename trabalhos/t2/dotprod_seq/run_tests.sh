#/bin/bash

ARGC=$#    # testes param.
MAX_ARGS=1 # <repeticoes por tam. vetor>

FILE="dotprod_seq"
TAM_VET=10 # default = inicio em 10
END=7      # default = 7 iterações (Tamanho max. vetor: TAM_VET^END = 10^7 = 10.000.000)

RunTests () {
    # default laço = tamanhos do vetor: 10, 50, 100, 500 ... 10.000.000, 50.000.000 (TAM_VET^END = 10^7 = 100.00.000)
    for (( i = 0; i < $END; i++ )) 
    do
        ( (echo -e "<$TAM_VET> <$1>") && (./$FILE $TAM_VET $1) ) >> out.txt                     # 10, 100 ... 10.000.000
        ( (echo -e "<$(( TAM_VET * 5 ))> <$1>") && (./$FILE $(( TAM_VET * 5 )) $1) ) >> out.txt # 50, 500 ... 50.000.000
        (( TAM_VET *= 10 ))
    done
    
    ( (echo -e "<$TAM_VET> <$1>") && (./$FILE $TAM_VET $1) ) >> out.txt # 100.000.000
}

if [ $ARGC -ne $MAX_ARGS ]; then 
    echo -e "Uso: $0 <repeticoes por tam. vetor>\n">&2 #testes param.
else
    gcc -o $FILE $FILE.c 
    RunTests $1 # $1 = <repeticoes>
fi
