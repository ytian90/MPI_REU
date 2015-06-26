#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include "mpi.h"

//#define N = 10000000;

double f(double);
double integrate(double, double, int);

int main(int argc, char *argv[])
{
  int N = 1000000;
  int N_lcl, num_procs, rank, ierr;
  double x0 = 0, x1 = 1, x0_lcl;
  double dx = (x1 - x0) / N;
  double integral, integral_lcl;

  MPI_Init(&argc, &argv);
  ierr = MPI_Comm_size(MPI_COMM_WORLD, &num_procs);
  ierr = MPI_Comm_rank(MPI_COMM_WORLD, &rank);

  // Average assignments to each processor
  x0_lcl = x0 + rank * (x1 - x0) / num_procs;
  // Average N to each processor
  N_lcl = N / num_procs;

  integral_lcl = integrate(x0_lcl, dx, N_lcl);
  printf("%2d: value of the integral is %10.15f\n", rank, integral_lcl);

  // Reduces values on all processes to a single value
  // int MPI_Reduce(const void *sendbuf, void *recvbuf, int count, MPI_Datatype datatype,
  // MPI_Op op, int root, MPI_Comm comm)
  ierr = MPI_Reduce(&integral_lcl, &integral, 1, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);

  if (ierr != MPI_SUCCESS)
    printf("!!!ERROR!!!: MPI_Reduce() failure...\n");

  if (rank == 0)
    printf("final value of the integral is ---> %10.15f\n", integral);

  MPI_Finalize();
  return 0;

}

// function f
double f(double x)
{
  return sin(x) * cos(x * 4.0) * sqrt(x * x+ 1) / (2 * x * x + 1);
}

// function to calculate integral
// param: start x, dx, how_many_times
double integrate(double x0, double dx, int n_iter)
{
  double integral, x;

  integral = 0;

  for (int i = 0; i < n_iter; i++)
  {
    x = x0 + dx * i;
    integral += f(x) * dx;
  }

  return integral;
}

