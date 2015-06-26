#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <mpi.h>
#include <string.h>

#define ITER 200
#define CX -0.74543
#define CY 0.11301
#define XMIN -1.5
#define XMAX 1.5
#define YMIN -1.5
#define YMAX 1.5
#define NX 2000
#define NY 2000

float move_point_count(float, float);
int save_set(char *, float *);

int main(int argc, char *argv[])
{
  char filename[20] = "./julia_set.dat";
  float res_x, res_y, x, y;
  float XMIN_lcl;
  double starttime, endtime;
  float *points, *points_lcl;
  int i, j, k;
  int NX_lcl, num_procs, rank, ierr;

  MPI_Init(&argc, &argv);
  ierr = MPI_Comm_size(MPI_COMM_WORLD, &num_procs);
  ierr = MPI_Comm_rank(MPI_COMM_WORLD, &rank);

  res_x = (XMAX - XMIN) / NX;
  res_y = (YMAX - YMIN) / NY;

  NX_lcl = (int) NX / num_procs; // NX per MPI process
  XMIN_lcl = XMIN + (float)rank * NX_lcl * res_x;

  points_lcl = (float*) malloc(NX_lcl * NY * sizeof(float));

  if (rank == 0)
    points = (float*) malloc(NX * NY * sizeof(float));

  starttime = MPI_Wtime();
  for (i = 0; i < NX_lcl; i++)
  {
    for (j = 0; j < NY; j++)
    {
      x = XMIN_lcl + i * res_x;
      y = YMIN + j * res_y;
      k = i * NY + j;
      points_lcl[k] = move_point_count(x, y);
    }
  }
  endtime = MPI_Wtime();
  printf("Parallel version: on processor %d compute time is: %f\n", rank, endtime - starttime);

 //  MPI_Barrier(MPI_COMM_WORLD);
 //  printf("Thread %d is done\n", rank);

  MPI_Gather(points_lcl, NY * NX_lcl, MPI_FLOAT,
             points, NY * NX_lcl, MPI_FLOAT,
             0, MPI_COMM_WORLD);

  if (rank == 0)
  {
    starttime = MPI_Wtime();
    save_set(filename, points);
    endtime = MPI_Wtime();
    printf("Parallel version: write_to_time time is: %f\n", endtime - starttime);
    free(points);
  }
  free(points_lcl);

  MPI_Finalize();
  return 0;

}

// Method to count the iterations to move the point to a place more than 2 distance
float move_point_count(float x, float y)
{
  float dist = 0, x_next, y_next, temp;
  int i = 0;
  x_next = x;
  y_next = y;

  while (dist < 2 && i <= ITER)
  {
    temp = x_next * x_next - y_next * y_next + CX;
    y_next = 2 * x_next * y_next + CY;
    x_next = temp;
    dist = sqrt(x_next * x_next + y_next * y_next);
    i++;
  }
  return (float)i;
}

int save_set(char *filename, float *data)
{
  FILE *f;
  int k;
  float x, y, res_x, res_y;

  res_x = (XMAX - XMIN) / NX;
  res_y = (YMAX - YMIN) / NY;

  f = fopen(filename, "w");
  for (int i = 0; i < NX; i++)
  {
    for (int j = 0; j < NY; j++)
    {
      x = XMIN + i * res_x;
      y = YMIN + j * res_y;
      k = i * NY + j;
      fprintf(f, "%f %f %f\n", x, y, data[k]);
    }
  }

  fclose(f);
  return 0;

}


