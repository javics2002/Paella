#include "Cliente.h"
#include "../../Control/game.h"

Cliente::Cliente(Game* game) : PoolObject(game)
{
	setDimension(DIMENSION, DIMENSION);
	vel.setX(2.0f);
}

void Cliente::update()
{
	setPosition(getX() + vel.getX(), getY());

}

void Cliente::cambiaTextura(string textureN)
{
	setTexture(textureN);
}

void Cliente::clienteRecogido()
{
	vel = { 0,0 };
}

