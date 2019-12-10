class Conteudo {
    int id, cont;
    Conteudo() {
        this.id = -1;
        this.cont = 0;
    }
}
class LE {
    private int leit, escr;
    LE () {
        this.leit = 0;
        this.escr = 0;
    }
    public synchronized void EntraLeitor (int id) {
        try {
            System.out.printf("Leitor %d vai tentar entrar...%n", id);
            while (this.escr > 0) {
                System.out.printf("Leitor %d vai se bloquear...%n", id);
                wait();
                System.out.printf("Leitor %d vai tentar voltar...%n", id);
            }
            this.leit++;
            System.out.printf("Leitor %d entrou; leitores: %d%n", id, this.leit);
        } catch (InterruptedException e) {return;}
    }

    public synchronized void SaiLeitor (int id) throws InterruptedException {
        this.leit--;
        if (this.leit == 0)
            notifyAll();
        System.out.printf("Leitor %d saiu; leitores: %d%n", id, this.leit);
        wait();
    }

    public synchronized void EntraEscritor (int id) {
        try {
            System.out.printf("Escritor %d vai tentar entrar...%n", id);
            while (this.leit > 0 || this.escr > 0) {
                System.out.printf("Escritor %d vai se bloquear...%n", id);
                wait();
                System.out.printf("Escritor %d vai tentar voltar...%n", id);
            }
            this.escr++;
            System.out.printf("Escritor %d entrou; escritores: %d%n", id, this.escr);
        } catch (InterruptedException e) {return;}
    }

    public synchronized void SaiEscritor (int id) throws InterruptedException {
        this.escr--;
        notifyAll();
        System.out.printf("Escritor %d saiu; escritores: %d%n", id, this.escr);
        wait();
    }
}

class Escritor extends Thread {
    LE monitor;
    Conteudo c;
    int id;

    Escritor(LE m, int id, Conteudo con) {
        this.monitor = m;
        this.id = id;
        this.c = con;
    }

    public void run () {
        try {
            for (;;) {
                this.monitor.EntraEscritor(this.id);
                this.c.cont++;
                this.c.id = this.id;
                System.out.printf("Escritor %d; c_id %d; c_cont %d%n",
                                  this.id, this.id, this.c.cont);
                sleep(1000);
                this.monitor.SaiEscritor(this.id);
            }
        } catch (InterruptedException e) {return;}
    }
}

class Leitor extends Thread {
    LE monitor;
    Conteudo c;
    int id;

    Leitor (LE m, int id, Conteudo con) {
        this.monitor = m;
        this.id = id;
        this.c = con;
    }

    public void run () {
        int conteudo_id, conteudo_cont;
        try {
            for (;;) {
                this.monitor.EntraLeitor(this.id);
                conteudo_id = this.c.id;
                conteudo_cont = this.c.cont;
                System.out.printf("Leitor %d; c_id %d; c_cont %d%n",
                                  this.id, conteudo_id, conteudo_cont);
                sleep(1000);
                this.monitor.SaiLeitor(this.id);
            }
        } catch (InterruptedException e) {return;}
    }
}

class LeitorEscritor {
    static final int E = 4;
    static final int L = 4;

    public static void main (String[] args) {
        LE monitor_le = new LE();
        Conteudo conteudo = new Conteudo();
        Leitor[] leitores = new Leitor[L];
        Escritor[] escritores = new Escritor[E];

        for (int i = 0; i < E; i++) {
            escritores[i] = new Escritor(monitor_le, i, conteudo);
            escritores[i].start();
        }
        for (int i = 0; i < L; i++) {
            leitores[i] = new Leitor(monitor_le, i, conteudo);
            leitores[i].start();
        }
    }
}
