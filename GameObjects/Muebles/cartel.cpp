#include "cartel.h"
#include "../../Control/game.h"
#include "../../Control/ObjectManager.h"

Cartel::Cartel(Game* game, Vector2D<double> position) : Mueble(game, position, TILE_SIZE, TILE_SIZE * 1.8, "cartel")
{
	//Siempre tiene que funcionar
	funcionando = true;
}

void Cartel::update()
{
	for (auto i : game->getObjectManager()->getPool<GrupoClientes>(_p_GRUPO)->getCollisions(getCollider())) 
		i->colisionClientes();	
}

SDL_Rect Cartel::getOverlap()
{
	return SDL_Rect();
}

SDL_Rect Cartel::getCollider()
{
	return getTexBox();
}
