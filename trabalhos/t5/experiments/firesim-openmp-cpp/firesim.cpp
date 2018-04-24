//  
// Simulação de incêndio em uma floresta.
// Baseada no código proposto por David Joiner.
//
// Uso: firesim <nro. threads> <tamanho-do-problema> <nro. experimentos> <probab. maxima> 

#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <chrono>
#include <omp.h>
#include "Random.h"
#include "Forest.h"

void
checkCommandLine(int argc, char** argv, int& threads, int& size, int& trials, int& probs)
{
   if (argc > 1) threads = atoi(argv[1]);
   if (argc > 2) size = atoi(argv[2]);
   if (argc > 3) trials = atoi(argv[3]);
   if (argc > 4) probs = atoi(argv[4]);
}

int 
main(int argc, char* argv[]) 
{
   typedef std::chrono::high_resolution_clock Clock;
   // parâmetros dos experimentos
   int n_threads = 4;
   int forest_size = 30;
   int n_trials = 5000;
   int n_probs = 101;

   double* percent_burned; // percentuais queimados (saída)
   double* prob_spread;    // probabilidades (entrada)
   double prob_min = 0.0;
   double prob_max = 1.0;
   double prob_step;
   int base_seed = 100;

   checkCommandLine(argc, argv, n_threads, forest_size, n_trials, n_probs);
    
   try {
      int ip, it, chunk_size = n_probs / n_threads;
      Random rand;

      prob_spread = new double[n_probs];
      percent_burned = new double[n_probs];

      prob_step = (prob_max - prob_min)/(double)(n_probs-1);

      printf("Probabilidade, Percentual Queimado\n");

      auto t_start = Clock::now();
      // para cada probabilidade, calcula o percentual de árvores queimadas
      #pragma omp parallel private(ip, it, rand) num_threads(n_threads)
      {
         Forest* forest = new Forest(forest_size);
         // #pragma omp for schedule(dynamic)
         #pragma omp for schedule(static, chunk_size)
         for (ip = 0; ip < n_probs; ip++) {
            prob_spread[ip] = prob_min + (double) ip * prob_step;
            percent_burned[ip] = 0.0;
            rand.setSeed(base_seed+ip); // nova seqüência de números aleatórios

            // executa vários experimentos
            for (it = 0; it < n_trials; it++) {
               // queima floresta até o fogo apagar
               forest->burnUntilOut(forest->centralTree(), prob_spread[ip], rand);
               percent_burned[ip] += forest->getPercentBurned();
            }

            // calcula média dos percentuais de árvores queimadas
            percent_burned[ip] /= n_trials;
            // mostra resultado para esta probabilidade
            printf("%lf, %lf\n", prob_spread[ip], percent_burned[ip]);
         }
      }
      auto t_end = Clock::now();
      std::cout << std::chrono::duration_cast<std::chrono::nanoseconds>(t_end - t_start).count()
                << " nanoseconds" << std::endl;
      
      delete[] prob_spread;
      delete[] percent_burned;
   }
   catch (std::bad_alloc)
   {
      std::cerr << "Erro: alocacao de memoria" << std::endl;
      return 1;
   }

   return 0;
}

