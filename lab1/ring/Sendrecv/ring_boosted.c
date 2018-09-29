#include <mpi.h>
#include <stdio.h>

#define BUFFER_SIZE 1024 * 1024

int main(int argc, char **argv) {
    int commsize, rank;
    char sbuffer[BUFFER_SIZE], rbuffer[BUFFER_SIZE];;
    double time = 0;

    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &commsize);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    int dest = (rank + 1) % commsize;
    int source = (rank + commsize - 1) % commsize;
    int sended = 0;

    if (rank == 0)
        time = MPI_Wtime();

    do {
        MPI_Sendrecv(sbuffer, BUFFER_SIZE, MPI_CHAR, dest, 0,
                     rbuffer, BUFFER_SIZE, MPI_CHAR, source, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        ++sended;
    } while(sended < commsize - 1);

    if (rank == 0)
        printf("Time: %.6f\n", MPI_Wtime() - time);

    MPI_Finalize();
    return 0;
}
