// Task Parallelism (chapel nao determina como as tasks sao mapeadas para threads)

config const n = 10; // tasks a serem criados no loop coforall

writeln("1: ### BEGIN ###");

// begin cria uma nova thread independente da thread corrente (main)
begin writeln("1: output da nova thread"); 

// thread main continua (sem garantias de quem ira executar primeiro, pois ela nao espera)
writeln("1: output da main task");


writeln("2: ### COBEGIN ###");

// cobegin gera um bloco de tasks (uma pra cada instrução), thread main so continua depois de todas tasks do bloco cobegin finalizarem
cobegin {
  writeln("2: output task 1");
  writeln("2: output task 2");
}

writeln("2: output da main task");

writeln("3: ### COBEGIN COM BEGIN ###");

// se usar begin dentro do bloco, a main thread nao ira esperar essas instruções
cobegin {
  begin writeln("3: output task 1");
  begin writeln("3: output task 2");
}

writeln("3: output da main task");

writeln("4: ### COFORALL ###");

// cada iteracao eh uma nova task (main thread espera a conclusao de todas iteracoes)
coforall i in 1..n {
  // a ordem desses outputs eh deterministica
  writeln("4: (1) output task (i=", i, ")"); 
  writeln("4: (2) output task (i=", i, ")");
}

writeln("4: output da  main task");

writeln("5: ### COFORALL COM BEGIN ###");

// main task nao ira esperar
coforall i in 1..n {
  // condicao de corrida
  begin writeln("5: (1) output task (i=", i, ")");
  begin writeln("5: (2) output task (i=", i, ")");
}

writeln("5: output da main task");
