/*O codigo abaixo foi escrito para resolver o problema dos “jantar dos fil  ́ osofos”,  ́
mas ele nao funciona corretamente. Identifique e corrija o(s) erro(s) no c  ̃ odigo. */
́
sem_t em=1, filo[N]=[0..0]; //N=num. de fil ́osofos
int DIR=(ID+1)%N; ESQ=(ID+N-1)%N; estado[N]=[PENSANDO..PENSANDO];
void filosofo(int i) {while(1){pensa(); pega_garfos(i); come(); devolve_garfos();}}
void pega_garfos(int i) {
sem_wait(&em); estado[i]=FAMINTO; teste(i); sem_wait(filo[i]); sem_post(&em);}
void devolve_garfos(int i) {
sem_wait(&em); estado[i]=PENSANDO; sem_post(&em); teste(ESQ); teste(DIR);}
void teste (int i) {
if (estado[ESQ]!=COMENDO && estado[DIR]!=COMENDO) {
estado[i]=COMENDO; sem_post(&filo[i]);}

Resp.: Ha 3 erros no c  ́ odigo. Na func ̧  ́ ao ̃ pega garfos, o filosofo ira bloquear em filo[i] antes  ́
de liberar o semaforo de exclus  ́ ao m  ̃ utua. Na func ̧  ́ ao ̃ devolve garfos, a exclusao m  ̃ utua deveria  ́
ser liberada apos as chamadas das func ̧  ́ oes teste(ESQ) e teste(DIR). E na func ̧  ̃ ao ̃ teste, e necess  ́ ario  ́
acrescentar a condic ̧ao de que o fil  ̃ osofo avaliado esteja no estado FAMINTO. A soluc ̧  ́ ao correta  ̃ e ́
apresentada abaixo:

sem_t em=1, filo[N]=[0..0]; //N=num. de fil ́osofos
int DIR=(ID+1)%N; ESQ=(ID+N-1)%N; estado[N]=[FAMINTO..FAMINTO];
void filosofo(int i) {while(1){pensa(); pega_garfos(i); come(); devolve_garfos();}}
void pega_garfos(int i) {
sem_wait(&em); estado[i]=FAMINTO; teste(i); sem_post(&em); sem_wait(filo[i]);}
void devolve_garfos(int i) {
sem_wait(&em); estado[i]=PENSANDO; teste(ESQ); teste(DIR); sem_post(&em);}
void teste (int i) {
if (estado[i])==FAMINTO && Estado[ESQ]!=COMENDO && estado[DIR]!=COMENDO) {
estado[i]=COMENDO; sem_post(&filo[i]);}
