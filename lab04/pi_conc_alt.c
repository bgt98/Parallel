#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <pthread.h>

int nThreads;
long long int n;

void *approximate_pi(void * tid) {
    double *partial_pi;
	double temp = 0.0;
    long long int id = * (long long int *) tid;

    for ( ; id <= n; id += nThreads) {
        temp += pow(-1.0, id) * 1.0/(2*id + 1);
    }

    partial_pi = (double *) malloc(sizeof(double));
	*partial_pi = temp;
    free(tid);
    pthread_exit((void *) partial_pi);
}

int main(int argc, char *argv[]) {
    long long int *local_tid;
    int t;
    double approx_pi;
    double *part_pi;
    pthread_t *sys_tid;

    if (argc < 3) {
        fprintf(stderr, "Missing options.\n");
        exit(-1);
    }

    n = atoll(argv[1]);
    nThreads = atoi(argv[2]);

    sys_tid = (pthread_t *) malloc(sizeof(pthread_t) * nThreads);
    if(!sys_tid) {
        fprintf(stderr, "Failure to allocate threads' ids.\n");
        exit(-1);
    }

    for (t = 0; t < nThreads; t++) {
        local_tid = (long long int *) malloc(sizeof(long long int));
	*local_tid = t;
        if (!local_tid) {
            fprintf(stderr, "Failure to allocate thread local id.\n");
            exit(-1);
        }

        if (pthread_create(&sys_tid[t], NULL, approximate_pi, (void *) local_tid)) {
            fprintf(stderr, "Failure to create thread.\n");
            exit(-1);
        }
    }

    approx_pi = 0.0;
    for (t = 0; t < nThreads; t++) {
        if(pthread_join(sys_tid[t], (void *) &part_pi)) {
            fprintf(stderr, "Failure to join threads.\n");
            exit(-1);
        }

        approx_pi += *part_pi;
        free(part_pi);
    }

    approx_pi = 4*approx_pi;
    printf("Calculated pi: %.15lf\nKnown pi: %.15lf\n", approx_pi, M_PI);

    free(sys_tid);
    pthread_exit(NULL);
}
