#include "Ingrediente.h"

#include <iostream>

Ingrediente::Ingrediente(Game* game) : ObjetoPortable(game) {
	setDimension(DIMENSION, DIMENSION);
}

void Ingrediente::update()
{
	setPosition(getX(), getY() + vel.getY());
}

void Ingrediente::onActivate()
{
	int n = rand() % (tipoIngrediente::LAST) ; // Se le resta uno a LAST porque el arroz no debe salir en la
													  // cinta (siendo arroz = 0).

	vel = { 0, 1.3 };
	
	miTipo = tipoIngrediente(n);

	setTexture(texturaIngrediente[n]);
}

void Ingrediente::onDeactivate()
{
}

void Ingrediente::ingredienteRecogido()
{
}

void Ingrediente::setProcesado(bool estadoIngr, Ingrediente *ingr)
{
	procesado = estadoIngr;

	setTexture(texturaIngrProcesado[ingr->miTipo]);
}


bool Ingrediente::getProcesado()
{
	return procesado;
}

tipoIngrediente Ingrediente::getTipo()
{
	return miTipo;
}

void Ingrediente::onObjectPicked()
{
	vel = { 0,0 };
}

void Ingrediente::onObjectDropped()
{
}



