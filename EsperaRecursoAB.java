/*Uma aplicac ̧ao possui threads do tipo A e do tipo B e um deter-  ̃
minado recurso usado por todas elas. Apenas threads do mesmo tipo podem acessar o

recurso ao mesmo tempo, threads do outro tipo devem aguardar ate que o recurso seja  ́

liberado. O codigo Java abaixo implementa a classe  ́ Recurso como um monitor para con-
trolar o acesso ao recurso compartilhado. As threads do tipo A devem chamar o metodo  ́

EntradaA antes de usar o recurso e o metodo  ́ SaidaA apos terminarem de usar o recurso.  ́
As threads B devem fazer o mesmo usando os metodos  ́ EntradaB e SaidaB. Verificou-se

que ha um erro l  ́ ogico nesse c  ́ odigo que pode levar a aplicac ̧  ́ ao a uma situac ̧  ̃ ao de  ̃ dead-
lock. Mantendo a mesma estrutura basica do c  ́ odigo, fac ̧a as correc ̧  ́ oes necess  ̃ arias para 
evitar a possibilidade de deadlock e garantir ausencia de ˆ starvation. Comente seu codigo.

class Recurso {
int esperandoA=0, esperandoB=0; //qtde threads A e B esperando pelo recurso
int usandoA=0, usandoB=0; //qtde threads A e B usando o recurso
Recurso() {//construtor}
public synchronized void EntradaA () {
while((usandoB>0) || (esperandoB>0)) {
esperandoA++; wait(); esperandoA--;
} usandoA++;
}
public synchronized void SaidaA () {
usandoA--;
if((usandoA == 0) && (esperandoB > 0))
notifyAll();
}
public synchronized void EntradaB ()
while((usandoA>0) || (esperandoA>0)) {
esperandoB++; wait(); esperandoB--;
} usandoB++;
}
public synchronized void SaidaB () {
usandoB--;
if((usandoB==0) && (esperandoA>0))
notifyAll();
} }*/
  
  
Resp.:

class Recurso {
  int esperandoA=0, esperandoB=0; //qtde threads A e B esperando pelo recurso
  int usandoA=0, usandoB=0; //qtde threads A e B usando o recurso
  int vezA, vezB; //sinaliza a vez de cada tipo de thread usar o recurso
  Recurso() {//construtor}
  public synchronized void EntradaA () {
    while((usandoB>0) || (esperandoB>0)) {
      esperandoA++; wait(); esperandoA--;
      if(vezA > 0) { //permite que as threads A usem o recurso
        vezA--; break;
      }
    } usandoA++;
  }
  public synchronized void SaidaA () {
    usandoA--;
    if(usandoA == 0) {
      vezA=0;
      if (esperandoB > 0)) {

        vezB = esperandoB;
        notifyAll();
      }}
  }
    
  public synchronized void EntradaB ()
    while((usandoA>0) || (esperandoA>0)) {
      esperandoB++; wait(); esperandoB--;
        if(vezB > 0) { //permite que as threads B usem o recurso
        vezB--; break;
      }
    } usandoB++;
  }
    public synchronized void SaidaB () {
      usandoB--;
      if(usandoB == 0) {
        vezB=0;
        if (esperandoA > 0)) {
          vezA = esperandoA;
          notifyAll();
        }}
    }
}
