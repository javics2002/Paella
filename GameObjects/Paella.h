#pragma once
#include "ObjetoPortable.h"
#include "Ingrediente.h"
#include "../Control/ObjectManager.h"
#include <list>


enum EstadoPaellas { Preparacion, Coccion, Preparada };
enum Resultado { Cruda, PocoHecha, Perfecta, MuyHecha, Quemada, Incomestible };
enum Contenido { Limpia, Entera, Mitad, Sucia };
enum TipoPaella { Minima, Mediana, Grande };

class Game;

class Paella : public ObjetoPortable
{	

public:
	
	Paella(Game* game, TipoPaella volumen_);
	
	~Paella() {};

	bool ingrValido(Ingrediente* ingr);
	void anadeIngr(Ingrediente* ingr_,bool arroz);

	void setState(EstadoPaellas estado_);
	void paellaRecogida();
	void update() override;
	void setLavado(Contenido contenidoPaella,string texturaPaella);

	void setEnsuciada();
	void setContenido(Contenido contenidoP);

	void onObjectPicked() override;
	void onObjectDropped() override;

	bool canPick() override;
	bool conArroz();
	void enLaMesa(bool estaEnLaMesa);

	list<tipoIngrediente> getVIngredientes();
	vector<bool> getIngrPaella();

	EstadoPaellas getState();
	TipoPaella getTipo();
	Contenido getContenido();
	Resultado getResultado();

private:
	const int MAX_INGR = 3;
	double tiempoCoccion = 0.0, tiempo = 0.0;
	int sumaIntervalo = 0, i = 0;
	bool enMesa = false;
	bool arroz_ = false;

	EstadoPaellas estado = Preparacion;
	Resultado estadoFinal = Cruda;
	Contenido contenido = Limpia;

	TipoPaella miTipo;

	vector<int> tiemposDeCoccion = { 14000, 20000, 25000, 28000, 33000, 38000 };

	list<tipoIngrediente> ingredientes;

	vector<bool> ingrEnPaella;

	
};

