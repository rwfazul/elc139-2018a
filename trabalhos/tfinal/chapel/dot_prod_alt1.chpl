/*  
    $ chpl -o dot_prod_alt1 dot_prod_alt1.chpl --fast
    $ ./dot_prod_alt1 <worksize> <repetitions>
*/

module DotProd {

    use Time; 
  
    proc main(args: [] string) {
        var wsize: int;   // worksize = tamanho do vetor de cada task
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

        // Calcula c = a . b em tasks (forall), medindo o tempo
        timer.start( ); 
        var c: real = dotprod_chpl(a, b, wsize, repetitions);
        timer.stop( ); 
        // Mostra resultado e estatisticas da execucao
        writeln(c);
        writeln(timer.elapsed( ), " usec"); 
        timer.clear( ); 
    }
  
  
    proc dotprod_chpl(a: [?DomainA] real, b: [?DomainB] real, wsize: int, repeat: int) : real {
        var mysum: real;
        
        for k in 1..repeat do {
            mysum = 0.0;
            [ i in {1..wsize} with (+ reduce mysum) ]
                mysum += a[i] * b[i];
        }
       
        return mysum;
    }
  
}
