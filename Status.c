#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <mpi.h>
#include <time.h>

int main(int argc, char *argv[])
{
  int MAX_ARRAY_SIZE = 100;
  int array_size;
  int how_many_received;
  int array[MAX_ARRAY_SIZE];
  int num_procs, rank, ierr, tag = 1;
  MPI_Status status;

  MPI_Init(&argc, &argv);
  ierr = MPI_Comm_size(MPI_COMM_WORLD, &num_procs);
  ierr = MPI_Comm_rank(MPI_COMM_WORLD, &rank);

  if (rank == 0)
  {
    srand(time(NULL));
    rand();
    array_size = (rand() / (float)RAND_MAX) * MAX_ARRAY_SIZE;
    printf("rank %d: sending %d integers\n", rank, array_size);
    ierr = MPI_Send(array, array_size, MPI_INT, 1, tag, MPI_COMM_WORLD);
  }
  else if (rank == 1)
  {
    ierr = MPI_Recv(array, MAX_ARRAY_SIZE, MPI_INT, 0, tag, MPI_COMM_WORLD, &status);
   // Gets the number of "top level" elements
   // int MPI_Get_count(const MPI_Status *status, MPI_Datatype datatype, int *count)
    MPI_Get_count(&status, MPI_INT, &how_many_received);
    printf("rank %d: received %d integers, tag = %d, sender = %d\n", rank, how_many_received, status.MPI_TAG, status.MPI_SOURCE);
  }

  MPI_Finalize();
  return 0;

}
