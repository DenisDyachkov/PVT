#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>

#define BUFFER_SIZE 1024

int main(int argc, char **argv) {
    int rank;
    char buffer[BUFFER_SIZE];
    double time = 0;

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    if (rank == 0)
        time = MPI_Wtime();

    MPI_Bcast(buffer, BUFFER_SIZE, MPI_CHAR, 0, MPI_COMM_WORLD);
    printf("[%d] Time: %.6f\n", rank, MPI_Wtime());

    if (rank == 0)
        printf("Time: %.6f\n", MPI_Wtime() - time);

    MPI_Finalize();
    return 0;
}
