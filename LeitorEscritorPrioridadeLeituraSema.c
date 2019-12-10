/* Usando semáforos, escreva uma solução concorrente para o proble-
ma dos leitores e escritores (com prioridade para leitura). As condições do problema são:

(i) os leitores podem ler simultaneamente a região de dados compartilhada; (ii) apenas
um escritor pode escrever a cada instante na região de dados compartilhada. (iii) se um
escritor está escrevendo, nenhum leitor pode ler a mesma região de dados compartilhada.*/


int num_leitores; //registra o numero de leitores ativos
sem em=1; //semaforo binario para exclusao mutua entre

//leitores para atualizacao de variaveis compartilhadas
sem wsem=1; //semaforo binario para exclusao mutua para escrita
void leitor() {
  while(true) {
    semWait(em);
    num_leitores++;
    //o primeiro leitor verifica ausencia de escritores
    if(num_leitores==1) {
      semWait(wsem);
    }
    semSignal(em);
    le_dados();
    semWait(em);
    num_leitores--;
    if(num_leitores==0) { //o ultimo leitor libera a escrita
      semSignal(wsem);
    }
    semSignal(em);
  }
}

void escritor() {
  while(true) {
    semWait(wsem);
    escreve_dados();
    semSignal(wsem);
  }
}
void main() {
  num_leitores=0;
  cria_thread(leitor); //cria as threads leitoras
  cria_thread(escritor); //cria as threads escritoras
}
