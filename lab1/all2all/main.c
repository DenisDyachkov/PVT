#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>

#define BUFFER_SIZE 1024 

int main(int argc, char **argv) {
    int commsize, rank;
    char sbuffer[BUFFER_SIZE];
    double time = 0;

    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &commsize);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    char rbuffer[commsize * BUFFER_SIZE];
    MPI_Request request[commsize * 2];
    MPI_Status stats[commsize * 2];
    time = MPI_Wtime();

    for (int i = 0; i < commsize; ++i) {
        MPI_Isend(sbuffer, BUFFER_SIZE, MPI_CHAR, i, 0, MPI_COMM_WORLD, &request[i]);
        MPI_Irecv(&rbuffer[i * commsize], BUFFER_SIZE, MPI_CHAR, i, 0, MPI_COMM_WORLD, &request[i + commsize]);
    }
    MPI_Waitall(commsize * 2, request, stats);

    printf("[%d] Time: %.6f\n", rank, MPI_Wtime() - time);

    MPI_Finalize();
    return 0;
}
