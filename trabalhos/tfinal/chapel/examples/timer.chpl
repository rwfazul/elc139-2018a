use Time; 

var timer: Timer;

var mat: [{1..1000,1..1000}] real;

// Serial
timer.start( ); 
for (x,y) in mat.domain {
  mat[x,y] = (x:real) / (y:real);
}
timer.stop( ); 
writeln( "Serial: ", timer.elapsed( ) ); 
timer.clear( ); 


// Paralelo
timer.start( );
forall (x,y) in mat.domain { 
  mat[x,y] = (x:real) / (y:real);
}
timer.stop( );
writeln( "Parallel: ", timer.elapsed( ) );
timer.clear( );
