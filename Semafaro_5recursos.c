/*(3.0 pts) Considere um recurso compartilhado com 5 replicas. Enquanto o n  ́ umero de  ́
threads usando o recurso e menor que 5, novas threads podem usar o recurso imediatamente. Uma  ́
vez que ha 5 threads usando o recurso, todas elas devem liber  ́ a-lo antes de qualquer outra thread poder  ́
usa-lo. Implemente uma soluc ̧  ́ ao para esse problema (c  ̃ odigo executado pelas threads para controlar o  ́
acesso as r ` eplicas do recurso). Use a linguagem C e o mecanismo de sem  ́ aforo.  ́ Comente seu codigo.*/



sem_t mutex=1, block=0; //sem ́aforos globais inicializados com 1 e 0
int active=0, waiting=0; //n ́umero de threads ativas e esperando pelo recurso
int must_wait=false; //estado da aplicac ̧ ̃ao: proxima thread deve ou n ̃ao esperar por recurso
sem_wait(&mutex); //exclus ̃ao mutua para acessar variaveis compartilhadas
if (must_wait) {
  ++waiting; //atualiza o num. de threads em espera e bloqueia a thread
  sem_post(&mutex);
  sem_wait(&block);
  --waiting; //atualiza o num. de threads em espera
}
++active; //atualiza o num. de threads ativas
must_wait = (active==5); //verifica se o num. de threads ativas chegou a 5
if (waiting>0 && !must_wait) //se h ́a r ́eplicas dispon ́ıveis e threas esperando...
  sem_post(&block); //..libera uma thread, mantendo a exclus ̃ao m ́utua
else
  sem_post(&mutex); //..se n ̃ao h ́a threads esperando, termina a exclus ̃ao m ́utua
/* sec ̧ ̃ao cr ́ıtica que usa a r ́eplica do recurso */
sem_wait(&mutex); //exclus ̃ao m ́utua para acessar vari ́aveis compartilhadas
--active;
if(active==0)
  must_wait=false; //se for a  ́ultima thread ativa, libera o acesso
if(waiting>0 && !must_wait) //se h ́a r ́eplicas dispon ́ıveis e threas esperando...
  sem_post(&block); //..libera uma thread, mantendo a exclus ̃ao m ́utua
else
  sem_post(&mutex); //..se n ̃ao h ́a threads esperando, termina a exclus ̃ao m ́utua
