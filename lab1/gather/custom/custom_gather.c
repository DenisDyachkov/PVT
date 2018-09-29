#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char **argv) {
    int buffer_size = 1;
    if (argc > 1) {
        sscanf(argv[1], "%d", &buffer_size);
    }

    int commsize, rank, wait_count = 1;
    char *rbuffer = NULL,
         *sbuffer = (char*)malloc(sizeof(char) * buffer_size);
    double time = 0;

    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &commsize);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    MPI_Request request[commsize + 1];
    MPI_Status stats[commsize + 1];
    if (rank == 0)
        time = MPI_Wtime();

    MPI_Isend(sbuffer, buffer_size, MPI_CHAR, 0, 0, MPI_COMM_WORLD, &request[0]);

    if (rank == 0) {
        wait_count = commsize + 1;
        rbuffer = (char*)malloc(sizeof(char) * commsize * buffer_size);
        for (int i = 0; i < commsize; ++i)
            MPI_Irecv(&rbuffer[i * commsize], buffer_size, MPI_CHAR, i, 0, MPI_COMM_WORLD, &request[i + 1]);
    }
    MPI_Waitall(wait_count, request, stats);

    printf("[%d] Time: %.6f\n", rank, MPI_Wtime());
    if (rank == 0)
        printf("Time: %.6f\n", MPI_Wtime() - time);

    MPI_Finalize();
    free(stats);
    free(request);
    free(sbuffer);
    free(rbuffer);

    return 0;
}
