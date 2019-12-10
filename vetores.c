receivedMsg;
}
}
Questao 3 (2.5pts)  ̃ Uma aplicac ̧ao consome grande parte do seu tempo total de proces-  ̃
samento executando o trecho de codigo abaixo. O vetor  ́ vet foi definido como variavel  ́
global com M inteiros. Implemente uma versao concorrente para esse trecho de c  ̃ odigo  ́
de forma a obter o maximo de aproveitameto poss  ́  ́ıvel (reduc ̧ao do tempo total de proces-  ̃
samento da aplicac ̧ao) em uma m  ̃ aquina com P processadores.  ́ Comente seu codigo.  ́
for (int t=0; t<N; t++) {
for (int i=0; i<M; i++) {
if(i==0)
vet[i] = vet[i] + vet[i+1] + k;
else if(i==(M-1))
vet[i] = vet[i-1] + vet[i] + k;
else
vet[i] = vet[i-1] + vet[i] + vet[i+1] + k;
}
}
Resp.: Aplicac ̧ao com P threads, onde cada thread executa o seguinte procedi-  ̃
mento:
int NTHREADS = P, cont=0;
pthread_mutex_t mutex_bar;
pthread_cond_t cond_bar;
void *thread (void *tid) {
int *id = (int*) tid;
for (int t=0; t<N; t++) {
for (int i=*id; i<M; i+=NTHREADS) {
if(i==0)
vet[i] = vet[i] + vet[i+1] + k;
else if(i==(M-1))
vet[i] = vet[i-1] + vet[i] + k;
else
vet[i] = vet[i-1] + vet[i] + vet[i+1] + k;
}
//barreira antes de passar para a proxima iteracao
pthread_mutex_lock(&mutex_bar);
cont++;
if(cont==NTHREADS) {
cont=0;
pthread_cond_broadcast(&cond_bar);
} else
pthread_cond_wait(&cond_bar, &mutex_bar);
pthread_mutex_unlock(&mutex_bar);
}
}
Questao 4 (2.5pts)  ̃ Uma
