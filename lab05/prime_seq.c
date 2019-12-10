#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <math.h>
#include "timer.h"

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

int main (int argc, char *argv[]) {
	long long int i, n;	
	double start, end, time;
	int quantity = 0;

	if (argc < 2) {
		fprintf(stderr, "Invalid number of options.\n");
		exit (-1);
	}

	n = atoll(argv[1]);

	GET_TIME(start);
	for (i = 0; i <= n; i++) {
		if (isPrime(i)) {
			// printf("%lld - ", i);
			quantity++;
		}
	}
	GET_TIME(end);
	
	time = end - start;
	// printf("\n");
	printf("Number of prime numbers: %d\n", quantity);
	printf("Time spent: %.6lf\n", time);
	return 0;
}
