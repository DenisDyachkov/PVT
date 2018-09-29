#include <mpi.h>
#include <stdio.h>

#define BUFFER_SIZE 1024

int main(int argc, char **argv) {
    int commsize, rank;
    char buffer[BUFFER_SIZE];
    double time = 0;
    
    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &commsize);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    
    int dest = (rank == commsize - 1 ? 0 : rank + 1);
    int source = (rank == 0 ? commsize - 1 : rank - 1);
    int sended = 0;
    
    if (rank == 0)
        time = MPI_Wtime();
    
    do {
        MPI_Send(buffer, BUFFER_SIZE, MPI_CHAR, dest, 0, MPI_COMM_WORLD);
        MPI_Recv(buffer, BUFFER_SIZE, MPI_CHAR, source, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        ++sended;
    } while(sended < commsize);
    
    if (rank == 0)
        printf("Time: %.6f\n", MPI_Wtime() - time);
    
    MPI_Finalize();
    return 0;
}
