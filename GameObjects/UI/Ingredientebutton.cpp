#include "Ingredientebutton.h"
#include "../UI/UIManager.h"
Ingredientebutton::Ingredientebutton(UIManager* uim,Game* game,string texturename, int x, int y, int w, int h):UiButton(game,texturename,x,y,w,h)
{

	gamet = game;
	//comandat = comanda;
	uimt = uim;
	textuname = texturename;

}
Ingredientebutton::~Ingredientebutton()
{
}
void Ingredientebutton::execute()
{
	uimt->getComanda()->a�adiraPedido(textuname);
	uimt->randomizaTeclado();
	
	//comandat->a�adiraPedido(textuname);
	//HAY QUE CORTAR EL BUCLE DE INTERAZ AQUI POR QEU SI NO SE A�ADEN 6 INGREDIENTES AL PULDSAR UN BOTON XD
	//well podria ser un while en ligar d eun for
}
void Ingredientebutton::update()
{
}
