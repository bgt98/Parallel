#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include "timer.h"

// global variables
float *matA;
float *vectX;
float *vectB;
int lines, columns, size, nThreads;

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

// function to load vector
int load_vector(float **vector, int siz, FILE *fil) {
  int i;

  *vector = (float *) malloc (sizeof(float) * siz);
  if (!vector) return 0;

  for (i = 0; i < siz; i++) {
    fscanf(fil, "%f", *vector + i);
  }
  return 1;
}

// function to print vector to file
void print_vector(float *vector, int siz, FILE *fil) {
  int i;
  for (i = 0; i < siz; i++) {
    fprintf(fil, "%.1f ", vector[i]);
  }
  fprintf(fil, "\n");
}

// function for thread execution
void *multiply(void * tid) {
  int id = * (int *) tid;
  int i;

  for ( ; id < lines; id += nThreads) {
    vectB[id] = 0;
    for (i = 0; i < columns; i++) {
      vectB[id] += matA[id * columns + i] * vectX[i];
    }
  }

  free(tid);
  pthread_exit(NULL);
}

int main(int argc, char *argv[]) {
  FILE *matFileA;
  FILE *vectFileX;
  FILE *vectFileB;
  double start, end, time1, time2, time3;
  int t;
  int *local_tid;
  pthread_t *system_tid;

  GET_TIME(start);

  // evaluates the number of arguments
  if (argc < 5) {
    printf("Insufficient number of arguments.\n");
    return 0;
  }

  // gets the number of threads
  nThreads = atoi(argv[1]);

  // open the file A
  matFileA = fopen(argv[2], "r");
  if(!matFileA) {
    printf("Failure to open matrix A file.\n");
    exit(-1);
  }

  //open the file X
  vectFileX = fopen(argv[3], "r");
  if(!vectFileX) {
    printf("Failure to open vector X file.\n");
    exit(-1);
  }

  // open the file B
  vectFileB = fopen(argv[4], "w");
  if(!vectFileB) {
    printf("Failure to open vector B file.\n");
    exit(-1);
  }

  // matriz dimensions
  fscanf(matFileA, "%d", &lines);
  fscanf(matFileA, "%d", &columns);

  // vector dimension
  fscanf(vectFileX, "%d", &size);

  // check if the dimensions are valid
  if (columns != size) {
    fprintf(stderr, "Error: the dimensions of the matrix and the vector are not compatible.\n");
    exit(-1);
  }

  // load matrix and vector
  if(!load_matrix(&matA, lines, columns, matFileA)) {
    printf("Failure to load matrix A.\n");
    exit(-1);
  }

  if(!load_vector(&vectX, size, vectFileX)) {
    printf("Failure to load vector X.\n");
    exit(-1);
  }

  // allocate the vector for answer
  vectB = (float *) malloc(sizeof(float) * lines);
  if(!vectB) {
    fprintf(stderr, "Error: failure to allocate vector.\n");
    exit(-1);
  }

  GET_TIME(end);
  time1 = end - start;

  GET_TIME(start);
  // thread multiplication
  system_tid = (pthread_t *) malloc(sizeof(pthread_t) * nThreads); if(!system_tid) exit(-1);
  for (t = 0; t < nThreads; t++) {
    local_tid = (int *) malloc(sizeof(int)); if(!local_tid) exit(-1);
    *local_tid = t;

    if(pthread_create(&system_tid[t], NULL, multiply, (void *) local_tid)) exit(-1);
  }

  for (t = 0; t < nThreads; t++) {
    if(pthread_join(system_tid[t], NULL)) exit(-1);
  }

  GET_TIME(end);
  time2 = end - start;

  GET_TIME(start);
  // print vector to file
  print_vector(vectB, size, vectFileB);

  free(system_tid);
  free(matA);
  free(vectX);
  free(vectB);
  GET_TIME(end);
  time3 = end - start;

  // show time measurements
  printf("Time used to initialize: %.8lf\n", time1);
  printf("Time used to multiply with %d threads: %.8lf\n", nThreads, time2);
  printf("Time used to clean up: %.8lf\n", time3);

  pthread_exit(NULL);
}
