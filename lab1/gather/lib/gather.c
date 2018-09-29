#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>

#define BUFFER_SIZE 1024

int main(int argc, char **argv) {
    int commsize, rank, wait_count = 1;
    char sbuffer[BUFFER_SIZE];
    double time = 0;

    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &commsize);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    MPI_Request request[commsize + 1];
    MPI_Status stats[commsize + 1];
    if (rank == 0)
        time = MPI_Wtime();

    char rbuffer[commsize * BUFFER_SIZE];
    MPI_Gather(sbuffer, BUFFER_SIZE, MPI_CHAR, rbuffer, BUFFER_SIZE, MPI_CHAR, 0, MPI_COMM_WORLD);

    printf("[%d] Time: %.6f\n", rank, MPI_Wtime());
    if (rank == 0)
        printf("Time: %.6f\n", MPI_Wtime() - time);

    MPI_Finalize();
    return 0;
}
