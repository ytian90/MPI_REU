#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <mpi.h>

int main(int argc, char *argv[])
{
  int num_procs, rank, ierr, dest, src, tag = 1;
  char outmsg[20] = "dummy_out", inmsg[20] = "dummy_in";
  MPI_Status status;

  MPI_Init(&argc, &argv);
  /* Determines the size of the group associated with a communicator.  */
  ierr = MPI_Comm_size(MPI_COMM_WORLD, &num_procs); 
  /* Determines the rank of the calling process in the communicator.  */
  ierr = MPI_Comm_rank(MPI_COMM_WORLD, &rank); 

  if (rank == 0) // processor 0
  {
    dest = 1; 
    src = 1; 
    strcpy(outmsg, "HelloFrom0"); 
    // Performs a blocking send.
    // int MPI_Send(const void *buf, int count, MPI_Datatype datatype, int dest, int tag, MPI_Comm comm)
    ierr = MPI_Send(&outmsg, 10, MPI_CHAR, dest, tag, MPI_COMM_WORLD); 
    // Blocking receive for a message. 
    // int MPI_Recv(void *buf, int count, MPI_Datatype datatype, int source, int tag, MPI_Comm comm, MPI_Status *status)
    ierr = MPI_Recv(&inmsg, 10, MPI_CHAR, src, tag, MPI_COMM_WORLD, &status); 
  }
  else if (rank == 1) // processor 1
  {
    dest = 0; 
    src = 0; 
    strcpy(outmsg, "HelloFrom1"); 
    ierr = MPI_Send(&outmsg, 10, MPI_CHAR, dest, tag, MPI_COMM_WORLD); 
    ierr = MPI_Recv(&inmsg, 10, MPI_CHAR, src, tag, MPI_COMM_WORLD, &status);
  }

  printf("I am %2d: my inmsg is \"%s\"\n", rank, inmsg); 

  MPI_Finalize(); 
  return 0; 

}

