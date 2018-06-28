// Task-parallel usando multiplos cores em um  único 'locale' (node)

module Coforall {

  /* numero de tasks a serem criadas, por default seta com uma estimação em tempo de execução 
     da capacidade maxima de paralelismo que o locale corrente ('here') suporta executar em paralelo */
  config const numTasks = here.maxTaskPar; 
  
  proc main() {
  
    // cria uma task por iteracao
    coforall tid in 0..#numTasks do
      writeln("Task" + tid + " de " + numTasks);


    /* sintaxe alternativa */
    [i in 1..numMessages] writeln("Iteracao ", i, " de ", numMessages);
  }

}
