#include <stdio.h>
#include <stdlib.h>
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
    int myrank, nproc, from_row, to_row, i, j, k;
    int *sendcounts; // integer array specifying the number of elements to send to each process
    int *displs;     // each entry i specifies the displacement (relative to sendbuff) to each process
    int rows_slice;  // integer slice of rows for each process
    int rest;        // rest of total rows of the matrix (SIZE) % nproc 
    int lastrow;     // auxiliary variable to pointer to last the mapped row
    
    MPI_Init (&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &myrank); /* who am i */
    MPI_Comm_size(MPI_COMM_WORLD, &nproc); /* number of processors */

    // There is no need to check if nproc is well-proportioned to the number of matrix rows
    /* if (SIZE % nproc != 0) {
        if (myrank == 0) 
        	printf("Matrix size not divisible by number of processors\n");
        MPI_Finalize();
        return -1;
    } */

    sendcounts = malloc(sizeof(int) * nproc); 
    displs = malloc(sizeof(int) * nproc);

    rows_slice = (int) SIZE / nproc; 
    rest = SIZE % nproc;
    lastrow = 0;
    
    for (i = 0; i < nproc; i++) {   // calculate elements and buffer offset for each process
        displs[i] = lastrow;        // offset receives the last mapped row
        lastrow += rows_slice;      // last mapped row plus the base slice of rows
        sendcounts[i] = rows_slice; // primarily the buffer receive the base number of rows for each process
        if (rest != 0) {          
            sendcounts[i]++;        // add one row for the process 'i' if there are still lines left to distribute (rest)
            lastrow++;              // update pointer to last mapped row
            rest--;                 // one row less to distribute
        } 
        // IMPORTANT: now it's time to convert the number of rows of process 'i' in the number of elements that this 
        // process will receive by MPI_Scatterv routine. it's also necessary to convert the rows-based offset of process 'i'
        // in the number of elements to offset in the sendbuff. 
        sendcounts[i] *= SIZE;     // rows vs numbers of elements of each row
    	displs[i] *= SIZE;         // rows vs numbers of elements of each row
    }
    
        
    from_row = displs[myrank] / SIZE;
    to_row = from_row + (sendcounts[myrank] / SIZE);

    /* Process 0 fills the input matrices and broadcasts them to the rest */
    /* (actually, only the relevant stripe of A is sent to each process) */
    if (myrank == 0) {
        // check if the distribution is ok
    	/* for (i = 0; i < nproc; i++) {
   	    printf("RANK=%d -> ROWS=%d, ROWS_OFFSET=%d\n", i, sendcounts[i] / SIZE, displs[i] / SIZE); 
   	    printf("RANK=%d -> ELEMENTS=%d, OFFSET_ELEMENTS=%d\n", i, sendcounts[i], displs[i]);
   	} */
        fill_matrix(A);
        fill_matrix(B);
    }

    // Broadcast B to other process with MPI_Bcast.
    MPI_Bcast(B, SIZE * SIZE, MPI_INT, 0, MPI_COMM_WORLD);
    
    // In this example the receive buffer isn't setted up based on the process rank.
    // MPI_Scatterv(const void *sendbuf, const int *sendcounts, const int *displs,
    //              MPI_Datatype sendtype, void *recvbuf, int recvcount, MPI_Datatype recvtype, int root, MPI_Comm comm)
    MPI_Scatterv(A, sendcounts, displs, MPI_INT, A, sendcounts[myrank], MPI_INT, 0, MPI_COMM_WORLD);
    
    printf("computing slice %d (from row %d to %d)\n", myrank, from_row, to_row - 1);
    // IMPORTANT: now the break condition of the loop is the number of rows designed to each process. 
    // this number is obtained with the same basic logic used before: divide the number of elements recived 
    // by process 'i' to the number of elements of each row
    for (i = 0; i < sendcounts[myrank] / SIZE; i++) {
        for (j = 0; j < SIZE; j++) {
            C[i][j] = 0;
            for (k = 0; k < SIZE; k++) {
                C[i][j] += A[i][k] * B[k][j];
            }
        }
    }

    // In this example the send buffer isn't setted up based on process rank.
    // MPI_Gatherv(const void *sendbuf, int sendcount, MPI_Datatype sendtype, void *recvbuf, 
    //             const int *recvcounts, const int *displs, MPI_Datatype recvtype, int root, MPI_Comm comm)
    MPI_Gatherv(C, sendcounts[myrank], MPI_INT, C, sendcounts, displs, MPI_INT, 0, MPI_COMM_WORLD);
    
    if (myrank == 0) {
        printf("\n\n");
        print_matrix(A);
        printf("\n\n\t       * \n");
        print_matrix(B);
        printf("\n\n\t       = \n");
        print_matrix(C);
        printf("\n\n");
    }

    free(sendcounts);
    free(displs);

    MPI_Finalize();
    return 0;
}
