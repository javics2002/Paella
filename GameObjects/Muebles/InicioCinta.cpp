#include "InicioCinta.h"
#include "../../Control/Game.h"
#include "../../Control/ObjectManager.h"
#include "../../Scenes/Tutorial.h"
#include "../../Control/NetworkManager.h"
#include "../../Utils/ParticleExample.h"
#include "../Herramienta.h"

InicioCinta::InicioCinta(Game* mGame, Vector2D<double> pos) : Mueble(mGame, pos, TILE_SIZE, TILE_SIZE, "inicioCinta")
{
	initTime = 0;
	funcionando = true;
}

void InicioCinta::update()
{
	if (!funcionando)
	{
		humo->setStyle(ParticleExample::SMOKE);
	}
	else 
	{
		humo->setStyle(ParticleExample::NONE);
	}

	humo->setPosition(getX(), getY());
	humo->update();

	if (!mGame->getNetworkManager()->isHost())
		return;

	int i = rand() % 1000;
	if (sdlutils().virtualTimer().currTime() - initTime >= SPAWN_DELAY && isActive() && funcionando)
	{
		if (i < porcentajeLetal && !dynamic_cast<Tutorial*>(mGame->getCurrentScene()))
		{
			IngredienteLetal* ingLetal = mGame->getObjectManager()->getPool<IngredienteLetal>(_p_INGREDIENTELETAL)->add(getPosition());
			ingLetal->setVel(vel);
			initTime = sdlutils().virtualTimer().currTime();

			mGame->getNetworkManager()->sendCreateIngredienteLetal(ingLetal->getTipoLetal(), getPosition(), vel);
		}
		else {
			Ingrediente* ing = mGame->getObjectManager()->getPool<Ingrediente>(_p_INGREDIENTE)->add(getPosition());
			ing->setVel(vel);
			initTime = sdlutils().virtualTimer().currTime();

			ing->setId(idCont);
			idCont++;

			mGame->getNetworkManager()->sendCreateIngrediente(ing->getTipo(), ing->getId(), getPosition(), vel);
		}
	}

	if(!dynamic_cast<Tutorial*>(mGame->getCurrentScene())){
		if (funcionando && couldBreak <= 0)
		{
			testMueble();
			if (funcionando)
			{
				//se reduce cuando se podr�a romper
				couldBreak = MAX_BREAK_TIME - REDUCE_BREAK_TIME;
			}
			else
			{
				//se resetea cuando se podr�a romper
				couldBreak = MAX_BREAK_TIME;
				humo->setStyle(ParticleExample::EXPLOSION);
			}
		}
		else if (funcionando && couldBreak > 0)
		{
			couldBreak -= seg;
		}
	}

	
}

void InicioCinta::render(SDL_Rect* cameraRect)
{
	SDL_Rect dest = { getX() - getWidth() / 2, getY() - getHeight() / 2, getWidth(),
	   getHeight() };
	if (isActive()) {

		//if (funcionando)drawRender(cameraRect, dest, &sdlutils().images().at("inicioCinta"));
		//else drawRender(cameraRect, dest, &sdlutils().images().at("berenjena"));
		drawRender(cameraRect, dest, &sdlutils().images().at("inicioCinta"));

		humo->draw(cameraRect);
	}
}

SDL_Rect InicioCinta::getCollider()
{
	if(isActive())return getTexBox();
	return { 0,0,0,0 };
}

bool InicioCinta::resetCounter()
{
	couldBreak = MAX_BREAK_TIME;
	return true;
}

bool InicioCinta::receiveHerramienta(Herramienta* h)
{
	if (!funcionando)
	{
		if (dynamic_cast<Tutorial*>(mGame->getCurrentScene()) && mGame->getCurrentScene()->getState() == TUTORIALSTATE_ARREGLAR_CINTA)
			mGame->getCurrentScene()->changeState(TUTORIALSTATE_PAUSA_ARREGLAR_CINTA);

		funcionando = true;
		h->setActive(false);
		return true;
	}
	return false;
}

SDL_Rect InicioCinta::getOverlap()
{
	return { int(getX() -getWidth()),
		int(getY() - getHeight()),
		(getWidth() *2),
		(getHeight() * 2) };
}
