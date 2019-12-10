/*Um escritorio possui um banheiro que pode ser usado por homens e mulheres, mas 
nao por ambos ao mesmo tempo. O c  ̃ odigo abaixo implementa uma soluc ̧  ́ ao para o problema dando  ̃ pri-
oridade para as mulheres. (i.e., se um homem esta no banheiro, outros homens s  ́ o podem entrar se n  ́ ao ̃
houver mulheres esperando). A soluc ̧ao deve permitir qualquer n  ̃ umero de homens ou qualquer n  ́ umero de  ́
mulheres (mas nao ambos) no banheiro ao mesmo tempo. Complete o c  ̃ odigo implementando a func ̧  ́ ao que  ̃
deve ser executada pelos homens. Comente seu codigo.  ́
int h=0, m=0; //numero de homens e mulheres no banheiro (ou esperando), respectivamente*/

sem_init(&emM, 0, 1); //exclusao mutua entre mulheres
sem_init(&emH, 0, 1); //exclusao mutua entre os homens
sem_init(&prior, 0, 1); //prioridade para mulheres
sem_init(&ban, 0, 1); //acesso ao banheiro restrito a apenas homens ou apenas mulheres
void *Mulheres (void *threadid) {
while(1) {
sem_wait(&emM); m++;
if(m==1) {
sem_wait(&prior); //impede a entrada de novos homens no banheiro (prioridade mulheres)
sem_wait(&ban); } //aguarda a liberacao do banheiro pelos homens
sem_post(&emM);
//SC: usa o banheiro
sem_wait(&emM); m--;
if(m==0) { sem_post(&prior); sem_post(&ban); }
sem_post(&emM);
} }
//funcao executada pelos homens
void *Homens (void *threadid) {
while(1) {
sem_wait(&prior);//nao permite a entrada de novos homens se ha mulheres esperando
h++;
if(h==1) {
sem_wait(&ban);//aguarda a liberacao do banheiro pelas mulheres
}
sem_post(&prior);
//SC: usa o banheiro
sem_wait(&emH); h--;
if(h==0) sem_post(&ban);
sem_post(&emH);
}
pthread_exit(NULL);
}
