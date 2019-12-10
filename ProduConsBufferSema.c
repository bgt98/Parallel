O codigo abaixo implementa uma soluc ̧  ́ ao para o problema do pro-  ̃
dutor/consumidor considerando um buffer de tamanho ilimitado. O codigo funcionou  ́
corretamente em varias execuc ̧  ́ oes mas em algumas delas ocorreu do consumidor retirar  ̃
um item que nao existia no buffer. Identifique a causa do erro no c  ̃ odigo e proponha uma  ́
maneira de resolve-lo (sem acrescentar novos sem ˆ aforos).  ́ Comente seu codigo. 
́
1:int n=0; sem_t s, d; //s inicializado com 1 e d inicializado com 0
2:void *cons(void* a) {int item; void *prod(void *a) {int item;
3: sem_wait(&d); while(1) {
4: while(1) { produz_item(&item);
5: sem_wait(&s); sem_wait(&s);
6: retira_item(&item); insere_item(item);
7: n--; n++;
8: sem_post(&s); if(n==1) sem_post(&d);
9: consome_item(item); sem_post(&s);
10: if(n==0) sem_wait(&d); }
11: } }
12:}

Resp.: Uma situac ̧ao que pode ocorrer  ̃ e a seguinte: O consumidor esvazia o  ́
buffer, altera o valor de n para 0, libera o semaforo s e em seguinda perde a CPU. O pro-  ́
dutor insere um novo item, incrementa o valor de n e o semaforo d antes do consumidor  ́
retomar a CPU e executar a sentenc ̧a if (linha 10). O semaforo d ficar  ́ a com um sinal  ́
acumulado incorretamente, o qual permitira que o consumidor retire um item quando o  ́
buffer estiver vazio (o valor de n voltara a ser 0, mas o consumidor n  ́ ao ser  ̃ a bloqueado  ́
pois o semaforo d ter  ́ a um sinal sobrando).  ́
Uma poss ́ıvel soluc ̧ao ̃ e apresentada abaixo: inclus  ́ ao da vari  ̃ avel local ’m’ no  ́
codigo do consumidor (linha 3), atualizac ̧  ́ ao dessa vari  ̃ avel com o valor de ’n’ (linha 9)  ́
e substituic ̧ao de ’n’ por ’m’ na sentenc ̧a if (linha 12). O c  ̃ odigo do produtor permanece  ́
como estava

1:int n=0; sem_bin s=1, sem_bin d=0;
2:void *cons(void *a) {int item;
3: int m; //variavel local
4: sem_wait(&d);
5: while(true) {
6: sem_wait(&s);
7: retira_item(&item);
8: n--;
9: m=n;
10: sem_post(&s);
11: consome_item(item);
12: if(m==0) sem_wait(&d);
13: }
14:}
