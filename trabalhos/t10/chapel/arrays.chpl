config const n = 5;

// criacao
var A: [1..n] int;
writeln("A ", A);  // default = 0

// slicing
A[2..4] = 5;
writeln("A ", A);

// random
use Random; 
config const seed = 31415;
var B: [1..n] real; 
fillRandom(B, seed, algorithm=RNG.NPB);
writeln("B " + B);

/* matriz */
var C: [1..n, 1..n] int;

// ordem que percorre (serial)
for (i,j) in {1..n, 1..n} do
  write("(i=", i, ",j=", j, ") ");
writeln();

// paralelizando
forall (i,j) in {1..n, 1..n} do
  C(i,j) = 1;
writeln("\nC:\n", C, "\n");
// outra sintaxe
[(i,j) in {1..n, 1..n}] C(i,j) = 2;
writeln("C:\n", C, "\n");

// outra forma de iterar (domain)
forall (i,j) in C.domain do
  C(i,j) = 3;
writeln("C:\n", C, "\n");
// outra sintaxe
[(i,j) in C.domain] C(i,j) = 4;
writeln("C:\n", C, "\n");
