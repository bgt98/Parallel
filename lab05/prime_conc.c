#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <math.h>
#include "timer.h"

int nThreads;
long long int global_index;
long long int num_elem;

pthread_mutex_t mutex;

int isPrime(long long int n) {
	int i;
	if (n <= 1) return 0;
	if (n == 2) return 1;
	if (n % 2 == 0) return 0;
	for (i = 3; i <= sqrt(n); i += 2) {
		if (n % i == 0) return 0;
	}
	return 1;
}

void *calculatePrimes(void *tid) {
	long long int index;
	int acc = 0;
	int *ret;

	pthread_mutex_lock(&mutex);
	index = global_index;
	global_index++;
	pthread_mutex_unlock(&mutex);

	while (index <= num_elem) {
		if (isPrime(index)) acc++;
		pthread_mutex_lock(&mutex);
		index = global_index;
		global_index++;
		pthread_mutex_unlock(&mutex);
	}

	ret = (int *) malloc(sizeof(int));
	*ret = acc;
	pthread_exit((void *) ret);
}

int main(int argc, char *argv[]) {
	long long int t, n;
	double start, end, time;
	int *qt_primes;
	int acc_primes = 0;
	pthread_t *sys_tids;

	pthread_mutex_init(&mutex, NULL);

	if (argc < 3) {
		fprintf(stderr, "Invalid number of options");
		exit(-1);
	}

	n = atoll(argv[1]);
	num_elem = n;
	nThreads = atoi(argv[2]);

	sys_tids = (pthread_t *) malloc(sizeof(pthread_t) * nThreads);
	GET_TIME(start);
	for (t = 0; t < nThreads; t++) {
		if (pthread_create(&sys_tids[t], NULL, calculatePrimes, NULL)) {
			fprintf(stderr, "Failure to create thread.\n");
			exit(-1);
		}
	}

	for (t = 0; t < nThreads; t++) {
		if (pthread_join(sys_tids[t], (void *) &qt_primes)) {
			fprintf(stderr, "Failure to join threads.\n");
			exit(-1);
		}
		acc_primes += *qt_primes;
		free(qt_primes);
	}
	GET_TIME(end);

	free(sys_tids);

	time = end - start;
	printf("Number of prime numbers: %d\n", acc_primes);
	printf("Time spent with %d threads: %.6lf\n", nThreads, time);

	pthread_exit(NULL);
}
