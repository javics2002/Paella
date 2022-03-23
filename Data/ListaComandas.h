#pragma once
#include "../GameObjects/GameObject.h"
#include <list>
#include <vector>
#include<queue>

class Comanda;
class UIManager;
class Game;

using namespace std;

class ListaComandas :public GameObject
{
	int maxvisibles = 4;
	int  numcomandas=0;
	int limit = 0;
	list<Comanda*> lista;
	queue<Comanda*> listanovisibles;
	Comanda* selected = nullptr;
public:
	//A�ade la comanda que escribe el camarero a la lista
	ListaComandas(Game* game, UIManager* m);
	~ListaComandas();
	void A�adeComanda(Comanda* comanda);
	void finalizacomanda(Comanda* comanda);
	void seleccionaComanda(Comanda* comanda);
	Comanda* getComandaSeleccionada();
	//Interfaz
	void renderComandas();
	void update() override;
	list<Comanda*> getlista() { return lista; };
private:
	int ancho =600;
	int alto = 50;
	int cX = 0;
	int cY = 0;
	int desplazamineto = 0;
	int inicx = 0;
	UIManager* uimt;
};

