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
