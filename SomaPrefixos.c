/*Dado (a1, a2, . . . , aN ) um vetor de dimensao ̃ N, a soma de prefixos e uma  ́
operac ̧ao que consiste em construir um outro vetor cujos termos s  ̃ ao a soma cumulativa dos termos  ̃
do vetor dado. Por exemplo, dado o vetor (1, 4, 2, 6, 0, 1) como entrada, o vetor de sa ́ıda sera ́ (1, 1 +
4, 1 + 4 + 2, 1 + 4 + 2 + 6, 1 + 4 + 2 + 6 + 0, 1 + 4 + 2 + 6 + 0 + 1), ou seja, (1, 5, 7, 13, 13, 14). Essa
operac ̧ao possui diversas aplicac ̧  ̃ oes no contexto de algoritmos paralelos. O algoritmo sequencial para  ̃
realizar essa operac ̧ao ̃ e bastante trivial, como mostrado abaixo:  ́
soma_prefixo[0] = vetor[0];
for(i=1; i<N; i++)
soma_prefixo[i] = soma_prefixo[i-1] + vetor[i];
Implemente um programa concorrente em C (e PThreads) para realizar essa operac ̧ao. Todo  ̃
o processamento devera ser feito pelas threads. A func ̧  ́ ao ̃ main devera apenas criar as threads,  ́
aguardar o termino de todas elas e imprimir o vetor de sa  ́  ́ıda. Considere que o vetor de entrada,
o seu tamanho e o numero de threads ser  ́ ao definidos pelo usu  ̃ ario a cada execuc ̧  ́ ao do programa.  ̃
Escreva o codigo de forma leg  ́  ́ıvel, organizada e com comentarios.  ́
Note que atribuir a cada thread a tarefa de calcular separadamente um elemento do vetor de
sa ́ıda nao ̃ e uma boa divis  ́ ao da tarefa pois a thread respons  ̃ avel pelo  ́ ultimo elemento ter  ́ a que executar  ́
o codigo sequencial (eliminando a possibilidade de ganhos da vers  ́ ao concorrente).  ̃
*/


//variaveis globais (inicializadas na main antes da criacao das threads)
//tamanho do vetor
int tam;
//vetor de entrada e de saida
float *veta, *vetb;
//vetor auxiliar
float *vetaux;
//numero de threads
int nthreads;
//lock para exclusao mutua
pthread_mutex_t mutex;
//variavel de condicao
pthread_cond_t cond;
//funcao para sincronizacao por barreira
void barreira(int nthreads) {
  static int n = 0; //numero de threads que ja chegaram na barreira
  pthread_mutex_lock(&mutex);
  n++;
  if(n < nthreads) { //barreira incompleta, thread deve se bloquear
    pthread_cond_wait(&cond, &mutex);
  } else {
    n = 0;
    pthread_cond_broadcast(&cond);
  }
  pthread_mutex_unlock(&mutex);
}
//funcao executada pelas threads
void *tarefa (void *tid) {
  int id = * (int *) tid, i;
  free(tid);
  int inicio, fim, bloco;
  //primeira rodada: cada thread calcula a soma de prefixos de uma parte do vetor
  
  //calcula as posicoes de inicio e fim que a thread ira processar
  bloco = nthreads/tam; //tamanho do bloco que cada thead ira processar
  inicio = id * bloco;
  fim = inicio + bloco;
  if (id == (nthreads-1)) fim = tam; //deixa para a ultima thread o restante do vetor
  
  vetb[inicio] = veta[inicio]; //o primeiro elemento eh repetido
  //calcula a soma de prefixos parcial
  for(i=inicio+1; i<fim; i++) {
    vetb[i] = vetb[i-1] + veta[i];
  }
  vetaux[id] = vetb[fim-1]; //armazena no vetor auxiliar o ultimo valor calculado
    //(devera ser somado aos valores seguintes do vetor de saida)
  //sincronizacao por barreira para aguardar todas as threads terminarem a primeira rodada
  barreira(nthreads);
  
  //segunda rodada: soma em cada parte a contribuicao das partes anteriores
  
  //apenas as threads com id>0 precisam continuar
  if(id > 0) {
    int soma_anterior = 0;
    for(i=0; i<id; i++) {
      soma_anterior += vetaux[i];
    }
    //atualiza os valores da sua parte do vetor de saida
    for(i=inicio; i<fim; i++) {
      vetb[i] += soma_anterior;
    }
  }
  pthread_exit(NULL);
}
