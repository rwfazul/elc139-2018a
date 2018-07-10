/*  
    $ chpl -o dot_prod_ntasks dot_prod_ntasks.chpl --fast
    $ ./dot_prod_ntasks <ntasks> <worksize> <repetitions>
*/

module DotProd {

    use Time; 
    
    // acessivel por todas tasks (variavel atomica)
    var c: atomic real;
  
    proc main(args: [] string) {
        var ntasks: int;      // numero de tasks a serem criadas
        var wsize: int;       // worksize = tamanho do vetor de cada task
        var repetitions: int; // numero de repeticoes dos calculos (para aumentar carga)
        var timer: Timer;

        if args.size != 4 {
            writeln("Uso: ", args[0], " <ntasks> <worksize> <repetitions>");
            exit(-1);
        } 
        try! {
            ntasks = args[1]: int;
            wsize = args[2]: int;
            repetitions = args[3]: int;
        }        
        
        // Cria vetores
        var size = {1..wsize};
        var a: [size] real = 0.01;
        var b: [size] real = 1.0;

        // Calcula c = a . b em ntasks, medindo o tempo
        timer.start( ); 
        dotprod_chpl(a, b, ntasks, wsize, repetitions);
        timer.stop( ); 
        // Mostra resultado e estatisticas da execucao
        // writeln(c);
        // writeln(timer.elapsed(TimeUnits.microseconds), " usec"); 
        writeln(timer.elapsed()); 
        timer.clear( ); 
    }
  
  
    proc dotprod_chpl(a: [?DomainA] real, b: [?DomainB] real, ntasks: int, wsize: int, repeat: int) : void {
        coforall t in {1..ntasks} do {
            var mysum: real;
            var slice: int = (wsize / ntasks) : int;
            var init: int = (t - 1) * slice;
            var end: int = if (t < ntasks) then (init + slice) else wsize;
            for 1..repeat do {
                mysum = 0.0;
                for i in {(init + 1)..end} do
                    mysum += a[i] * b[i];
            }
            c.add(mysum);
        }
    }
  
}
