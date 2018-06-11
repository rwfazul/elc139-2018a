#include <stdio.h>
#include <mpi.h>

#define SIZE 8      /* Size of matrices */

int A[SIZE][SIZE], B[SIZE][SIZE], C[SIZE][SIZE];

void fill_matrix(int m[SIZE][SIZE]) {
    static int n = 0;
    int i, j;
    for (i = 0; i < SIZE; i++)
        for (j = 0; j < SIZE; j++)
            m[i][j] = n++;
}

void print_matrix(int m[SIZE][SIZE]) {
    int i, j;
    for (i = 0; i < SIZE; i++) {
        printf("\n\t| ");
        for (j = 0; j < SIZE; j++)
            printf("%*d", 6, m[i][j]);
        printf("|");
    }
}

int main(int argc, char *argv[]) {
    int myrank, nproc, i, j, k;
  
    MPI_Init (&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &myrank); /* who am i */
    MPI_Comm_size(MPI_COMM_WORLD, &nproc); /* number of processors */

    if (SIZE % nproc != 0) {
        if (myrank == 0) 
        	printf("Matrix size not divisible by number of processors\n");
        MPI_Finalize();
        return -1;
    }

    /* Process 0 fills the input matrices and broadcasts them to the rest */
    /* (actually, only the relevant stripe of A is sent to each process) */
    if (myrank == 0) {
        fill_matrix(A);
        fill_matrix(B);
    }

    // Broadcast B to other process with MPI_Bcast.
    // Root process (myrank = 0) sends the same data to all processes in the communicator.
    // MPI_Bcast uses a tree-based communication (it's like a pipeline where even the processes that 
    // aren't root help out the root process by forwarding the data to remaining processes).

    // MPI_Bcast(void* data, int count, MPI_Datatype datatype, int root, MPI_Comm communicator)
    MPI_Bcast(B, SIZE * SIZE, MPI_INT, 0, MPI_COMM_WORLD);


    // Send "Total of lines" / "Number of processes" lines to the processes with MPI_Scatter.
    // MPI_Scatter sends chunks of an array to different processes, although the root proccess 
    // (myrank = zero) contains the entire array of data, MPI_Scatter will copy the appropriate
    // elements into the receiving buffer of the process.
    // In this example the receive buffer insn't setted up based on process rank.

    // MPI_Scatter(void* send_data, int send_count, MPI_Datatype send_datatype, void* recv_data, 
    //             int recv_count, MPI_Datatype recv_datatype, int root, MPI_Comm communicator)
    MPI_Scatter(A, SIZE * SIZE / nproc, MPI_INT, A, SIZE * SIZE / nproc, MPI_INT, 0, MPI_COMM_WORLD);

    printf("computing slice %d (from row %d to %d)\n", myrank, myrank * SIZE / nproc, (myrank + 1) * SIZE / nproc);
    // given that the receive/send buffer aren't setted up, the for loop can init at 0
    // (there is no need to start the loop in a Rank-based position that represents the slice of each process)
    for (i = 0; i < SIZE / nproc; i++) {
        for (j = 0; j < SIZE; j++) {
            C[i][j] = 0;
            for (k = 0; k < SIZE; k++) {
                C[i][j] += A[i][k] * B[k][j];
            }
        }
    }

    // Similar to MPI_Scatter, MPI_Gather takes elements from each process and gathers them to the root process.
    // In MPI_Gather, only the root process needs to have a valid receive buffer.
    // IMPORTANT: recv_count parameter is the count of elements received per process, not the total count from all processes. 
    // In this example the send buffer isn't setted up based on process rank.

    // MPI_Gather(void* send_data, int send_count, MPI_Datatype send_datatype, void* recv_data,
    //            int recv_count, MPI_Datatype recv_datatype, int root, MPI_Comm communicator)
    MPI_Gather(C, SIZE * SIZE / nproc, MPI_INT, C, SIZE * SIZE / nproc, MPI_INT, 0, MPI_COMM_WORLD);

    if (myrank == 0) {
        printf("\n\n");
        print_matrix(A);
        printf("\n\n\t       * \n");
        print_matrix(B);
        printf("\n\n\t       = \n");
        print_matrix(C);
        printf("\n\n");
    }

    MPI_Finalize();
    return 0;
}
