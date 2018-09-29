#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char **argv) {
    int buffer_size = 1024;
    if (argc > 1) {
        sscanf(argv[1], "%d", &buffer_size);
    }

    int commsize, rank, wait_count = 1;
    char *sbuffer = NULL,
         *rbuffer = (char*)malloc(sizeof(char) * buffer_size);
    MPI_Request *request = NULL;
    MPI_Status *stats = NULL;
    double time = 0;

    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &commsize);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    if (rank == 0) {
        wait_count = commsize + 1;
        request = (MPI_Request*)malloc(sizeof(*request) * wait_count);
        stats = (MPI_Status*)malloc(sizeof(*stats) * wait_count);
        sbuffer = (char*)malloc(sizeof(char) * buffer_size);
        time = MPI_Wtime();
        for (int i = 0; i < commsize; ++i)
            MPI_Isend(sbuffer, buffer_size, MPI_CHAR, i, 0, MPI_COMM_WORLD, &request[i]);
    } else {
        request = (MPI_Request*)malloc(sizeof(*request) * wait_count);
        stats = (MPI_Status*)malloc(sizeof(*stats) * wait_count);
    }

    MPI_Irecv(rbuffer, buffer_size, MPI_CHAR, 0, 0, MPI_COMM_WORLD, &request[wait_count - 1]);
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

