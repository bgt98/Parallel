#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#define N 4
#define M 4
#define SIZE 10

pthread_mutex_t mutex;
pthread_cond_t c_leitor, c_escritor;
int leitores = 0, escritores = 0;
int vetor[SIZE];

void entraEscrita(int tid) {
	pthread_mutex_lock(&mutex);
	while(leitores || escritores){
		pthread_cond_wait(&c_escritor, &mutex);
	}
	escritores++;
	//printf("Escritor %d entrou; escritores: %d\n", tid, escritores);
	pthread_mutex_unlock(&mutex);
}

void saiEscrita(int tid) {
	pthread_mutex_lock(&mutex);
	escritores--;
	pthread_cond_signal(&c_escritor);
	pthread_cond_broadcast(&c_leitor);
	//printf("Escritor %d saiu; escritores: %d\n", tid, escritores);
	pthread_mutex_unlock(&mutex);
}

void *escr(void *arg) {
	int tid = * (int *) arg;
	int i, j;

	//j = 5;
	while(1) {
		//j--;
		entraEscrita(tid);
		vetor[0] = vetor[SIZE-1] = tid;
		for(i = 1; i < SIZE - 1; i++) vetor[i] = tid * 2;
		saiEscrita(tid);
	}

	free(arg);
	pthread_exit(NULL);
}

void entraLeitura(int id) {
	pthread_mutex_lock(&mutex);
	while(escritores) {
		printf("Leitor %d esperando\n", id);
		pthread_cond_wait(&c_leitor, &mutex);
	}
	leitores++;
	//printf("Leitor %d entrou; leitores: %d\n", id, leitores);
	pthread_mutex_unlock(&mutex);
}

void saiLeitura(int id) {
	pthread_mutex_lock(&mutex);
	leitores--;
	if(!leitores) pthread_cond_signal(&c_escritor);
	//printf("Leitor %d saiu; leitores: %d\n", id, leitores);
	pthread_mutex_unlock(&mutex);
}

void *leit(void *arg) {
	static pthread_mutex_t m_print = PTHREAD_MUTEX_INITIALIZER;
	int id = * (int *) arg;
	int i, j, acc = 0;
	int boba = 10000000, boba1 = 0;
	double media;

	//j = 5;
	while(1) {
		//j--;
		entraLeitura(id);
		for(i = 0; i < boba; i++) boba1++;
		for(i = 0; i < SIZE; i++) {
			acc += vetor[i];
		}
		media = (double) acc / SIZE;
		pthread_mutex_lock(&m_print);
		for(i = 0; i < SIZE; i++) {
			printf("%d - ", vetor[i]);
		}
		printf("%.3lf\n", media);
		pthread_mutex_unlock(&m_print);
		acc = 0;
		saiLeitura(id);
	}

	free(arg);
	pthread_exit(NULL);
}

int main(int argc, char *argv[]) {
	pthread_t sid_leitoras[N], sid_escritoras[M];
	int *tid;
	int i, t;

	pthread_mutex_init(&mutex, NULL);
	pthread_cond_init(&c_leitor, NULL);
	pthread_cond_init(&c_escritor, NULL);

	for(i = 0; i < SIZE; i++) vetor[i] = 0;

	for(t = 0; t < M; t++) {
		tid = malloc(sizeof(int));
		*tid = t;
		pthread_create(&sid_escritoras[t], NULL, escr, (void *) tid);
	}

	for(t = 0; t < N; t++) {
		tid = malloc(sizeof(int));
		*tid = t;
		pthread_create(&sid_leitoras[t], NULL, leit, (void *) tid);
	}

	for(t = 0; t < M; t++) pthread_join(sid_escritoras[t], NULL);
	for(t = 0; t < N; t++) pthread_join(sid_leitoras[t], NULL);

	pthread_mutex_destroy(&mutex);
	pthread_cond_destroy(&c_escritor);
	pthread_cond_destroy(&c_leitor);

	pthread_exit(NULL);
}
