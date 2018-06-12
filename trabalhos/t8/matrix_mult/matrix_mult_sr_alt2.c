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
    int myrank, nproc, from, to, i, j, k;
  
    MPI_Init (&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &myrank); /* who am i */
    MPI_Comm_size(MPI_COMM_WORLD, &nproc); /* number of processors */

    if (SIZE % nproc != 0) {
        if (myrank == 0) 
        	printf("Matrix size not divisible by number of processors\n");
        MPI_Finalize();
        return -1;
    }

    from = myrank * SIZE / nproc;
    to = (myrank + 1) * SIZE / nproc;

    /* Process 0 fills the input matrices and broadcasts them to the rest */
    /* (actually, only the relevant stripe of A is sent to each process) */
    if (myrank == 0) {
        fill_matrix(A);
        fill_matrix(B);
    }

    // Broadcast B to other process with MPI_Bcast.
    MPI_Bcast(B, SIZE * SIZE, MPI_INT, 0, MPI_COMM_WORLD);
    // Send "Total of lines" / "Number of processes" lines to the processes with MPI_Scatter.
    // In this example the receive buffer is setted up based on the process rank.
    MPI_Scatter(A, SIZE * SIZE / nproc, MPI_INT, &A[from], SIZE * SIZE / nproc, MPI_INT, 0, MPI_COMM_WORLD);

    printf("computing slice %d (from row %d to %d)\n", myrank, from, to - 1);
    // Now it's important to init the for loop in the correct position, because the array A began 
    // to be filled up by Mpi_Scatter in a position based on the process Rank.
    for (i = from; i < to; i++) {
        for (j = 0; j < SIZE; j++) {
            C[i][j] = 0;
            for (k = 0; k < SIZE; k++) {
                C[i][j] += A[i][k] * B[k][j];
            }
        }
    }

    // takes elements from each process and gathers them to the root process with MPI_Gather.
    // In this example the send buffer is setted up based on process rank.
    MPI_Gather(C[from], SIZE * SIZE / nproc, MPI_INT, C, SIZE * SIZE / nproc, MPI_INT, 0, MPI_COMM_WORLD);

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
