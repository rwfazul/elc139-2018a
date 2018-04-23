#ifndef __RANDOM_H
#define __RANDOM_H

//  
// Classe para gera��o de n�meros pseudo-aleat�rios.
//
class Random
{
   public:
      Random();
      Random(unsigned int s);
      
      // Retorna pr�ximo n�mero da seq��ncia pseudo-aleat�ria
      // (valores entre 0.0 e 1.0)
      double nextDouble();

      void setSeed(unsigned int s);
      void setSeedByTime();

   private:
      unsigned int seed;
      
};

#endif

