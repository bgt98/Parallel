/*(2.5pts)  ̃ Para danc ̧ar a quadrilha de Sao Jo  ̃ ao ̃ e preciso formar um par (um homem e uma  ́
mulher). Considere uma aplicac ̧ao com threads  ̃ homem e mulher. Escreva uma classe Java thread-safe
(monitor) que oferec ̧a os metodos  ́ formaParM() e formaParH() os quais deverao ser chamados  ̃
pelas threads mulher e homem, respectivamente, antes de entrarem na quadrilha. O metodos devem  ́
garantir que cada thread bloqueie ate formar um par e que as threads deixem o monitor sempre em pares. */
  
 ́
  class Pares {
  private int m, h, formandoParM, formandoParH;
  Pares() {
  this.m = 0; //numero de mulheres esperando um par
  this.h = 0; //numero de homens esperando um par
  this.formandoParM = 0;
  //se==1, uma mulher j ́a foi selecionada para o proximo par
  this.formandoParH = 0;
  //se==1, um homem j ́a foi selecionado para o proximo par
  }
  // Barreira para mulheres
  public synchronized void formaParM (int id) {
    try {
      this.m++;
      while ((this.h == 0) || (this.formandoParM == 1)) {
        wait(); //bloqueia pela condicao logica da aplicacao
      }
      this.formandoParM = 1; //uma mulher foi selecionada para formar um par
      if(this.formandoParH == 0) {
        notifyAll(); //desbloqueia os homens esperando par
      } else {
      this.m--; this.h--; //um par foi formado
      this.formandoParM = 0; this.formandoParH = 0; //termina a formacao do par
      }
    } catch (InterruptedException e) { } 
  }
  ∗Lembre-se de comentar todos os codigos e  ́ boa prova!

  // Barreira para homens
  public synchronized void formaParH (int id) {
    try {
      this.h++;
      while ((this.m == 0) || (this.formandoParH == 1)) {
        wait(); //bloqueia pela condicao logica da aplicacao
      }
      this.formandoParH = 1; //um homem foi selecionado para formar um par
      if(this.formandoParM == 0) {
        notifyAll(); //desbloqueia as mulheres esperando par
      } else {
        this.m--; this.h--; //um par foi formado
        this.formandoParM = 0; this.formandoParH = 0; //termina a formacao do par
      s}
    } catch (InterruptedException e) { }
  }
}

