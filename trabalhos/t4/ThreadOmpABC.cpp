#include <algorithm> // necessary by std::count
#include <iostream>
#include <string>
#include <omp.h>

class SharedArray {
private:
   char* array; 
   int index; 
   int size;
   bool usecritical; // controls the use of pragma critical
public:
   SharedArray(int n, bool use) : index(0), size(n), usecritical(use) { /* fix compiler warning: members in the initializer list 
   	                                                 should appear in the same order as they appear in the class definition */
      array = new char[size];                                        
      std::fill(array, array+size, '-'); // fill the array with '-' (acts like an empty position)
   }
   ~SharedArray() { // class destructor
      delete[] array;
   }
   void addChar(char c) {
      if (usecritical) { 
         #pragma omp critical // the mutal exclusion is guaranteed only if usecritical is true
         {
            do_operations(c); 
         } 
      } else {
         do_operations(c);
      }
   }
   void do_operations(char c) {
      array[index] = c; 
      spendSomeTime();
      index++;
   }
   int countOccurrences(char c) {
      // array, array+size = the range of elements to examine, c = the value to search for
      return std::count(array, array+size, c);
   }
   std::string toString() { 
      return std::string(array, size);
   }
private:
   void spendSomeTime() {  // acts like a sleep()
      for (int i = 0; i < 10000; i++) {
         for (int j = 0; j < 100; j++) {
         
         }
      }
   }
};

class ArrayFiller {
private:
   static const int nThreads = 3;
   static const int nTimes = 20;
   SharedArray* array; 
   // used to define the omp schedule type on runtime
   omp_sched_t schedule_type; 
   bool use_defined_chunk = true; // if this value is not overwritten in the class constructor, then chunk_size will be = nTimes
   // used to match the correspondence schedule kind (integer number) to the schedule type (name)
   std::string schedule_types_strings[4] = { "omp_sched_static", "omp_sched_dynamic", "omp_sched_guided", "omp_sched_auto" };
public:
   ArrayFiller(bool usecritical, omp_sched_t schedule) : schedule_type(schedule) {
      array = new SharedArray(nThreads * nTimes, usecritical);
   }
   ArrayFiller(bool usecritical, omp_sched_t schedule, bool use_chunk) : schedule_type(schedule), use_defined_chunk(use_chunk) {
      array = new SharedArray(nThreads * nTimes, usecritical);
   }
   void fillArrayConcurrently() {
      // sets the value of the run-sched-var internal control variable
      int chunk_size = use_defined_chunk ? nTimes : 1; /* if zero or negative chunk_size will be set to the default value, 
                                                          but it will be printed with value 0 by omp_get_schedule */
      omp_set_schedule(schedule_type, chunk_size); // sets the runtime scheduling method
      check_schedule(); // prints the current schedule type and chunk size (when used)
      #pragma omp parallel num_threads(nThreads) // creates threads group
      {
         run('A' + omp_get_thread_num()); // 'A'+TID ('A', 'B' or 'C')
      }
   }
   void check_schedule() {
      // the type of schedule that will be used
      omp_sched_t schedule_kind; // omp_sched_static = 1, omp_sched_dynamic = 2, omp_sched_guided = 3, omp_sched_auto = 4  
      int chunk_size; // represents the chunk size that is set for applicable schedule types
      // returns the run-sched-var internal control variable of the team that is processing the parallel region
      omp_get_schedule(&schedule_kind, &chunk_size); // obtain the runtime scheduling method 
      std::cout << "\tschedule_type: "<< schedule_types_strings[schedule_kind - 1] << ", chunk_size: ";
      // for omp_sched_auto the chunk_size argument is ignored.
      (schedule_kind == omp_sched_auto) ? std::cout << " - " << std::endl : std::cout << chunk_size << std::endl; 
   }
   void printStats() {
      std::cout << array->toString() << std::endl; // print array positions
      for (int i = 0; i < nThreads; ++i) 
         std::cout << (char) ('A'+i) << "=" 
                   << array->countOccurrences('A'+i) << " "; // print frequency of the letters 'A', 'B', 'C' 
      std::cout << std::endl;
   }
   ~ArrayFiller() { // class destructor
      delete array;
   }
private:
   void run(char c) {
      /* Runtime schedule: the schedule type and chunk size are taken from the 
         run-sched-var (internal control variable) set with the execution 
         environment routine omp_set_schedule(). */
      #pragma omp for schedule(runtime)  // it is illegal to specify a chunk size for this clause (runtime)
      for (int i = 0; i < nThreads * nTimes; i++) {
         // std::cout << "TID=" << omp_get_thread_num() << ", i=" << i << std::endl;
         array->addChar(c);    
      }         
   }
};


