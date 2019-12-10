/*Para usar um determinado recurso e necess  ́ ario que duas threads (do mesmo tipo) o  ́
fac ̧a ao mesmo tempo (i.e, se uma thread quer usar o recurso, ela deve aguardar ate que outra thread tamb  ́ em ́
queira usa-lo). O acesso ao recurso  ́ e exclusivo, ent  ́ ao assim que as threads terminam de us  ̃ a-lo, o seu acesso  ́
deve ser liberado para outros pares de threads. Usando semaforos  ́ ou monitores, implemente uma soluc ̧ao ̃
para o problema (codigo das threads).  ́ Comente seu codigo  ́ .*/

sem_t em, par; //em: exclus ̃ao m ́utua; par: barreira (condicional)
int cont=0; //contador de threads para a barreira
sem_init(&em, 0, 1); sem_init(&par,0,0); //globais
proc T() {
  while(true) {
    //executa c ́odigo que n ̃ao depende do recurso...
    sem_wait(&em); //exclus ̃ao m ́utua para acesso a vari ́aveis compartilhadas
    cont++; //incrementa contador de threads que querem usar o recurso
    if(cont!=2) {//ainda n ̃ao existe um par
      sem_post(&em); //libera a exclus ̃ao m ́utua
      sem_wait(&par); //bloqueia para esperar outra thread
      sem_post(&par); //deixa um sinal no sem ́aforo para us ́a-lo comm EM adiante
    } else { //j ́a existe um par
      sem_post(&par); //libera a outra thread e mantem a exclus ̃ao m ́utua
    }
    //executa o c ́odigo que usa o recurso...
    sem_wait(&par); //usa o semaforo ’par’ para EM entre as duas threads
    cont--; //terminou de usar o recurso
    if(cont==0) //as duas threads terminaram de usar o recurso, libera a EM para as demais
      sem_post(&em);
    else
      sem_post(&par); //usa o semaforo ’par’ para EM entre as duas threads
  }
}
