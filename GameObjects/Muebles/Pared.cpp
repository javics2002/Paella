#include "Pared.h"
#include "../../Control/Game.h"
#include "../../Control/ObjectManager.h"

Pared::Pared(Game* game, Vector2D<double> position) : Mueble(game, position, TILE_SIZE, TILE_SIZE, "cartel")
{
	setDimension(64, 64);
}

Pared::~Pared()
{
}



void Pared::update()
{
	//for (auto i : game->getObjectManager()->getPoolGrupoClientes()->getCollisions(getCollider())) {
	//	i->colisionClientes();
	//}
}

void Pared::render(SDL_Rect* cameraRect)
{
}

//bool Pared::colisionPlayer(Player* p)
//{
//	p->setVel(0, 0);
//	return false;
//}
