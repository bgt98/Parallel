#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>

#define N 5
#define P 4
#define C 4

sem_t cond_in, cond_rem;
sem_t em_in, em_out;

int buffer[N];

void insere(int item) {
  static int in = 0;
  sem_wait(&cond_in);
  sem_wait(&em_in);
  buffer[in] = item;
  in = (in + 1) % N;
  printf("P[%d] inseriu; buffer: ", item);
  printf("%d # %d # %d # %d # %d\n", buffer[0], buffer[1], buffer[2], buffer[3], buffer[4]);
  sem_post(&em_in);
  sem_post(&cond_rem);
}

int retira(int id) {
  static int out = 0;
  int item;
  sem_wait(&cond_rem);
  sem_wait(&em_out);
  item = buffer[out];
  buffer[out] = -1;
  out = (out + 1) % N;
  printf("C[%d] removeu; buffer: ", id);
  printf("%d # %d # %d # %d # %d\n", buffer[0], buffer[1], buffer[2], buffer[3],
         buffer[4]);
  sem_post(&em_out);
  sem_post(&cond_in);
  return item;
}

void *consumidor(void *arg) {
  int item, boba = 0, i;
  int tid = * (int *) arg;

  while(1) {
    item = retira(tid);
    for (i = 0; i < 10000000; i++) {
      boba++;
      item++;
      boba--;
      item--;
    }
  }

  free(arg);
  pthread_exit(NULL);
}
void *produtor(void *arg) {
  int tid = * (int *) arg;
  int i, boba1 = 0, boba2 = 0;

  while (1) {
    insere(tid);
    for (i = 0; i < 10000000; i++) {
      boba1++;
      boba2++;
      boba1--;
      boba2--;
    }
  }

  free(arg);
  pthread_exit(NULL);
}

int main(void) {
  pthread_t id_prod[P];
  pthread_t id_cons[C];
  int *tid;
  int i = 0;

  sem_init(&cond_in, 0, N);
  sem_init(&cond_rem, 0, 0);
  sem_init(&em_in, 0, 1);
  sem_init(&em_out, 0, 1);

  for (i = 0; i < N; i++) buffer[i] = -1;

  for (i = 0; i < C; i++) {
    tid = malloc(sizeof(int));
    *tid = i;
    pthread_create(&id_cons[i], NULL, consumidor, (void *) tid);
  }
  for (i = 0; i < P; i++) {
    tid = malloc(sizeof(int));
    *tid = i;
    pthread_create(&id_prod[i], NULL, produtor, (void *) tid);
  }

  pthread_exit(NULL);
}

