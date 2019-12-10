/*Suponha que apenas troquemos a ordem das duas primeiras sentenc ̧as da soluc ̧ao de Peterson para o  ̃
problema de exclusao m  ̃ utua entre duas threads (c  ́ odigo abaixo). Essa soluc ̧  ́ ao modificada garante exclus  ̃ ao m  ̃ utua? Se sim,  ́
ela garante tambem aus  ́ encia de ˆ starvation? Justifique suas respostas.*/

boolean querEntrar0=false; querEntrar1=false; int volatile turno; //globais
T0: while(true) {                        T1: while(true) {
  (1) turno = 1;                            (1) turno = 0;
  (2) querEntrar0 = true;                   (2) querEntrar1 = true;
  while((querEntrar1) && (turno==1)) {;}     while((querEntrar0) && (turno==0)) {;}
  //sec ̧ ̃ao cr ́ıtica                         //sec ̧ ̃ao cr ́ıtica
  querEntrar0 = false;                      querEntrar1 = false;
  //fora sec ̧ ̃ao cr ́ıtica }                  //fora sec ̧ ̃ao cr ́ıtica }

Resp.: A soluc ̧ao modificada N  ̃ AO garante exclus  ̃ ao m  ̃ utua. Por exemplo, se T1 executa a sentenc ̧a (1) e em seguida  ́
perde a CPU para T0, T0 executa as sentenc ̧as (1) e (2) e em seguida entra na sec ̧ao cr  ̃  ́ıtica. Enquanto esta executando a  ́
sec ̧ao cr  ̃  ́ıtica, T0 perde a CPU de volta para T1. T1 executa a sentenc ̧a (2) e tambem entra na sec ̧  ́ ao cr  ̃  ́ıtica.
