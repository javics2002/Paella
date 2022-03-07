#include "cartel.h"
#include "../../Control/game.h"
#include "../../Control/ObjectManager.h"

Cartel::Cartel(Game* game, Vector2D<double> position) : Mueble(game, position, TILE_SIZE, TILE_SIZE, "cartel")
{
}

void Cartel::update()
{
	for (auto i : game->getObjectManager()->getGrupoClientes(getCollider())) {
		i->colisionClientes();
	}
}
