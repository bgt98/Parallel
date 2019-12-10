#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#define NTHREADS 4

pthread_mutex_t mutex;
pthread_cond_t c_cond;

int count = 0;

void *A(void * arg) {
	printf("tudo bem?\n");

	pthread_mutex_lock(&mutex);
	count++;
	if(count >= 2){
		pthread_cond_broadcast(&c_cond);
	}
	pthread_mutex_unlock(&mutex);

	pthread_exit(NULL);
}
void *B(void * arg) {
	printf("hola!\n");

	pthread_mutex_lock(&mutex);
	count++;
	if(count >= 2){
		pthread_cond_broadcast(&c_cond);
	}
	pthread_mutex_unlock(&mutex);

	pthread_exit(NULL);
}
void *C(void * arg) {
	pthread_mutex_lock(&mutex);
	while(count < 2) {
		pthread_cond_wait(&c_cond, &mutex);
	}
	pthread_mutex_unlock(&mutex);
	printf("até mais tarde.\n");

	pthread_exit(NULL);
}
void *D(void * arg) {
	pthread_mutex_lock(&mutex);
	while(count < 2) {
		pthread_cond_wait(&c_cond, &mutex);
	}
	pthread_mutex_unlock(&mutex);
	printf("tchau!\n");

	pthread_exit(NULL);
}

int main(int argc, char *argv[])
{
	int t;
	pthread_t sys_tids[NTHREADS];

	pthread_mutex_init(&mutex, NULL);
	pthread_cond_init(&c_cond, NULL);

	pthread_create(&sys_tids[3], NULL, D, NULL);
	pthread_create(&sys_tids[2], NULL, C, NULL);
	pthread_create(&sys_tids[0], NULL, A, NULL);
	pthread_create(&sys_tids[1], NULL, B, NULL);

	for (t = 0; t < NTHREADS; ++t) {
		pthread_join(sys_tids[t], NULL);
	}

	pthread_mutex_destroy(&mutex);
	pthread_cond_destroy(&c_cond);
	pthread_exit(NULL);
}
