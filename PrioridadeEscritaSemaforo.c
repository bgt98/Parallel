/*Usando semaforos (em C) ou monitores (em Java) implemente uma soluc ̧  ́ ao para o  ̃
problema dos leitores/escritores (mais de um leitor pode ler ao mesmo tempo, apenas um escritor pode
escrever de cada vez e se um escritor esta escrevendo nenhum leitor pode estar lendo) com prioridade para  ́
escrita.
Resp.:*/

sem_t em_e=1, em_l=1, escr=1, leit=1; int e=0, l=0; //globais

Leitores:                                  Escritores:

while(1) {                                  while(1) {
sem_wait(&leit);                            sem_wait(&em_e); e++;
sem_wait(&em_l); l++;                       if(e==1) sem_wait(&leit);
if(l==1) sem_wait(&escr);                   sem_post(&em_e);
sem_post(&em_l);                            sem_wait(&escr);
sem_post(&leit)                             //escreve...
//le...                                     sem_post(&escr);
sem_wait(&em_l); l--;                       sem_wait(&em_e); e--;
if(l==0) sem_post(&escr);                   if(e==0) sem_post(&leit);
sem_post(&em_l); }                          sem_post(&em_e); }
