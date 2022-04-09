#include "Cinta.h"

Cinta::Cinta(Game* game, Vector2D<double> pos) : Mueble(game, pos, TILE_SIZE,  TILE_SIZE, "cinta")
{
	//Este sonido es m�s bajo
	sdlutils().soundEffects().at("cinta").setVolume(32);
	canalSonido = sdlutils().soundEffects().at("cinta").play(-1);
}

Cinta::~Cinta()
{
	sdlutils().soundEffects().at("cinta").haltChannel(canalSonido);
}

SDL_Rect Cinta::getCollider()
{
	return cintaCollider;
}

void Cinta::setCollider(SDL_Rect r)
{
	cintaCollider = r;
}


SDL_Rect Cinta::getOverlap()
{
	return SDL_Rect();
}
