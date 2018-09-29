#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char **argv) {
    int buffer_size = 1;
    if (argc > 1) {
        sscanf(argv[1], "%d", &buffer_size);
    }

    int commsize, rank;
    char *sbuffer = (char*)malloc(sizeof(char) * buffer_size),
         *rbuffer = (char*)malloc(sizeof(char) * buffer_size);
    MPI_Request request[2];
    MPI_Status stats[2];
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
        MPI_Isend(sbuffer, buffer_size, MPI_CHAR, dest, 0, MPI_COMM_WORLD, &request[0]);
        MPI_Irecv(rbuffer, buffer_size, MPI_CHAR, source, 0, MPI_COMM_WORLD, &request[1]);
        MPI_Waitall(2, request, stats);
        ++sended;
    } while(sended < commsize - 1);

    if (rank == 0)
        printf("Time: %.6f\n", MPI_Wtime() - time);

    MPI_Finalize();
    free(sbuffer);
    free(rbuffer);
    return 0;
}

