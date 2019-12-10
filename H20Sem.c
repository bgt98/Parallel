/*Considere dois tipos de threads — O e H — que devem ser ligadas para formarem moleculas  ́ H2O.
Para isso, e preciso criar uma barreira para fazer cada thread esperar at  ́ e que uma mol  ́ ecula completa esteja pronta (2 threads  ́
H e 1 thread O). Assim que uma thread passa pela barreira, ela deve invocar a operac ̧ao ̃ fazLigacao(). Deve-se garantir
que todas as threads de uma molecula invocam essa operac ̧  ́ ao antes de qualquer outra thread de outra mol  ̃ ecula. Usando  ́
semaforos  ́ , implemente uma soluc ̧ao para o problema (c  ̃ odigo das threads O e c  ́ odigo das threads H).  ́ Comente seu codigo  ́ .
sem_t em, hidro, oxi, bar; //sem ́aforos de exclus ̃ao m ́utua e de condic ̧ ̃ao*/


int contH=0, contO=0; //contadores para n ́umero de threads H e O esperando na barreira
int h=0; //contador das threads de H na formac ̧ ̃ao da mol ́ecula
sem_init(&em,0,0); sem_init(&hidro,0,0); sem_init(&oxi,0,0); sem_init(&bar,0,0);
proc void H () { //c ́odigo das threads de hidrogˆenio
  sem_wait(&em);
  contH++;
  if((contH>1)&&(contO>0)) { //formar mol ́ecula
    contH=contH-2; contO--; //decrementa os contadores de O e H
    sem_post(&hidro); //libera a outra thread de H
    sem_post(&oxi); //libera a thread de O
  } else {
    sem_post(&em); //libera a exclus ̃ao m ́utua
    sem_wait(&hidro); //bloqueia para esperar as outras threads
  }
  h++; //contabiliza as threads de H da mol ́ecula
  if(h==2) sem_post(&bar); //libera a barreira
  fazLigacao();
}

proc void O () { //c ́odigo das threads de oxigˆenio
  sem_wait(&em);
  contO++;
  if(contH>1) { //formar mol ́ecula
    contH=contH-2; contO--; //decrementa os contadores de O e H
    sem_post(&hidro); sem_post(&hidro); //libera duas threads de H
  } else {
    sem_post(&em); //libera a exclus ̃ao m ́utua
    sem_wait(&oxi); //bloqueia para esperar as outras threads
  }
  sem_wait(&bar); //espera as duas threads de H para liberar a barreira
  h=0;
  sem_post(&em); //libera a exclus ̃ao m ́utua (permite a formac ̧ ̃ao de novas mol ́eculas)
  fazLigacao();
}
