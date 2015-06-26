#include <mpi.h>
#include <stdio.h>

int main(int argc, char *argv[])
{
  int num_procs, my_rank, len;
  int if_initialized, ierr;
  int ver, subver;

  double start, stop;

  char MPI_Proc_Name[MPI_MAX_PROCESSOR_NAME];

  MPI_Init(&argc, &argv);
  start = MPI_Wtime();
  ierr = MPI_Comm_size(MPI_COMM_WORLD, &num_procs);
  ierr = MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);
  ierr = MPI_Get_processor_name(MPI_Proc_Name, &len);
  ierr = MPI_Initialized(&if_initialized);
  stop = MPI_Wtime();

  printf("Hello from rank %d out of %d, I am on processor %s, if_init is %d\n", my_rank, num_procs, MPI_Proc_Name, if_initialized);
  MPI_Barrier(MPI_COMM_WORLD);
  printf("on processor %d elapsed time is: %f\n", my_rank, stop - start);
  MPI_Barrier(MPI_COMM_WORLD);

  if (my_rank == 0)
  {
    printf("-----------------------\n");
    ierr = MPI_Get_version(&ver, &subver);
    printf("MPI Version is %d.%d\n", ver, subver);
  }

  MPI_Finalize();
  return 0;

}
