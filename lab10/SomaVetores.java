class Vetor {
  double elementos[];

  public Vetor(int tam) {
    this.elementos = new double[tam];
  }

  public void init(double val) {
    for (int i = 0; i < this.elementos.length; i++) {
      this.elementos[i] = val;
    }
  }

  public void imprime() {
    for (int i = 0; i < this.elementos.length; i++) {
      System.out.printf("[%d]: %f - ", i, this.elementos[i]);
    }
  }

  public int tam() {
    return this.elementos.length;
  }

  public double acessa(int i) {
    return this.elementos[i];
  }

  public void insere(int i, double elem) {
    this.elementos[i] = elem;
  }
}

class Somadora extends Thread {
    int id;
    Vetor a, b, c;

    public Somadora(int tid, Vetor a, Vetor b, Vetor c) {
        this.a = a;
        this.b = b;
        this.c = c;
        this.id = tid;
    }

    public void run() {
        double soma;
        double t = SomaVetores.nThreads;

        for (int i = id; i < this.c.tam(); i += t) {
            soma = this.a.acessa(i) + this.b.acessa(i);
            this.c.insere(i, soma);
        }
    }
}

class SomaVetores {
    static final int nThreads = 4;

    public static void main(String[] args) {
        Thread[] threads = new Thread[nThreads];

        Vetor a = new Vetor(20);
        Vetor b = new Vetor(20);
        Vetor c = new Vetor(20);

        a.init(2);
        b.init(3);

        a.imprime();
        b.imprime();

        for (int i = 0; i < threads.length; i++) {
            threads[i] = new Somadora(i, a, b, c);
        }

        for (int i = 0; i < threads.length; i++) {
            threads[i].start();
        }

        for (int i = 0; i < threads.length; i++) {
            try {
                threads[i].join();
            } catch (InterruptedException e) {
                return;
            }
        }

        c.imprime();
    }
}
