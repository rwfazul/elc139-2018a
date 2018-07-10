/*  
    $ chpl -o dot_prod_alt2 dot_prod_alt2.chpl --fast
    $ ./dot_prod_alt2 <worksize> <repetitions>
*/

module DotProd {

    use Time; 
  
    proc main(args: [] string) {
        var wsize: int;       // worksize = tamanho do vetor de cada task
        var repetitions: int; // numero de repeticoes dos calculos (para aumentar carga)
        var timer: Timer;

        if args.size != 3 {
            writeln("Uso: ", args[0], " <worksize> <repetitions>");
            exit(-1);
        } 
        try! {
            wsize = args[1]: int;
            repetitions = args[2]: int;
        }        
        
        // Cria vetores
        var size = {1..wsize};
        var a: [size] real = 0.01;
        var b: [size] real = 1.0;
        var c: real;
        
        // Calcula c = a . b, medindo o tempo
        timer.start( ); 
        for 1..repetitions do
            c = + reduce (a * b); // (a * b) resulta em um array de dominio {1..wsize} onde array[i] = a[i] * b[i]
        timer.stop( ); 
        // Mostra resultado e estatisticas da execucao
        // writeln(c);
        // writeln(timer.elapsed(TimeUnits.microseconds), " usec"); 
        writeln(timer.elapsed()); 
        timer.clear( ); 
    }
  
}
