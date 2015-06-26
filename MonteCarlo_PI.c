#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>
#include <math.h>

#define PI_KNOWN 3.14159265358979323846
#define N 1000000000

double compute_pi(int how_many);

int main(int argc, char *argv[])
{
  int in_points, N_local, i;
  int num_procs, rank, ierr;
  double my_pi, pi;

  MPI_Init(&argc, &argv);
  ierr = MPI_Comm_size(MPI_COMM_WORLD, &num_procs);
  ierr = MPI_Comm_rank(MPI_COMM_WORLD, &rank);

  srandom(rank);

  // N is the total number of trials, N_local is number of trials in each processor
  N_local = (int) N / num_procs;
  // Calculate the local pi
  my_pi = compute_pi(N_local);

  // Reduce values on all processes to a single value.
  // int MPI_Reduce(const void *sendbuf, void *recvbuf, int count, MPI_Datatype datatype,
  // MPI_Op op, int root, MPI_Comm comm)
  ierr = MPI_Reduce(&my_pi, &pi, 1, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);

  if (ierr != MPI_SUCCESS)
    printf("!!!ERROR!!!: MPI_Reduce() failure...\n"); 

  if (rank == 0)
  {
    pi = pi / num_procs;
    printf("PI = %f, error is %f\n", pi, pi - PI_KNOWN);
  }

  MPI_Finalize();
  return 0;

}
// Implementation of the comput_pi method
double compute_pi(int how_many)
{
  int in_points;
  int i;
  double x, y, r, pi;
  double INV_RAND_MAX = 1.0/(RAND_MAX);

  in_points = 0;
  for (i = 0; i < how_many; i++)
  {
    x = (double) rand() * INV_RAND_MAX;
    y = (double) rand() * INV_RAND_MAX;
    if ( x * x + y * y <= 1) in_points++;
  }
  pi = 4.0 * in_points / how_many;
  return pi;

}
