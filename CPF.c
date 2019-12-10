Voce foi contratado para desenvolver uma aplicac ̧ ˆ ao para a Justic ̧a  ̃
Federal que, dado o numero do CPF de uma pessoa, consulte todos os processos relaciona-  ́
dos a essa pessoa em um banco de dados e gere um arquivo texto com esse levantamento.
Como a busca no banco de dados e muito demorada, j  ́ a foi definido que a aplicac ̧  ́ ao ̃
devera ser concorrente, i.e., executar com v  ́ arias threads para aumentar sua efici  ́ encia, ˆ
diminuindo o tempo total para processar varias buscas consecutivas. A thread princi-  ́
pal (main) devera inicialmente criar um n  ́ umero de threads auxiliares igual ao  ́ dobro do
numero de processadores da m  ́ aquina  ́ . As threads auxiliares deverao realizar a tarefa  ̃
principal de, dado um numero de CPF, consultar o banco de dados e gerar o arquivo de  ́
sa ́ıda. A thread principal (main), apos criar as threads auxiliares, dever  ́ a ficar esperando  ́
a entrada de um novo CPF pelo teclado, ou o valor -1 indicando que a aplicac ̧ao deve  ̃
ser encerrada. A cada novo CPF informado, a thread principal devera acionar uma thread  ́
auxiliar para processa-lo, e logo em seguida voltar a esperar nova entrada do usu  ́ ario. As  ́
threads auxiliares devem ficar aguardando uma nova tarefa (CPF para consultar), realizar
a tarefa e voltar a aguardar nova tarefa, ate que a thread principal avise que a aplicac ̧  ́ ao ̃
deve terminar e nao h  ̃ a mais consultas pendentes para realizar (nesse caso a thread auxili-  ́
ar deve finalizar sua execuc ̧ao). Ap  ̃ os o usu  ́ ario solicitar o encerramento da aplicac ̧  ́ ao, as  ̃
consultas pendentes devem ser realizadas/finalizadas e novas consultas nao ser  ̃ ao mais re-  ̃
cebidas. A thread principal deve aguardar todas as threads auxiliares terminarem e entao ̃

finalizar a aplicac ̧ao.  ̃
Considere que ja existe uma func ̧  ́ ao implementada que dado um CPF ela faz a  ̃
busca dos processos e gera o arquivo de sa ́ıda. A assinatura dessa func ̧ao ̃ e: ́ void
consultaProcessos(long int cpf).
A func ̧ao ̃ sysconf( SC NPROCESSORS ONLN) pode ser usada para identificar o numero  ́
de processadores da maquina.  ́
(a) Usando a linguagem C, implemente uma soluc ̧ao para a aplicac ̧  ̃ ao definida  ̃
acima (codigo da thread principal e das threads auxiliares) usando de forma eficiente a  ́
capacidade de processamento da maquina.  ́

(b) Avalie a sua implementac ̧ao quanto  ̃ a` corretude (atende aos requisitos do prob-
lema, nao possui erros de concorr  ̃ encia, como possibilidade de condic ̧ ˆ oes de corrida in-  ̃

desejaveis, e n  ́ ao pode causar deadlock ou livelock) e  ̃ a` eficiencia ˆ (usa da melhor forma
poss ́ıvel a capacidade de processamento da maquina). Apresente argumentos objetivos  ́
que justifiquem a sua avaliac ̧ao.  ̃
Resp.: (a)


