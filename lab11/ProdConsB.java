// Monitor
class BufferB {
    static final int N = 10;    //tamanho do buffer
    private int[] buffer = new int[N];  //reserva espaco para o buffer
    private int count=0, in=0, out=0;   //variaveis compartilhadas

    // Construtor
    BufferB() {
        for (int i=0;i<N;i++)  buffer[i] = -1;
    } // inicia o buffer

    // Insere um item
    public synchronized void Insere (int item) {
        try {
            while (count == N) {
                System.out.println("Produtora " + item + " vai se bloquear.");
                wait();
                System.out.println("Produtora " + item + " vai tentar voltar do bloqueio.");
            }
            // System.out.println("Produtora " + item + " voltou do bloqueio.");
            this.buffer[this.in] = item;
            this.in = (this.in + 1) % N;
            this.count++;
            System.out.println("Produtora " + item + " inseriu sua id. count: " + this.count);
            if(this.count == N)
                notifyAll();
        } catch (InterruptedException e) { }
    }

    // Remove um item
    public synchronized int[] Remove (int id) {
        int aux[] = new int[N];
        try {
            while (count != N) {
                System.out.println("Consumidora " + id + " vai se bloquear.");
                wait();
                System.out.println("Consumidora " + id + " vai tentar voltar do bloqueio.");
            }
            // System.out.println("Consumidora " + id + " voltou do bloqueio.");
            // aux = this.buffer[this.out];
            // this.out = (this.out + 1) % N;
            // this.count--;
            for (int i = 0; i < N; i++) {
                aux[i] = this.buffer[this.out];
                this.out = (this.out + 1) % N;
                this.count--;
                System.out.println("Consumidora " + id + " retirou " + aux[i] + ". count: " + this.count);
            }
            notifyAll();
            return aux;
    } catch (InterruptedException e) { return new int[N]; }
    }
}

//--------------------------------------------------------
// ConsumidorB
class ConsumidorB extends Thread {
    int id;
    int delay;
    BufferB buffer;

    // Construtor
    ConsumidorB (int id, int delayTime, BufferB b) {
        this.id = id;
        this.delay = delayTime;
        this.buffer = b;
    }

    // Método executado pela thread
    public void run () {
        int item[];
        try {
            for (;;) {
                item = this.buffer.Remove(this.id);
                // System.out.println("Thread " + this.id + " removeu " + item + " do buffer.");
                sleep(this.delay); //...simula o tempo para fazer algo com o item retirado
            }
        } catch (InterruptedException e) { return; }
    }
}

//--------------------------------------------------------
// ProdutorB
class ProdutorB extends Thread {
    int id;
    int delay;
    BufferB buffer;

    // Construtor
    ProdutorB (int id, int delayTime, BufferB b) {
        this.id = id;
        this.delay = delayTime;
        this.buffer = b;
    }

    // Método executado pelo thread
    public void run () {
        try {
            for (;;) {
                this.buffer.Insere(this.id); //simplificacao: insere o proprio ID
                // System.out.println("Thread " + this.id + " inseriu sua id no buffer.");
                sleep(this.delay);
            }
        } catch (InterruptedException e) { return; }
    }
}

//--------------------------------------------------------
// Classe principal
class ProdConsB {
    static final int P = 4;
    static final int C = 4;

    public static void main (String[] args) {
        int i;
        BufferB buffer = new BufferB();      // Monitor
        ConsumidorB[] cons = new ConsumidorB[C];   // Consumidores
        ProdutorB[] prod = new ProdutorB[P];       // Produtores

        for (i=0; i<C; i++) {
            cons[i] = new ConsumidorB(i+1, 1000, buffer);
            cons[i].start();
        }
        for (i=0; i<P; i++) {
            prod[i] = new ProdutorB(i+1, 1000, buffer);
            prod[i].start();
        }
    }
}
