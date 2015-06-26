#include <stdlib.h>
#include <stdio.h>
#include <mpi.h>

int main(int argc, char *argv[])
{
  int num_procs, rank, left, right, ierr;

  int data_from_left, data_from_right;
  int tag1 = 1, tag2 = 2;

  MPI_Request request[4];

  MPI_Init(&argc, &argv);
  ierr = MPI_Comm_size(MPI_COMM_WORLD, &num_procs);
  ierr = MPI_Comm_rank(MPI_COMM_WORLD, &rank);

  left = (int)(rank + num_procs - 1) % num_procs;
  right = (int)(rank + 1) % num_procs;

  printf("I am %2d: sending %2d to my right and my left\n", rank, rank);
  MPI_Barrier(MPI_COMM_WORLD);

  // Begins a nonblocking send
  // int MPI_Isend(const void *buf, int count, MPI_Datatype datatype, int dest, int tag, MPI_Comm comm, MPI_Request *request)
  MPI_Isend(&rank, 1, MPI_INT, left, tag1, MPI_COMM_WORLD, &request[0]);
  MPI_Isend(&rank, 1, MPI_INT, right, tag2, MPI_COMM_WORLD, &request[1]);

  MPI_Irecv(&data_from_right, 1, MPI_INT, right, tag1, MPI_COMM_WORLD, &request[2]);
  MPI_Irecv(&data_from_left, 1, MPI_INT, left, tag2, MPI_COMM_WORLD, &request[3]);

  MPI_Barrier(MPI_COMM_WORLD);
  printf("I am %2d: got %2d from my left, got %2d from my right\n", rank, data_from_left, data_from_right);

  MPI_Finalize();
  return 0;

}
