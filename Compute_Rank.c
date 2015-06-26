// Compute_Rank.c
//

#include <stdlib.h>
#include <stdio.h>
#include <mpi.h>
#include <string.h>

typedef struct {
  int rank;
  union {
    float fl;
    int in;
  } rankValue;
} RankObj; // Why it is out of the scope of struct

// gathers all rank values to the zero'th thread
void * gather_rankValues(void *rankValue, MPI_Datatype datatype, MPI_Comm comm)
{
  int myrank, mysize;
  MPI_Comm_rank(comm, &myrank);
  MPI_Comm_size(comm, &mysize);

  int datatype_size;
  MPI_Type_size(datatype, &datatype_size);
  void * gathered_rankValues;
  if (myrank == 0)
    gathered_rankValues = malloc(datatype_size * mysize);
  // Gathers together values from a group of processes
  // int MPI_Gather(const void *sendbuf, int sendcount, MPI_Datatype sendtype, void *recvbuf, int recvcount,
  // MPI_Datatype recvtype, int root, MPI_Comm comm).
  MPI_Gather(rankValue, 1, datatype, gathered_rankValues, 1, datatype, 0, comm);
  return gathered_rankValues;
}

// compare rank values in case we are dealing with float weights.
int compare_float_rankValues(const void *a, const void *b)
{
  RankObj * rank_value_a = (RankObj *) a;
  RankObj * rank_value_b = (RankObj *) b;

  if (rank_value_a->rankValue.fl < rank_value_b->rankValue.fl)
  {
    return -1;
  }
  else if (rank_value_a->rankValue.fl < rank_value_b->rankValue.fl)
  {
    return 1;
  }
  else
    return 0;
}

// compare rank values in case we are dealing with integer weights.
int compare_int_rankValues(const void *a, const void *b)
{
  RankObj * rank_value_a = (RankObj *) a;
  RankObj * rank_value_b = (RankObj *) b;

  if (rank_value_a->rankValue.in < rank_value_b->rankValue.in)
  {
    return -1;
  }
  else if (rank_value_a->rankValue.in < rank_value_b->rankValue.in)
  {
    return 1;
  }
  else
    return 0;
}

// sorts all the rankValues
//
int * sort_ranks_according_to_values(void * gather_rankValues, int gathered_rankValues_num, MPI_Datatype datatype)
{
  int i;
  int datatype_size;
  MPI_Type_size(datatype, &datatype_size);
  RankObj * rank_values = malloc(sizeof(RankObj) * gathered_rankValues_num);

  for (i = 0; i < gathered_rankValues_num; i++)
  {
    rank_values[i].rank = i;
    memcpy(&(rank_values[i].rankValue), gathered_rankValues + (i * datatype_size), datatype_size);
  }

  if (datatype == MPI_FLOAT)
  {
    qsort(rank_values, gathered_rankValues_num, sizeof(RankObj), &compare_float_rankValues);
  }
  else
  {
    qsort(rank_values, gathered_rankValues_num, sizeof(RankObj), &compare_float_rankValues);
  }

  int *ranks = (int *) malloc(sizeof(int) * gathered_rankValues_num);
  for (i = 0; i < gathered_rankValues_num; i++)
  {
    ranks[rank_values[i].rank] = i;
  }

  free(rank_values);
  return ranks;
}

MPI_Rank(void * send_data, void * recv_data, MPI_Datatype datatype, MPI_Comm comm)
{
  if (datatype != MPI_INT && datatype != MPI_FLOAT)
  {
    return MPI_ERR_TYPE;
  }

  int mysize, myrank;
  MPI_Comm_size(comm, &mysize);
  MPI_Comm_rank(comm, &myrank);

  // To calculate the rank, we must gather the numbers to one process, sort the numbers, and then
  // scatter the resulting rank values. Start by gathering the numbers on process 0 of comm.
  void *gathered_rankValues = gather_rankValues(send_data, datatype, comm);

  // Get the ranks of each process
  int *ranks = NULL;
  if (comm_rank == 0)
  {
    ranks = get_ranks(gathered_numbers, comm_size, datatype);
  }

  // Scatter the rank resutls
  // Send data from one process to all other processes in a communicator
  // int MPI_Scatter(const void *sendbuf, int sendcount, MPI_Datatype sendtype, void *recvbuf, int recvcount,
  // MPI_Datatype recvtype, int root, MPI_Comm comm)
  MPI_Scatter(ranks, 1, MPI_INT, recv_data, 1, MPI_INT, 0, comm);

  // Do clean up
  if (comm_rank == 0)
  {
    free(gathered_numbers);
    free(ranks);
  }
}

int main(int argc, char *argv[])
{
  int task, num_procs, ierr;

  int * how_many2, * displacement2, my_Recv_Size;

  MPI_Init(&argc, &argv);
  ierr = MPI_Comm_size(MPI_COMM_WORLD, &num_procs);
  ierr = MPI_Comm_rank(MPI_COMM_WORLD, &task);

  MPI_Finalize();
  return 0;
}


