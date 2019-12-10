/*
    Atividade nº1 do laboratório de Computação Concorrente
    Nome: Gilberto Lopes
    E-mail: gilbertolif@dcc.ufrj.br

    Realizar um incremento nos elementos de um vetor, efetuando uma divisão de tarefa entre duas threads.
    Versão com divisão bem definida de elementos por threads.
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>

typedef struct {
    int idThread, numThreads, vectorSize;
    int *vetor;
} ArgThreads_t;

void * IncrementoConcorrente(void *arg) {
    ArgThreads_t *args = (ArgThreads_t *) arg;
    int i, startPoint, endPoint, remainder;
    remainder = args->vectorSize % args->numThreads;
    startPoint = (args->vectorSize / args->numThreads) * args->idThread;
    if (args->idThread < remainder) {
        startPoint += args->idThread;
    } else {
        startPoint += remainder;
    }
    endPoint = startPoint + (args->vectorSize / args->numThreads);
    if (args->idThread < remainder) {
        endPoint++;
    }

    for (i = startPoint; i < endPoint; i++) args->vetor[i]++;

    printf("Fim da thread %d\n", args->idThread);
    free(args);
    pthread_exit(NULL);
}

int main(int argc, char const *argv[])
{
    pthread_t *tidSistema;
    int i, t, vecSize, nThreads;
    int *vec;
    ArgThreads_t *args;

    vecSize = 10; nThreads = 2;

    for (i = 0; i < argc; i++) {
        if(!strcmp(argv[i], "-n")) nThreads = atoi(argv[i+1]);
        if(!strcmp(argv[i], "-s")) vecSize = atoi(argv[i+1]);
    }

    tidSistema = (pthread_t *) calloc(nThreads, sizeof(pthread_t));
    if (!tidSistema) {
        printf("ERRO: calloc()\n"); exit(1);
    }

    // aloca o vetor de S elementos
    vec = (int *) calloc(vecSize, sizeof(int));
    if (!vec) {
        printf("ERRO: calloc()\n"); exit(1);
    }

    // inicializa cada posição i com i+1 e imprime
    printf("--- Vetor original:\n");
    for (i = 0; i < vecSize; i++) {
        vec[i] = i;
        printf("Elemento %d: %d\n", i, vec[i]);
    }

    // loop principal das threads
    for (t = 0; t < nThreads; t++) {
        // Aloca os argumentos pra cada thread
        args = (ArgThreads_t *) malloc(sizeof(ArgThreads_t));
        if(!args) {
            printf("ERRO: malloc()\n"); exit(1);
        }

        // inicializa cada argumento com a id local da thread e o mesmo ponteiro pro vetor a ser incrementado
        args->idThread = t;
        args->numThreads = nThreads;
        args->vectorSize = vecSize;
        args->vetor = vec;

        // cria as threads
        if (pthread_create(&tidSistema[t], NULL, IncrementoConcorrente, (void *) args)) {
            printf("Erro: pthread_create()\n"); exit(-1);
        }
    }

    // garante que todas as threads tenham terminado
    for (t = 0; t < nThreads; t++) {
        if (pthread_join(tidSistema[t], NULL)) {
            printf("ERRO: pthread_join()\n"); exit(-1);
        }
    }

    // imprime e libera o vetor incrementado
    printf("--- Vetor incrementado:\n");
    for (i = 0; i < vecSize; i++) printf("Elemento %d: %d\n", i, vec[i]);
    free(vec);
    free(tidSistema);

    printf("Thread principal terminou\n");
    pthread_exit(NULL);
}
