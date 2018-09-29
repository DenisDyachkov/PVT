#include <mpi.h>
#include <stdio.h>

#define BUFFER_SIZE 1024

int main(int argc, char **argv) {
    int commsize, rank;
    char buffer[BUFFER_SIZE];
   
    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &commsize);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    sprintf(buffer, "Message sended from %d. Message: \"Hi\"", rank);
    
    int dest = (rank == commsize - 1 ? 0 : rank + 1);
    int source = (rank == 0 ? commsize - 1 : rank - 1);
    int sended = 0;
     
    do {
        MPI_Send(buffer, BUFFER_SIZE, MPI_CHAR, dest, 0, MPI_COMM_WORLD);
        printf("[%d]: Sent message to %d: <%s>!\n", rank, dest, buffer);
        MPI_Recv(buffer, BUFFER_SIZE, MPI_CHAR, source, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        printf("[%d]: Recv message from %d: <%s>\n", rank, source, buffer);
        ++sended;
    } while(sended < commsize);
    
    MPI_Finalize();
    return 0;
}
