#include "Silla.h"
#include "../../Control/Game.h"
#include "../../Control/ObjectManager.h"

#include "Mesa.h"

Silla::Silla(Game* mGame, Vector2D<double> pos, string texture) : Mueble(mGame, pos, TILE_SIZE, 2 * TILE_SIZE, texture)
{
	mMesa = nullptr;

	//Siempre tiene que funcionar
	funcionando = true;
}

Silla* Silla::initMesa(Mesa* mesa)
{
	mMesa = mesa;

	return this;
}

bool Silla::receiveGrupoClientes(GrupoClientes* gc)
{
	if (mMesa != nullptr) {
		return mMesa->receiveGrupoClientes(gc);
	}
}

bool Silla::receivePaella(Paella* paella)
{
	if (mMesa != nullptr) {
		return mMesa->receivePaella(paella);
	}	
}

bool Silla::returnObject(Player* p)
{
	if (mMesa != nullptr) {
		return mMesa->returnObject(p);
	}
}

void Silla::decirPedido()
{
	mMesa->decirPedido();
}

SDL_Rect Silla::getCollider()
{
	SDL_Rect rect = getTexBox();

	if (isActive()) return {
		rect.x,
		rect.y + rect.h / 2,
		rect.w,
		rect.h / 2
	};
	return { 0,0,0,0 };
}

SDL_Rect Silla::getOverlap()
{
	SDL_Rect rect = getTexBox();

	int incr = 10;

	return {
		rect.x - incr,
		rect.y - incr,
		rect.w + incr * 2,
		rect.h + incr * 2
	};
}

