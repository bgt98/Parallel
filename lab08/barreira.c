#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#define NTHREADS 5
#define PASSOS 5

pthread_mutex_t mutex;
pthread_cond_t b_cond;

void barreira(int nthreads) {
	static int cont_barreira = 0;

	pthread_mutex_lock(&mutex);
	cont_barreira++;
	if(cont_barreira != nthreads) {
		pthread_cond_wait(&b_cond, &mutex);
	} else {
		cont_barreira = 0;
		pthread_cond_broadcast(&b_cond);
	}
	pthread_mutex_unlock(&mutex);
}

void *A(void *arg) {
	int tid = * (int *) arg, i;
	int cont = 0, boba1, boba2;

	for (i = 0; i < PASSOS; i++) {
		cont++;
		printf("Thread %d: cont=%d, passo=%d\n", tid, cont, i);

		// sincronização condicional
		barreira(NTHREADS);

		// operações para gastar tempo
		boba1 = 100; boba2 = -100; while(boba2 < boba1) boba2++;
	}

	free(arg);
	pthread_exit(NULL);
}

int main(int argc, char *argv[])
{
	pthread_t threads[NTHREADS];
	int *tid;
	int t;

	pthread_mutex_init(&mutex, NULL);
	pthread_cond_init(&b_cond, NULL);

	for (t = 0; t < NTHREADS; t++) {
		tid = malloc(sizeof(int));
		*tid = t;

		if(pthread_create(&threads[t], NULL, A, (void *) tid)) {
			fprintf(stderr, "Falha na criação de threads.\n");
			exit(-1);
		}
	}

	for(t = 0; t < NTHREADS; t++) {
		if(pthread_join(threads[t], NULL)) exit(-1);
	}

	pthread_exit(NULL);
}
