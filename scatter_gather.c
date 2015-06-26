#include <stdlib.h>
#include <stdio.h>
#include "mpi.h"

int main(int argc, char *argv[])
{
  int task, num_procs, ierr;
  int *global1, *local1;
  char *global2, *local2;
  int i, root = 0;

  int *how_many2, *displacement2, my_Recv_Size;

  MPI_Init(&argc, &argv);
  ierr = MPI_Comm_size(MPI_COMM_WORLD, &num_procs);
  ierr = MPI_Comm_rank(MPI_COMM_WORLD, &task);

  if (task == 0)
    printf("============Task 1==============\n");
  if (task == root)
  {
    global1 = malloc(8 * sizeof(int));
    for (i = 0; i < 8; i++)
      global1[i] = i;
    printf("On task %d global1 = [", task);
    for (i = 0; i < 8; i++)
      printf("%d ", global1[i]);
    printf("]\n");
  }

  local1 = malloc(2 * sizeof(int));

  MPI_Scatter(global1, 2, MPI_INT, // send everyone 2 ints
              local1, 2, MPI_INT, // everyone receives 2 ints
              root, MPI_COMM_WORLD); // sending process is root

  local1[0] += 100;
  local1[1] += 100;

  MPI_Gather(local1, 2, MPI_INT,
            global1, 2, MPI_INT,
            root, MPI_COMM_WORLD);

  if (task == root)
  {
    printf("After Scatter\\Gather:\n");
    printf("On task %d global1 = [", task);
    for (i = 0; i < 8; i++)
      printf(" %d", global1[i]);
    printf("]\n");
  }


  // TASK 2
  MPI_Barrier(MPI_COMM_WORLD);
  if (task == root)
    printf("==============Task 2===============\n");

  if (task == root)
  {
    global2 = malloc(9 * sizeof(char));
    for (i = 0; i < 9; i++)
      global2[i] = 'a' + i;
    printf("On task %d global2 = [", task);
    for (i = 0; i < 9; i++)
      printf("%c ", global2[i]);
    printf("]\n");
  }

  how_many2 = malloc(num_procs * sizeof(int));
  displacement2 = malloc(num_procs * sizeof(int));
  local2 = malloc(sizeof(char) * 3);

  how_many2[0] = 2;
  how_many2[1] = 2;
  how_many2[2] = 2;
  how_many2[3] = 3;
  displacement2[0] = 0;
  displacement2[1] = 2;
  displacement2[2] = 4;
  displacement2[3] = 6;

  my_Recv_Size = how_many2[task];
  // Scatters a buffer in parts to all processes in a communicator
  // int MPI_Scatterve(const void *sendbuf, const int *sendcounts, const int *displs,
  // MPI_Datatype sendtype, void *recvbuf, int recvcount, int root, MPI_Comm comm)
  MPI_Scatterv(global2, how_many2, displacement2, MPI_CHAR,
              local2, my_Recv_Size, MPI_CHAR,
              root, MPI_COMM_WORLD);

  local2[0] += task;
  local2[1] += task;
  local2[2] += task;

  MPI_Gatherv(local2, my_Recv_Size, MPI_CHAR,
              global2, how_many2, displacement2, MPI_CHAR,
              root, MPI_COMM_WORLD);

  if (task == root)
  {
    printf("After Scatter\\Gather:\n");
    printf("On task %d global2 = [", task);
    for (i = 0; i < 9; i++)
      printf("%c ", global2[i]);
    printf("]\n");
  }

  MPI_Barrier(MPI_COMM_WORLD);
  if (task == root)
    printf("\n\n\nFinished.....\n");

  MPI_Barrier(MPI_COMM_WORLD);
  if (task == root)
  {
    free(global1);
    free(global2);
  }

  free(how_many2);
  free(displacement2);
  free(local1);
  free(local2);
  MPI_Finalize();
  return 0;

}
