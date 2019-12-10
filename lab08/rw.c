#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#define N 10
#define M 4

pthread_mutex_t mutex;
pthread_cond_t c_leitor, c_escritor;
int leitores = 0, escritores = 0;

struct {
	int cont;
	int id;
} conteudo;

void entraEscrita(int tid) {
	pthread_mutex_lock(&mutex);
	while(leitores || escritores){
		pthread_cond_wait(&c_escritor, &mutex);
	}
	escritores++;
	printf("Escritor %d entrou; escritores: %d\n", tid, escritores);
	pthread_mutex_unlock(&mutex);
}

void saiEscrita(int tid) {
	pthread_mutex_lock(&mutex);
	escritores--;
	pthread_cond_signal(&c_escritor);
	pthread_cond_broadcast(&c_leitor);
	printf("Escritor %d saiu; escritores: %d\n", tid, escritores);
	pthread_mutex_unlock(&mutex);
}

void *escr(void *arg) {
	int tid = * (int *) arg;
	int i = 5;

	while(i) {
		entraEscrita(tid);
		conteudo.cont++;
		conteudo.id = tid;
		printf("Escritor escreveu c_id %d; c_cont %d\n", conteudo.id, conteudo.cont);
		saiEscrita(tid);
		i--;
	}

	free(arg);
	pthread_exit(NULL);
}

void entraLeitura(int id) {
	pthread_mutex_lock(&mutex);
	while(escritores) {
		pthread_cond_wait(&c_leitor, &mutex);
	}
	leitores++;
	printf("Thread %d entrou; leitores: %d\n", id, leitores);
	pthread_mutex_unlock(&mutex);
}

void saiLeitura(int id) {
	pthread_mutex_lock(&mutex);
	leitores--;
	if(!leitores) pthread_cond_signal(&c_escritor);
	printf("Thread%d saiu; leitores: %d\n", id, leitores);
	pthread_mutex_unlock(&mutex);
}

void *leit(void *arg) {
	int id = * (int *) arg;
	int i, j, c_id, c_cont;

	j = 5;
	while(j) {
		entraLeitura(id);
		c_id = conteudo.id;
		c_cont = conteudo.cont;
		for(i = 0; i < 300; i++) {
			c_id++;
			c_id--;
			c_cont++;
			c_cont--;
		}
		printf("Leitor %d; c_id %d; c_cont %d\n", id, c_id, c_cont);
		saiLeitura(id);
		j--;
	}

	free(arg);
	pthread_exit(NULL);
}

int main(int argc, char *argv[]) {
	pthread_t sid_leitoras[N], sid_escritoras[M];
	int *tid;
	int t;

	pthread_mutex_init(&mutex, NULL);
	pthread_cond_init(&c_leitor, NULL);
	pthread_cond_init(&c_escritor, NULL);

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
