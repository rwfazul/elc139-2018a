import x10.io.Console;

public class DotProd {
	
    private static val ACTIVITIES: int = Int.operator_as(4);
    
    private static val WSIZE_DEFAULT:  int = Int.operator_as( 10000 );
    private static val REPEAT_DEFAULT: int = Int.operator_as( 2000  );
    
    public static def main(args:Rail[String]) {

        val nActiv: int  = (args.size > 0) ? Int.parse( args(0) ) : ACTIVITIES;     // numero de activities a serem criadas
        val wsize:  int  = (args.size > 1) ? Int.parse( args(1) ) : WSIZE_DEFAULT;  // worksize = tamanho do vetor de cada activity
        val repeat: int  = (args.size > 2) ? Int.parse( args(2) ) : REPEAT_DEFAULT; // numero de repeticoes dos calculos (para aumentar carga)

        // Cria vetores
        val a = new Rail[Double](wsize, 0.01);    	
        val b = new Rail[Double](wsize, 1.0);    	
    	
        // acessivel por todas activities
        var c: Double = 0.0; 
        
        // Calcula c = a . b em nActivities, medindo o tempo
        var time: long = - System.nanoTime();
        finish for ( act in 0..(nActiv - 1) ) async {
            var mysum: Double = 0.0; 			
            val slice: int = (wsize / nActiv);
            val init: int = Int.operator_as( act * slice );
            val end: int = (act < (nActiv - 1)) ? (init + slice) : wsize;
            for ( 0..(repeat - 1) ) {
                mysum = 0.0;
                for ( i in init..(end - 1) )
                    mysum += a(i) * b(i);
                atomic { c += mysum; }
            }
        }
        time += System.nanoTime();
        
        // Mostra resultado e estatisticas da execucao
        Console.OUT.println(Int.operator_as(c));
        Console.OUT.println((time/1000) + " usec");
    }
  
}
