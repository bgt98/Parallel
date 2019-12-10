/*
PS 2015.1
Questao 2: Uma aplicac ̧ao possui N threads “Foo” que geram strings para serem impressas na sa ́ıda padrao, e uma thread “Bar” que  ̃ e responsável unicamente pela im-  ́
pressao dessas strings. Quando uma thread “Foo” gera uma nova string, ela solicita/avisa que quer imprimir e aguarda (bloqueando-se) ate que a thread “Bar” selecione a sua  ́
string para ser impressa. O fluxo de execuc ̧ao da thread “Bar” consiste em aguardar (bloqueando-se) ate que uma requisic ̧  ́ ao de impress  ̃ ao seja feita. A thread “Bar” deve re-  ̃
alizar a impressao das strings respeitando a ordem de chegada das requisic ̧  ̃ oes, ou seja, se duas threads “Foo” estao aguardando, a primeira thread que fez a solicitac ̧  ̃ ao deve ser aten-  ̃
dida primeiro. (a) Implemente o codigo das threads “Foo” e “Bar” usando a  ́ linguagem C e os mecanismos de locks e variaveis de condic ̧  ́ ao ̃ para realizar as sincronizac ̧oes  ̃
necessarias entre as threads. (Avalie se seu c  ́ odigo est  ́ a correto: atende aos requisitos do problema e nao gera situac ̧  ̃ oes de deadlock ou livelock.)
Resp.: Esse problema e similar ao problema do “barbeiro dorminhoco”*/






#define N 5 //numero maximo de threads Foo
#define W 100 //tamanho maximo das strings que serao impressas
int esperando=0; //numero de threads Foo esperando (no maximo N)
int ocupado=0; //estado da thread Bar (0:livre, 1:ocupada)
int proximaThread=0; //thread Foo no inicio da fila de espera
int ultimaThread=0; //proxima posicao para as threads Foo na fila de espera
char *str; //string para ser impressa (tamanho maximo W-1 caracteres)
pthread_mutex_t em; //exclusao mutua
pthread_cond_t cond; //variavel de condicao das threads Foo
pthread_cond_t condB; //variavel de condicao da thread Bar
//funcao que sera chamada pelas threads Foo
int solicitaImpressao (char * s) {
  int minhaSenha;
  pthread_mutex_lock(&em);
  esperando++; //avisa que mais uma thread esta esperando
  minhaSenha = ultimaThread; //guarda sua ordem na fila de espera
  ultimaThread++;
  while(proximaThread != minhaSenha) {
    pthread_cond_wait(&cond, &em); //deve aguardar sua vez na fila
  }
  ocupado = 1; //sera atendida em seguida
  str = s; //armazena sua string na variavel global
  pthread_cond_signal(&condB); //desbloqueia a thread Bar
  pthread_mutex_unlock(&em); //libera a exclus ̃ao m ́utua
  return 1;
}
//funcao que sera executada pela thread Bar para aguardar requisicoes
void EsperaRequisicao () {
  pthread_mutex_lock(&em);
  while((esperando == 0) || (ocupado == 0)) { //nao ha nenhuma thread
    //Foo aguardando impressao ou a thread da vez ainda nao
    //retomou a sua execuc ̧ ̃ao

    pthread_cond_wait(&condB, &em);
  }
  esperando--;
  pthread_mutex_unlock(&em);
}
//funcao que sera executada pela thread Bar para finalizar requisicoes
void TerminaRequisicao () {
  pthread_mutex_lock(&em);
  ocupado = 0; //sinaliza que esta livre para a proxima impressao
  proximaThread++; //sinaliza qual  ́e a proxima thread que ser ́a atendida
  if(esperando > 0) //acorda todas as threads Foo para garantir o desbloqueio da proxima
  pthread_cond_broadcast(&cond);
  pthread_mutex_unlock(&em);
}
//funcao executada pelas threads Foo
void *Foo (void *threadid) {
  int req=0;
  while(req<M) { //executa M requisicoes
    solicitaImpressao(geraString());
    req++;
  }
  pthread_exit(NULL);
}
//funcao executada pela thread Bar
void *Bar (void *threadid) {
  int req=0;
  while(req<N*M) { //atende N*M requisicoes
    //espera requisicao
    EsperaRequisicao();
    //executa a impressao
    fprintf(stderr, %s\n", str);
    //termina a escrita
    TerminaRequisicao();
    req++;
  }
  pthread_exit(NULL);
}
//funcao principal
int main(int argc, char *argv[]) {
  //variaveis...
  //inicilaiza o mutex (lock de exclusao mutua) e a variavel de condicao
  pthread_mutex_init(&em, NULL);
  pthread_cond_init(&cond, NULL);
  pthread_cond_init(&condB, NULL);
  //aloca espaco para a string
  str = malloc(sizeof(char)*W);
  //cria as threads...
  pthread_exit(NULL);
}
