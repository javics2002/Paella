#include "Mueble.h"
#include "../Herramienta.h"
#include "../../Control/Game.h"
#include "../../Utils/ParticleExample.h"
#include "../../Control/ObjectManager.h"
#include "../../Scenes/Tutorial.h"


Mueble::Mueble(Game* mGame, Vector2D<double> position, int width, int height, string claveTextura) : GameObject(mGame)
{
	setDimension(width, height);
	setPosition(position.getX() + getWidth() / 2, position.getY() + getHeight() / 2);
	setTexture(claveTextura);

	funcionando = true;

	timerTexture = &sdlutils().images().at("timer");

	humo = new ParticleExample();
	humo->setRenderer(sdlutils().renderer());
	humo->setStyle(ParticleExample::NONE);
}



SDL_Rect Mueble::getCollider()
{
	SDL_Rect rect = getTexBox();

	if(isActive())return {
		rect.x,
		rect.y + rect.h / 3,
		rect.w,
		rect.h / 3 * 2
	};
	return { 0,0,0,0 };
}

SDL_Rect Mueble::getOverlap()
{
	SDL_Rect rect = getTexBox();

	int incr = 9;

	if(isActive())return {
		rect.x,
		rect.y,
		rect.w,
		rect.h / 2 - incr
	};
	return { 0,0,0,0 };
}

bool Mueble::receiveHerramienta(Herramienta* h)
{
	if (!funcionando) 
	{
		funcionando = true;
		h->setActive(false);
		return true;
	}
	return false;
}

bool Mueble::testMueble()
{
	if (!dynamic_cast<Tutorial*>(mGame->getCurrentScene()) && mGame->getNetworkManager()->isHost()) {
		float rnd = rand() % randomOptions + 1;

		if (rnd <= roto)
		{
			romperMueble();

			// Mandar mensaje de que se rompe el mueble
			mGame->getNetworkManager()->syncMuebleRoto(getId());
		}
		return funcionando;
	}
	return true;

}

void Mueble::romperMueble() {
	funcionando = false;
	roto -= 5; //Para que cada vez sea m�s dif�cil que se rompa

	mGame->getObjectManager()->resetMueblesCounter();
}
