*/Considere um recurso compartilhado com as seguintes características: 
(i) enquanto o número de processos usando o recurso é menor
que três, novos processos podem usar o recurso imediatamente; 
(ii) uma vez que há três
processos usando o recurso, todos os três devem liberar o recurso antes de qualquer outro
processo poder usá-lo. 

Para a solução do problema mostrada abaixo: (a) Explique como
o programa funciona (ideia geral da solução e significado de cada linha). (b) Mostre que
ele está correto ou aponte pelo menos um erro./*


1: semaphore mutex=1, block=0;

2: int active=0, waiting=0;
3: boolean must_wait=false;
4:
5: semWait(mutex);
6: if(must_wait) {
7: ++waiting;
8: semSignal(mutex);
9: semWait(block);
10: --waiting;
11: }
12: ++active;
13: must_wait = (active==3);
14: if(waiting>0 && !must_wait)
15: { semSignal(block); }
16: else { semSignal(mutex); }
17:
18: /* secao critica */
19:
20: semWait(mutex);
21: --active;
22: if(active==0) { must_wait=false; }
23: if(waiting>0 && !must_wait)
24: { semSignal(block); }
25: else { semSignal(mutex); }
Resp.: (a) Dois semáforos são usados para sincronizar os processos: um semáforo
binário (mutex) é usado para acessar e alterar as variáveis compartilhadas, garantindo
a exclusão mútua; e um semáforo contador (block) é usado para bloquear os processos
caso a condição lógica para prosseguirem a execução não seja atendida. Uma variável
é usada para contar o número de processos ativos (active), e outra variável é usada

para manter o número de processos aguardando pelo recurso (waiting). A última vari-
ável (must_wait) é usada para indicar se o processo deve bloquear ou não. A solução

usa a técnica de passagem de bastão: quando um processo é desbloqueado ele desen-
cadeia o desbloqueio dos demais processos (caso haja outros bloqueados), até o limite

das restrições do problema. O processo aloca o semáforo de exclusão mútua (linha 5)
para checar se deve aguardar pelo recurso (linha 6). Se sim, o número de processos
aguardando é incrementado (linha 7), o semáforo de exclusão mútua é liberado (linha

8), e o processo se bloqueia (linha 9). Se não, o número de processos ativos é incre-
mentado (linha 12), caso o número de processos ativos tenha alcançado o valor três,

a variável booleana (must_wait) é ativada para impedir o acesso ao recurso por novos

processos (linha 13). O processo libera o semáforo de exclusão mútua (linha 16) e e-
xecuta a sua seção crítica (linha 18). Quando finaliza, ele aloca novamente o semáforo

de exclusão mútua (linha 20) para atualizar as variáveis compartilhadas. O número de
processos ativos é decrementado (linha 21). Caso esse número chegue a 0, a variável
booleana (must_wait) é desativada, permitindo novamente o acesso ao recurso por novos
processos (linha 22). Se existir processos esperando pelo recurso e o acesso ao recurso
estiver liberado, um desses processos é desbloqueado, desencadeando o desbloqueio de
outros processos até o limite de processos ativos simultaneamente (linhas 14 e 15). Caso
contrário, apenas o semáforo de exclusão mútua é liberado (linha 25).
(b) O programa está correto: todos os acessos para variáveis compartilhadas são
protegidos, os processos não bloqueiam eles mesmos dentro da exclusão mútua, novos

processos são impedidos de usar o recurso se há três usuários ativos, e os processos
esperando pelo recurso conseguem usá-lo em algum momento.
