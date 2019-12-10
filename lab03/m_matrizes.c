#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include "timer.h"

// global variables
int nThreads;
float *matA, *matB, *matAB;
int lines_A, columns_A, lines_B, columns_B;

// function to allocate matrix
int load_matrix(float **mat, int lin, int col, FILE *fil) {
  int i, j;

  *mat = (float *) malloc (sizeof(float) * lin * col);
  if (!mat) return 0;

  for (i = 0; i < lin; i++) {
    for (j = 0; j < col; j++) {
      fscanf(fil, "%f", (*mat) + (i*col + j));
    }
  }
  return 1;
}

// function to print matrix to file
void print_matrix(float *mat, int lin, int col, FILE* fil) {
  int i, j;
  for (i = 0; i < lin; i++) {
    for (j = 0; j < col; j++) {
      fprintf(fil, "%.1f ", mat[i * col + j]);
    }
    fprintf(fil, "\n");
  }
}

// function for thread execution
void *matrix_product(void *tid) {
  int id = * (int *) tid;
  int j, k;

  for ( ; id < lines_A; id += nThreads) {
    for (j = 0; j < columns_B; j++) {
      matAB[id*columns_B+j] = 0;
      for (k = 0; k < lines_B; k++) {
	matAB[id*columns_B+j] += matA[id*columns_A+k] * matB[k*columns_B+j];
      }
    }
  }

  free(tid);
  pthread_exit(NULL);
}

int main(int argc, char *argv[])
{
  FILE *matAB_file = stdout;
  FILE *matA_file;
  FILE *matB_file;

  pthread_t *sys_tid;
  int *local_tid;

  int t, c;

  double start, end, time1, time2, time3;

  GET_TIME(start);
  nThreads = sysconf(_SC_NPROCESSORS_ONLN);

  // options processing
  opterr = 0;
  while ((c = getopt(argc, argv, "n:o:")) != -1) {
    switch(c) {
    case 'n':
      nThreads = atoi(optarg);
      if (nThreads <= 0) {
	fprintf(stderr ,"Invalid option: -n must receive a non-zero positive integer.\n");
	exit(-1);
      }
      break;

    case 'o':
      matAB_file = fopen(optarg, "w");
      if(!matAB_file) {
	fprintf(stderr, "Failure to open output file.\n");
	exit(-1);
      }
      break;

    case '?':
      if (optopt == 'n' || optopt == 'o')
	fprintf(stderr, "Option -%c requires an argument.\n", optopt);
      else if (isprint(optopt))
	fprintf(stderr, "Unknown option '-%c'.\n", optopt);
      else
	fprintf(stderr, "Unkown option character '\\x%x'.\n", optopt);

      return 1;
    default:
      abort();
    }
  }

  // arguments processing
  if ((argc - optind) < 2) {
    fprintf(stderr, "Missing non-optional arguments.\n");
    exit(1);
  }

  // open input files
  matA_file = fopen(argv[optind], "r");
  if (!matA_file) {
    fprintf(stderr, "Failure to open first matrix file.\n");
    exit(-1);
  }

  matB_file = fopen(argv[optind+1], "r");
  if (!matB_file) {
    fprintf(stderr, "Failure to open second matrix file.\n");
    exit(-1);
  }

  // matrix A dimensions
  fscanf(matA_file, "%d", &lines_A);
  fscanf(matA_file, "%d", &columns_A);

  // matrix B dimensions
  fscanf(matB_file, "%d", &lines_B);
  fscanf(matB_file, "%d", &columns_B);

  // check if the product is possible
  if (columns_A != lines_B) {
    fprintf(stderr, "The dimensions are not compatible.\n");
    exit(-1);
  }

  // load input matrices
  if(!load_matrix(&matA, lines_A, columns_A, matA_file)) {
    fprintf(stderr, "Failure to load first matrix.\n");
    exit(-1);
  }

  if(!load_matrix(&matB, lines_B, columns_B, matB_file)) {
    fprintf(stderr, "Failure to load second matrix.\n");
    exit(-1);
  }

  // allocate product matrix AB
  matAB = (float *) malloc(sizeof(float) * lines_A * columns_B);
  if(!matAB) {
    fprintf(stderr, "Failure to allocate memory for product matrix.\n");
    exit(-1);
  }
  GET_TIME(end);
  time1 = end - start;

  GET_TIME(start);
  // thread multiplication
  sys_tid = (pthread_t *) malloc(sizeof(pthread_t) * nThreads);
  if (!sys_tid) {
    fprintf(stderr, "Failure to allocate memory for array of threads' internal ids.\n");
    exit(-1);
  }
  for (t = 0; t < nThreads; t++) {
    local_tid = (int *) malloc(sizeof(int));
    if(!local_tid) {
      fprintf(stderr, "Failure to allocate memory for local thread id.\n");
      exit(-1);
    }
    *local_tid = t;

    if(pthread_create(&sys_tid[t], NULL, matrix_product, (void *) local_tid)) {
      fprintf(stderr, "Failure to create thread: %d\n", t);
      exit(-1);
    }
  }
  for (t = 0; t < nThreads; t++) {
    if(pthread_join(sys_tid[t], NULL)) {
      fprintf(stderr, "Failure to join thread: %d", t);
      exit(-1);
    }
  }
  GET_TIME(end);
  time2 = end - start;

  GET_TIME(start);
  // print product matrix AB to file
  print_matrix(matAB, lines_A, columns_B, matAB_file);

  free(sys_tid);
  free(matA);
  free(matB);
  free(matAB);
  GET_TIME(end);
  time3 = end - start;


  printf("Time in initializations: %.8lf\n", time1);
  printf("Time in matrix product with  %d threads: %.8lf\n", nThreads, time2);
  printf("Time in cleaning up: %.8lf\n", time3);
  pthread_exit(NULL);
}

