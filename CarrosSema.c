/*
Questao 4 (2.5pts)  ̃ Carros vindos do norte e do sul chegam em uma ponte de uma unica pista, apenas  ́
carros na mesma direc ̧ao podem compartilhar a ponte, carros em direc ̧  ̃ ao oposta devem esperar at  ̃ e a ponte  ́
ficar liberada. O ultimo carro a cruzar a ponte libera a travessia para os carros aguardando na direc ̧  ́ ao oposta.  ̃
Implemente uma soluc ̧ao para o problema (c  ̃ odigo para os carros do sul e do norte) usando as primitivas  ́
espera() (faz os carros esperarem) e libera() (libera os carros que estao esperando). Comente seu c  ̃ odigo. */

́
sem_t em, s; int cont=0; sem_init(&em,0,1); sem_init(&s,0,0);
espera() { libera() {
sem_wait(&em); cont++; sem_post(&em); sem_wait(&em);
sem_wait(&s); cont--; if(cont>0) sem_post(&s);
if(cont>0) sem_post(&s); else sem_post(&em);
else sem_post(&em); } }
Resp.:
int n_sul=0, n_norte=0; sem_t e; //variaveis globais
sem_init(&e,0,1);
//Pseudo-codigo dos carros do norte
sem_wait(&e);
while(n_sul > 0) {
sem_post(&e);
espera();
sem_wait(&e);
}
n_norte++;

sem_post(&e);
//atravessa a ponte
sem_wait(&e);
n_norte--;
if(n_norte == 0)
libera();
sem_post(&e);
//Pseudo-codigo dos carros do sul
sem_wait(&e);
while(n_norte > 0) {
sem_post(&e);
espera();
sem_wait(&e);
}
n_sul++;
sem_post(&e);
//atravessa a ponte
sem_wait(&e);
n_sul--;
if(n_sul == 0)
libera();
sem_post(&e);
