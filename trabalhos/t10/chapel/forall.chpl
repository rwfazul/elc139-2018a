// Data-parallel usando multiplos cores em um  único 'locale' (node)


module Forall {

  config const numMessages = 100;  // numero de menssagens serem printadas
  
  proc main() {
    // por default cria tasks = numero de cpu/cores
    forall i in 1..numMessages do
       // o procedimento writeln() protege as messangens de serem intercaladas entre as tasks
       writeln("Iteracao ", i, " de ", numMessages);
       
    /* sintaxe alternativa */
    [i in 1..numMessages] writeln("Iteracao ", i, " de ", numMessages);
  }

}
