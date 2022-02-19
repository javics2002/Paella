#pragma once
#include <vector>
#include "../View/Texture.h"
#include "../SDL2-2.0.20/Nuklear-master/nuklear.h"
#include "../GameObjects/GameObject.h"
#include "../UiButton.h"

#define NK_IMPLEMENTATION
class Paella;
class Ingrediente;
class Game;
class Ingredientebutton;
using namespace std;
using uint = uint32_t;
using Iconos = char;
class Comanda:public GameObject
{
	//al a�adir paella se alarga un poco la comanda
	//hay 8 ingredientes distintos
public:
	enum Tama�o { Peque�a, Mediana, Grande };
	Comanda(Game* game,uint nmesa);//crear comanda

/*	~Comanda();No se puede definir todavia y no compila NO LO BORREIS*/
	void a�adiraPedido(TextureName i);
	void randomizaIconos();
	//void abrirComanda(int x,int y);
	void dibujaPedido();
	/*void cierraComanda();*/
	//void Ecribeinfoenjson();
	//Compara las paellas recibidas con el pedido y le pone la puntuaci�n
	//float Puntuacion(vector<Paella*> paellas);
private:
	vector<Point2D<double> >posicionesBotones;//vas seleccionando y vas borrando las ya usadas
	vector<TextureName> texturasIngredienes = { ingrAlcachofa, ingrCalamar, ingrCangrejo,
	ingrGamba, ingrLangosta, ingrMejillon, ingrPimientoRojo, ingrPimientoVerde, ingrPollo
	};
	int x;
	int y;
	int escritoX;
	int escritoY;
	int w;
	int h;
	int margenbotones=5;
	int anchobotones=25;
	uint numeroMesa;
	uint numeroPaellas;
	float puntuacion;
	Game* gamet;
	//Texture textura;
	//Por cada paella, un tama�o y un vector de ingredientes
	//vector<Tama�o> tama�os;
	//vector<vector<Ingrediente*>> ingredientes;
	//vector<Paella> paellas;//datos de la paella recogida en la comanda con la que se calcula la puntuacion
	vector<Iconos> Pedido;//lo que mostramos en la ui  de lo que vas a�adiendo a la comanda
	vector<Ingredientebutton*> teclado;
	
	//float calculaPuntuacion(Paella paella) ; definicion pendiente de la creacion e paella
	void update() override{};
};

