#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>

sem_t condCD;
sem_t em;

int cont = 0;

void *A(void *arg) {
  printf("olá, tudo bem?\n");
  sem_wait(&em);
  cont++;
  if (cont == 2) {
    sem_post(&condCD);
    sem_post(&condCD);
  }
  sem_post(&em);

  pthread_exit(NULL);
}

void *B(void *arg) {
  printf("hello!\n");
  sem_wait(&em);
  cont++;
  if (cont == 2) {
    sem_post(&condCD);
    sem_post(&condCD);
  }
  sem_post(&em);

  pthread_exit(NULL);
}

void *C(void *arg) {
  sem_wait(&condCD);
  printf("até mais tarde.\n");

  pthread_exit(NULL);
}

void *D(void *arg) {
  sem_wait(&condCD);
  printf("tchau!\n");

  pthread_exit(NULL);
}

int main(void) {
  pthread_t sys_tids[4];
  int i;

  sem_init(&condCD, 0, 0);
  sem_init(&em, 0, 1);

  pthread_create(&sys_tids[0], NULL, A, NULL);
  pthread_create(&sys_tids[1], NULL, B, NULL);
  pthread_create(&sys_tids[2], NULL, C, NULL);
  pthread_create(&sys_tids[3], NULL, D, NULL);

  for (i = 0; i < 4; i++) pthread_join(sys_tids[i], NULL);

  pthread_exit(NULL);
}
