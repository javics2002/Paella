#pragma once
#include "../UI/UiButton.h"
class Game;
class Ingredientebutton;
class Comanda;
using namespace std;

class UIManager
{
	vector<string> texturasIngredienes = { "alcachofa",  "calamar", "cangrejo", "gamba",
		   "langosta", "mejillon", "pimientoRojo", "pimientoVerde", "pollo" };
	vector<Point2D<double> >posicionesBotones;
	vector<GameObject*> interfaz;//el resto de iconos 
	vector<Comanda*> comandas;//comandas de la lista de comandas
	vector<	Ingredientebutton*> teclado;//iconos que se usan en la comanda qeu esta redactando
	//vector <GameObject*> pedido;//pedido que se est� redactando lo lleva la comanda
	int mx;
	int my;
	int anchobotones = 25;
	Game* gamet;
	Comanda* actual;
public:
	UIManager(Game* game);
	~UIManager();
	void uiEvent(int mx, int my);
	void update();

	void render();
	void render(SDL_Rect& rect);

	void creaComanda(Game* game);
	Comanda* getComanda();
	vector< Point2D<double>> getPosTeclado();
	void setPosTeclado(vector< Point2D<double>> t);
	void creaTeclado();
	void randomizaTeclado();
};