int main() {
   std::cout << "Each thread should add its char to the array n times (n=20)" << std::endl;
   std::cout << "Correct results should total exactly nThreads*nTimes chars" << std::endl;
   std::cout << "------------------------------------" << std::endl;
   /* Case 1 and 2: Static schedule: iterations are divided into chunks of size 
      chunk_size, and the chunks are assigned to the threads in the team in a
      round-robin fashion in the order of the thread number. if chunk is not specified, 
      the iterations are evenly (if possible) divided contiguously among the threads. */
   std::cout << "* Case 1: no omp critical (expecting wrong results)" << std::endl;
   ArrayFiller m1(false, omp_sched_static);
   m1.fillArrayConcurrently();
   m1.printStats();
   std::cout << "------------------------------------" << std::endl;
   std::cout << "* Case 2: using omp critical (expecting correct results)" << std::endl;
   ArrayFiller m2(true, omp_sched_static);
   m2.fillArrayConcurrently();
   m2.printStats();
   std::cout << "------------------------------------" << std::endl;
   /* Case 3 and 4: Dynamic schedule: Each thread executes a chunk of iterations, 
      then requests another chunk, until no chunks remain to be distributed. */
   std::cout << "* Case 3: no omp critical (expecting wrong results)" << std::endl;
   ArrayFiller m3(false, omp_sched_dynamic);
   m3.fillArrayConcurrently();
   m3.printStats();
   std::cout << "------------------------------------" << std::endl;
   std::cout << "* Case 4: using omp critical (expecting correct results)" << std::endl;
   ArrayFiller m4(true, omp_sched_dynamic);
   m4.fillArrayConcurrently();
   m4.printStats();
   std::cout << "------------------------------------" << std::endl;
   /* The default chunk size of dynamic schedule is 1. */
   std::cout << "* Case 4b: using omp critical but chunk_size is the default value used by OpenMP" << std::endl;
   ArrayFiller m4b(true, omp_sched_static, false);
   m4b.fillArrayConcurrently();
   m4b.printStats();
   std::cout << "------------------------------------" << std::endl;
   /* Case 5 and 6: Guided schedule: Each thread executes a chunk of iterations,
      then requests another chunk, until no chunks remain to be assigned.
      The chunk sizes start large and shrink to the indicated chunk_size as chunks are scheduled. 
      Similar to DYNAMIC except that the block size decreases each time a parcel of work is given to 
      a thread. The size of the initial block is proportional to: (number_of_iterations / number_of_threads)
      Subsequent blocks are proportional to (number_of_iterations_remaining / number_of_threads). */
   std::cout << "* Case 5: no omp critical (expecting wrong results)" << std::endl;
   ArrayFiller m5(false, omp_sched_guided);
   m5.fillArrayConcurrently();
   m5.printStats();
   std::cout << "------------------------------------" << std::endl;
   std::cout << "* Case 6: using omp critical (expecting correct results)" << std::endl;
   ArrayFiller m6(true, omp_sched_guided);
   m6.fillArrayConcurrently();
   m6.printStats(); 
   std::cout << "------------------------------------" << std::endl;
   /* The chunk parameter defines the minimum block size. The default chunk size of guided schedule is 1. */
   std::cout << "* Case 6b: using omp critical but chunk_size is the default value used by OpenMP" << std::endl;
   ArrayFiller m6b(true, omp_sched_static, false); 
   m6b.fillArrayConcurrently();
   m6b.printStats();
   std::cout << "------------------------------------" << std::endl;
   /* Case 7 and 8: Auto schedule: The decision regarding scheduling is delegated 
      to the compiler and/or runtime system. */
   std::cout << "* Case 7: no omp critical (expecting wrong results)" << std::endl;
   ArrayFiller m7(false, omp_sched_auto);
   m7.fillArrayConcurrently();
   m7.printStats();
   std::cout << "------------------------------------" << std::endl;
   std::cout << "* Case 8: using omp critical (expecting correct results)" << std::endl;
   ArrayFiller m8(true, omp_sched_auto);
   m8.fillArrayConcurrently();
   m8.printStats();   
}