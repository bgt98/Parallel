Considere um programa com 5 threads T1, T2, T3, T4 e T5 (codigo mostrado abaixo). Pode ocorrer dos valores 1, 2, 5 e 6 serem impressos na tela em al-
guma execuc ̧ao do programa? Justifique cada valor mostrando uma poss  ̃  ́ıvel sequencia de execuc ̧ao das threads.  ̃


int y=0; sem_t s1, s2; sem_init(&s1,0,0); sem_init(&s2,0,0); //globais

(a)T1: int i=2;             T2: int i=3;          T3:               T4:                 T5: int x=1;
(b)while(i>0) {             sem_wait(&s1);        y=y+1;            sem_wait(&s2);      y=y+x;
(c) y=y+i;                  y=y+i;                sem_post(&s1);    printf("%d",y);     x=x+1;   
(d) i=i-1;}
(e)sem_post(&s2);

Resp.:
• 1: T3.b (le y=0); T1; T3.b (faz y=1); T4 ˆ
• 2: T3.b (le y=0); T1.abcd (faz y=2); T3.b (faz y=1); T1.bcde (faz y=2); T4 ˆ
• 5: T1.abc (le y=0); T3; T1.c (faz y=2); T2.abc (l ˆ e y=2); T1 (faz y=3); T2.c (faz y=5); T4
• 6: T1.abc (le y=0); T3; T1 (faz y=3); T2 (faz y=6); T4
