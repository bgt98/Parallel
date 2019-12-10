/* Dadas as seis threads de uma aplicac ̧ao (mostradas abaixo: T1, T2, T3, T4, T5,  ̃
T6), complete o codigo dessas threads de forma a garantir a seguinte sequ  ́ encia de execuc ̧ ˆ ao sempre:  ̃
T1 e T2 (qualquer ordem ou em paralelo); depois T3 e T4 (qualquer ordem ou em paralelo); depois
T5 e T6 (qualquer ordem ou em paralelo). Use C ou Java. Comente seu codigo.*/



int x, y, z, k, a, b; //variiáveis globais
T1: //lˆe x da entrada padrão T2: //lˆe y da entrada padrão
T3: //lˆe x e y T4: //lˆe x e y
//calcula z //calcula k
T5: //lˆe z e k T6: //lˆe z e k
//calcula a //calcula b
Resp.:
int x, y, z, k, a, b; //variiáveis globais
sem_t em=1, s1=0, s2=0; //semáforos globais
int cond=0; //vari ́avel global
T1 e T2:
//lˆe x ou y da entrada padrão
sem_wait(&em);
cond++;
if(cond==2) sem_post(&s1);
sem_post(&em);
T3 e T4:
sem_wait(&s1);
cond--;
sem_post(&s1);
//lˆe x e y
//calcula z ou k
sem_wait(&s1);
cond++;
if(cond==2) sem_post(&s2);
else sem_post(&s1);
