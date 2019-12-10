#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <pthread.h>
#include "timer.h"

double alt_sum(int num) {
	int i;
	long long int j;
	double approx[2] = {0.0, 0.0};
	double ret = 0.0;

	for (i = 0; i < 2; i++) {
		for(j = i; j <= num; j += 2) {
			approx[i] += pow(-1.0, j) * 1.0/(2*j+1);
		}
		ret += approx[i];
	}

	ret = ret * 4.0;
	return ret;
}

int main(int argc, char *argv[])  {
	long long int n;
	double approx_pi = 0;
	double start, end, time_spent;
  
	if (argc < 2) {
		fprintf(stderr, "Missing option: number of elements.\n");
		exit(-1);
	}
	n = atoll(argv[1]);

	GET_TIME(start);
	approx_pi = alt_sum(n);
	GET_TIME(end);

	time_spent = end - start;
	printf("Calculated pi: %.15lf\nKnown pi: %.15lf\nErro aproximado: %.15lf\n", approx_pi, M_PI, M_PI - approx_pi);
	printf("Time used with sequential algorithm for %lld elements: %.8lf\n", n, time_spent);
	return 0;
}
