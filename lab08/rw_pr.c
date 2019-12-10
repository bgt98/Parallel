#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

pthread_mutex_t mutex;
pthread_cond_t c_leitor, c_escritor;
int N, M;
int leitores = 0, escritores = 0, fila = 0;

struct {
	int cont;
	int id;
} conteudo;

void entraEscrita(int tid) {
	pthread_mutex_lock(&mutex);
  fila++;
	while(leitores || escritores){
		pthread_cond_wait(&c_escritor, &mutex);
	}
  fila--;
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
	while(escritores || fila) {
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
	pthread_t *sid_leitoras, *sid_escritoras;
	int *tid;
	int t;

  if(argc < 3) {
    fprintf(stderr, "Use o formato: %s <nº de escritoras> <nº de leitoras>\n", argv[0]);
    exit(-1);
  }

  M = atoi(argv[1]);
  N = atoi(argv[2]);

  sid_leitoras = (pthread_t *) malloc(sizeof(pthread_t) * N);
  sid_escritoras = (pthread_t *) malloc(sizeof(pthread_t) * M);

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
