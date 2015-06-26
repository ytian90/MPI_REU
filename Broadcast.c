// This MPI program demonstrate the Broadcast in MPI.

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <mpi.h>

// our version of broadcast takes the same arguments.
void my_bcast(void *data, int count, MPI_Datatype datatype, int root, MPI_Comm comm)
{
  int my_rank, my_num_procs, my_ierr;
  int i;
  MPI_Status status;
  my_ierr = MPI_Comm_size(comm, &my_num_procs);
  my_ierr = MPI_Comm_rank(comm, &my_rank);

  if (my_rank == root) // root, send others except itself
  {
    for(i = 0; i < my_num_procs; i++)
    {
      if (i != my_rank)
        MPI_Send(data, count, datatype, i, 0, comm);
    }
  }
  else // others, receive from root
    MPI_Recv(data, count, datatype, root, 0, comm, &status);

}

int main(int argc, char *argv[])
{
  int num_procs, rank, ierr, dest, src, tag = 1;
  int i;
  MPI_Status status;

  MPI_Init(&argc, &argv);
  ierr = MPI_Comm_size(MPI_COMM_WORLD, &num_procs);
  ierr = MPI_Comm_rank(MPI_COMM_WORLD, &rank);

  // Store start time and end time
  double tic, toc;
  // Store my_bcast time and standard library time
  double my_time = 0.0, mpi_time = 0.0;
  int * array;
  int array_length = 1000000;
  int send_times = 1000;
  // allocate array with full length
  array = (int*)malloc(sizeof(int) * array_length);

  for (i = 0; i < send_times; i++)
  {
    // run our own implementation of Bcast
    MPI_Barrier(MPI_COMM_WORLD);
    tic = MPI_Wtime();
    my_bcast(array, array_length, MPI_INT, 0, MPI_COMM_WORLD);
    MPI_Barrier(MPI_COMM_WORLD);
    toc = MPI_Wtime();
    my_time += toc - tic;

    // run MPI_Bcast.
    MPI_Barrier(MPI_COMM_WORLD);
    tic = MPI_Wtime();
    MPI_Bcast(array, array_length, MPI_INT, 0, MPI_COMM_WORLD);
    MPI_Barrier(MPI_COMM_WORLD);
    toc = MPI_Wtime();
    mpi_time += toc - tic;

  }

  if (rank == 0)
  {
    printf("my_bcast time = %lf\n", my_time);
    printf("MPI_Bcast time = %lf\n", mpi_time);
  }

  MPI_Finalize();
  return 0;

}

