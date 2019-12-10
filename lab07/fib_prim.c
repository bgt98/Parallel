#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <math.h>

#define SIZE 5
#define FIB 25

int buffer[SIZE];
int count=0, in=0, out=0, total=0;
pthread_mutex_t mutex, m_cons;
pthread_cond_t cond_cons, cond_prod;

int ehPrimo(long unsigned int n) {
	int i;
	if(n <= 1) return 0;
	if(n == 2) return 1;
	if(n % 2 == 0) return 0;
	for(i = 3; i < sqrt(n) + 1; i += 2) {
		if (n % i == 0) return 0;
	}
	return 1;
}

int fib() {
	static int f1 = 1, f2 = 1;
	static int index = 1;
	int f = 1;

	switch(index) {
		case 1:
			f = 1;
			index++;
			break;
		case 2:
			f = 1;
			index++;
			break;
		default:
			f = f2 + f1;
			f1 = f2;
			f2 = f;
			index++;
			break;
	}
	return f;
}

void insere(int n) {
	pthread_mutex_lock(&mutex);
	//printf("Thread produtora, contador: %d\n", count);
	while(count == SIZE) {
		pthread_cond_wait(&cond_prod, &mutex);
	}
	buffer[in] = n;
	in = (in + 1) % SIZE;
	count++;
	pthread_cond_signal(&cond_cons);
	pthread_mutex_unlock(&mutex);
}

void *prod(void *tid) {
	int elem, i;
	for (i = 0; i < FIB; ++i) {
		elem = fib();
		insere(elem);
	}
	pthread_exit(NULL);
}

int retira() {
	int elem;
	pthread_mutex_lock(&mutex);
	while(count == 0) {
		pthread_cond_wait(&cond_cons, &mutex);
	}
	count--;
	elem = buffer[out];
	out = (out + 1) % SIZE;
	pthread_cond_signal(&cond_prod);
	pthread_mutex_unlock(&mutex);
	return elem;
}

void processa(int n) {
	if(ehPrimo(n)) {
		printf("%d (primo)\n", n);
	} else {
		printf("%d\n", n);
	}
}

void *cons(void *tid) {
	int id = * (int *) tid;
	int elem, i = 0;
	//printf("Entrou na thread %d\n", id);
	while(1) {
		//printf("Thread %d; iteração %d\n", id, i);
		i++;
		pthread_mutex_lock(&m_cons);
		//printf("%d\n", total);
		if(total < FIB) {
			total++;
			elem = retira();
			pthread_mutex_unlock(&m_cons);
			processa(elem);
		} else {
			pthread_mutex_unlock(&m_cons);
			break;
		}
	}
	pthread_exit(NULL);
}

int main(int argc, const char *argv[]) {
	pthread_t threads[4];
	int tids[4] = {0, 1, 2, 3};

	pthread_mutex_init(&mutex, NULL);
	pthread_mutex_init(&m_cons, NULL);
	pthread_cond_init(&cond_prod, NULL);
	pthread_cond_init(&cond_cons, NULL);

	pthread_create(&threads[0], NULL, prod, NULL);
	pthread_create(&threads[1], NULL, cons, &tids[1]);
	pthread_create(&threads[2], NULL, cons, &tids[2]);
	pthread_create(&threads[3], NULL, cons, &tids[3]);

	/*
	for(t = 0; t < 4; t++) {
		pthread_join(threads[t], NULL);
	}
	*/

	pthread_mutex_destroy(&mutex);
	pthread_mutex_destroy(&m_cons);
	pthread_cond_destroy(&cond_prod);
	pthread_cond_destroy(&cond_cons);
	pthread_exit(NULL);
}
