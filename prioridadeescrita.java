/*Implemente uma nova versao para essa classe/monitor com  ̃ prioridade para a escrita 
(leitores nao podem iniciar nova leitura quando h  ̃ a escritores esperando para escrever).*/

class LE {
private int leit, escr, escresp;
LE() {
this.leit = 0; //numero de leitores lendo
this.escr = 0; //numero de escritores escrevendo (0 ou 1)
this.escresp = 0; //numero de escritores esperando
}
// Entrada para leitores

public synchronized void EntraLeitor (int id) {
try {
while (this.escr > 0 || this.escresp > 0) {
wait(); //bloqueia se houver escritores esperando ou escrevendo
}
this.leit++;
} catch (InterruptedException e) { }
}
// Saida para leitores
public synchronized void SaiLeitor (int id) {
this.leit--;
if (this.leit == 0)
notifyAll(); //libera os escritores
//(a fila da variavel de condicao pode ter leitores e escritores)

}
// Entrada para escritores
public synchronized void EntraEscritor (int id) {
try {
this.escresp++; //sinaliza que quer escrever
while ((this.leit > 0) || (this.escr > 0)) {
wait(); //bloqueia se houver leitores lendo ou outro escritor escrevendo
}
this.escresp--;
this.escr++;
} catch (InterruptedException e) { }
}
// Saida para escritores
public synchronized void SaiEscritor (int id) {
this.escr--;
notifyAll(); //libera os leitores e outros escritores
}
}
