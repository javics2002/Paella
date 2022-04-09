#include "Cliente.h"
#include "../../Control/game.h"

Cliente::Cliente(Game* game) : GameObject(game)
{
	setDimension(DIMENSION, DIMENSION);
}

void Cliente::update()
{
	setPosition(pos+vel);
}

void Cliente::cambiaTextura(string textureN)
{
	setTexture(textureN);
}

