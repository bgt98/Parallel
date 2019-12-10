// Monitor
class Bandeija {
static final int N = 2; //tamanho da bandeija
int[] bandeija = new int[N]; //reserva espaco para a bandeija
int count=0, in=0, out=0; //variaveis compartilhadas
Bandeija () { for (int i=0;i<N;i++) bandeija[i] = 0; }
// Insere um item
public synchronized void Insere (int item) {
try {
while (count==N) {
//bloqueia pela condicao logica da aplicacao
wait();
}
// insere item
bandeija[in%N] = item;
in++;
count++;
notifyAll(); //para desbloquear um consumidor bloqueado
} catch (InterruptedException e) { }
}
// Remove um item
public synchronized int Remove (int id) {
int aux;
try {
while (count==0) {
//bloqueia pela condicao logica da aplicacao
wait();
}
// remove item
aux = bandeija[out%N];
bandeija[out%N] = 0; //0 considerado como vazio
out++;
count--;
notifyAll(); //para desbloquear um produtor bloqueado
return aux;
} catch (InterruptedException e) { return -1;}
}
}