/* Variaveis globais */
pthread_mutex_t em; //lock par exclusao mutua
pthread_cond_t cond; //variavel de condicao para sincronizacao logica
int NTHREADS = sysconf(_SC_NPROCESSORS_ONLN) * 2;
long int cpf[NTHREADS]; //buffer para guardar CPFs esperando por atendimento
int in=0, out=0, cont=0; //variaveis de controle do buffer
int terminar=0; //variavel para registrar se a aplicacao deve terminar (0:nao, 1:sim)
//insere um novo cliente (CPF) no buffer para ser atendido por uma thread
//funcao executada pela thread main
int insere(long int cliente) {
pthread_mutex_lock(&em);
if(cont==NTHREADS) { //se nao ha espaco no buffer, recusa a solicitacao
pthread_mutex_unlock(&em);
return 0;
}
//insere o novo cliente e atualiza as variaveis de estado do buffer
cpf[in]=cliente;
in=(in+1)%NTHREADS;
cont++;
pthread_mutex_unlock(&em);
return 1;
}
//retira um cliente do buffer para atende-lo
//funcao executada pelas threads auxiliares
//(elas soh executam essa funcao se ja houver uma solicitacao armazenada)
long int retira() {
long int cliente;
cliente=cpf[out];
cpf[out]=0; //slot vazio
out=(out+1)%NTHREADS;
cont--;
return cliente;
}
//funcao principal das threads auxiliares
void *Trabalhador (void *t) {

int my_id = *(int*)t;
long int local_cpf;
while(1) { //permanece ativa ate que o sinal de terminar seja lanc ̧ado
pthread_mutex_lock(&em);
while((cont==0) && (!terminar)) {
pthread_cond_wait(&cond, &em);
}
if(cont>0) { //ha solicitacao para tratar, pega uma delas e processa
local_cpf=retira();
pthread_mutex_unlock(&em);
fazInteracao(local_cpf); //interacao com o cliente remoto, fora da secao critica
} else { //terminar==1, a thread deve encerrar
pthread_mutex_unlock(&em);
break;
}
}
pthread_exit(NULL);
}
//funcao principal
int main(int argc, char *argv[]) {
int i,res;
long int req;
pthread_t threads[NTHREADS];
int id[NTHREADS];
//inicializa o lock e a variavel de condicao
pthread_mutex_init(&em, NULL);
pthread_cond_init(&cond, NULL);
//cria as threads auxiliares
for(i=0;i<NTHREADS;i++) {
id[i]=i;
pthread_create(&threads[i], NULL, Trabalhador, (void *) &id[i]);
}
//espera os CPFs ou aviso de terminar (valor negativo)
do {
scanf("%ld", &req);
if(req<0) { //terminar a aplicacao
pthread_mutex_lock(&em);
terminar=1; //altera a variavel de estado
//desbloqueia as threads auxiliares que estiverem bloqueadas
pthread_cond_broadcast(&cond);
pthread_mutex_unlock(&em);
break; //sai do loop
}
res=insere(req); //tenta armazenar o CPF do cliente
if(res) { //se conseguir, desbloqueia uma thread auxiliar
pthread_cond_signal(&cond);
} else //se o limite de CPFs esperando ja foi atingido, ignora essa solicitacao
printf("Thread main: cpf=%d nao sera atendido!\n", req);
} while(1);
//espera todas as threads completarem
for (i = 0; i < NTHREADS; i++) {
pthread_join(threads[i], NULL);
}
pthread_exit (NULL);
}



Resp.: (b) Quanto a corretude: Ap ` os disparar todas as threads auxiliares, a  ́
thread main fica esperando a entrada do usuario. Se as threads auxiliares comec ̧arem  ́
antes de existir requisic ̧oes do usu  ̃ ario elas ficar  ́ ao bloqueadas na fila da vari  ̃ avel de  ́
condic ̧ao. A cada requisic ̧  ̃ ao recebida pela thread principal, uma thread auxiliar  ̃ e des-  ́
bloqueada. Se as threads auxiliares comec ̧arem quando ja h  ́ a requisic ̧  ́ oes pendentes, elas  ̃
nao ficar  ̃ ao bloqueadas (evitando deadlock). Um lock de exclus  ̃ ao m  ̃ utua est  ́ a sendo us-  ́
ado para sincronizar o acesso as sec ̧ ` oes cr  ̃  ́ıticas do codigo (o mesmo lock para a thread  ́
main e auxiliares). As threads liberam o lock antes de se bloquearem ou terminarem
(evitando deadlock). Quando o sinal para terminar a aplicac ̧ao ̃ e recebido, a chamada  ́
“broadcast” garante que todas as threads auxiliares serao desbloqueadas para avaliar o  ̃
final da aplicac ̧ao, e todas as requisic ̧  ̃ oes pendentes ser  ̃ ao tratadas.  ̃
Quanto a efici ` encia: A chamada ˆ a func ̧ ` ao fazIteracao() est  ̃ a sendo feita fora da  ́
sec ̧ao cr  ̃  ́ıtica, permitindo que as threads possam executa-la em paralelo. Cada thread  ́
espera por uma requisic ̧ao, trata essa requisic ̧  ̃ ao e volta a esperar por nova requisic ̧  ̃ ao,  ̃
garantindo balanceamento de carga entre as threads auxiliares. Quando nao h  ̃ a requisic ̧  ́ oes  ̃
para atender, as threads auxiliares se bloqueiam, liberando a CPU para as outras threads.
A thread principal aguarda a entrada do usuario sem impedir que as threads auxiliares  ́
continuem trabalhando.
