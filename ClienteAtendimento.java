/*Implemente um monitor em Java para simular uma fila de atendi-
mento em um banco. O metodo  ́ EsperaAtendimento devera ser executado pelos  ́

clientes para entrar na fila de espera por atendimento. O cliente so pode entrar nessa fila  ́
se o numero de clientes j  ́ a esperando na mesma fila for menor que um dado valor M.  ́
Os metodos  ́ EsperaCliente e TerminaCliente deverao ser executados pelo fun-  ̃
cionario do banco para aguardar clientes na sua fila e informar que o cliente atual acabou  ́
de ser atendido, respectivamente. Cada fila deve ser associada a um unico funcion  ́ ario.  ́
Os clientes devem ser atendidos na ordem de chegada na fila. Comente seu codigo.*/


  class CaixaBanco {
  private int max;
  private int esperando;
  private int ocupado;
  private int proximoCliente;
  private int ultimoCliente;
  // Construtor
  CaixaBanco(int max) {
    this.max = max; //numero maximo de clientes na fila do caixa
    this.esperando = 0; //numero de clientes esperando
    this.ocupado = 0; //estado do caixa (0: livre; 1: ocupado)
    this.proximoCliente = 0; //senha do cliente no inicio da fila de espera
    this.ultimoCliente = 0; //proxima senha do cliente que entrar na fila de espera
  }
  // Metodo dos clientes
  public synchronized boolean EsperaAtendimento (int id) {
    if(this.esperando == this.max) {
    return false;
    }
    try {
      int minhaSenha;
      this.esperando++;
      minhaSenha = this.ultimoCliente;
      this.ultimoCliente++;
      while(this.proximoCliente != minhaSenha) wait(); //deve aguardar
      this.ocupado = 1;
      this.clienteAtual = id;
      notifyAll(); //para acordar o caixa
    } catch (InterruptedException e) {return false; }
    return true;
  }
  // Metodo do funcionario para esperar novo cliente
  public synchronized void EsperaCliente () {
    try {
      while((this.esperando == 0) || (this.ocupado == 0)) wait();
      this.esperando--;
    } catch (InterruptedException e) { }
  }
  // Metodo do funcionario para terminar um cliente
  public synchronized void TerminaCliente () {
    this.ocupado = 0;
    this.proximoCliente++;
    if(this.esperando > 0) notifyAll();
  }
}
