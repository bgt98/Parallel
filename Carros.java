/*Carros vindos do norte e do sul chegam em uma ponte de uma unica pista, apenas carros na mesma  ́
direc ̧ao podem compartilhar a ponte, carros em direc ̧  ̃ ao oposta devem esperar at  ̃ e a ponte ficar liberada. O  ́ ultimo carro a  ́
cruzar a ponte libera a travessia para os carros aguardando na direc ̧ao oposta. O c  ̃ odigo Java abaixo implementa uma soluc ̧  ́ ao ̃
para esse problema. Verifique se a soluc ̧ao est  ̃ a correta. Se sim, verifique se ela garante aus  ́ encia de ˆ starvation. Justifique
suas respostas.*/

class Ponte {
int sulE=0, norteE=0, norteP=0, sulP=0; Ponte() {//construtor}
public synchronized void EntraSul (int id) {
while((norteP>0) || (norteE>0)) { sulE++; wait(); sulE--; } sulP++; }
public synchronized void SaiSul (int id) {
sulP--; if((sulP == 0) && (norteE > 0)) notifyAll(); }
public synchronized void EntraNorte (int id)
while((sulP>0) || (sulE>0)) { norteE++; wait(); norteE--; } norteP++; }
public synchronized void SaiNorte (int id) {
norteP--; if((norteP==0) && (sulE>0)) notifyAll(); } }
Resp.: O codigo N  ́ AO est  ̃ a correto pois pode entrar em deadlock. Se carros do sul chegam primeiro, eles comec ̧am  ́
a atravessar a ponte. Quando o primeiro carro do norte chegar, ele ficara bloqueado. Em seguida, todos os carros do sul  ́
que chegarem tambem ficar  ́ ao bloqueados. Quando o  ̃ ultimo carro do sul sair da ponte, ele desbloquear  ́ a todos os carros  ́
bloqueados. Ate aqui tudo certo. Entretanto, como as vari  ́ aveis  ́ norteE e sulE estarao diferentes de zero, nenhum carro  ̃
consequira mais atravessar a ponte pois todos ficar  ́ ao bloqueados.
