#include "Cliente.h"
#include "../../Control/game.h"

#include <iostream>

using namespace std;

Cliente::Cliente(Game* mGame) : GameObject(mGame)
{
	setDimension(DIMENSION, DIMENSION);

	setDepth(1);

	clip.x = 0;
	clip.y = 0;
	clip.w = 256;
	clip.h = 256;

	frameCounter = sdlutils().rand().nextInt(0, 15);
	lastFrameTime = sdlutils().currRealTime();
	frameRate = 1000 / 50;

	currAnim = 0;

	gosht = false;

	flip = SDL_FLIP_NONE;
}

void Cliente::update()
{
	setPosition(pos+vel);
}

void Cliente::render(SDL_Rect* cameraRect)
{
	animUpdate(Vector2D<double>(1, 0));

	SDL_Rect dest = { getX() - getWidth() / 2, getY() - getHeight() / 2, w, h };

	if (gosht) {
		SDL_SetTextureAlphaMod(anims[currAnim]->getSdlTexture(), 128);
		drawRender(cameraRect, dest, anims[currAnim], { 0 ,0 ,255, 255 }, flip);
		SDL_SetTextureAlphaMod(anims[currAnim]->getSdlTexture(), 255);
	}
	else
	{
		drawRender(cameraRect, dest, anims[currAnim], clip, flip);
	}	
}

void Cliente::setFlip(SDL_RendererFlip f)
{
	flip = f;
}

void Cliente::setAnimResources(int clientType)
{
	if (clientType == 0) {

		anims.push_back(&sdlutils().images().at("cliente1WalkSide"));

		anims.push_back(&sdlutils().images().at("cliente1IdleDown"));
		anims.push_back(&sdlutils().images().at("cliente1IdleSide"));

		anims.push_back(&sdlutils().images().at("cliente1SittingDown"));
		anims.push_back(&sdlutils().images().at("cliente1SittingSide"));

		anims.push_back(&sdlutils().images().at("cliente1WalkDown"));
	}
	else if (clientType == 1) {

		anims.push_back(&sdlutils().images().at("cliente2WalkSide"));

		anims.push_back(&sdlutils().images().at("cliente2IdleDown"));
		anims.push_back(&sdlutils().images().at("cliente2IdleSide"));

		anims.push_back(&sdlutils().images().at("cliente2SittingDown"));
		anims.push_back(&sdlutils().images().at("cliente2SittingSide"));

		anims.push_back(&sdlutils().images().at("cliente2WalkDown"));

	}
	else if (clientType == 2) {

		anims.push_back(&sdlutils().images().at("cliente3WalkSide"));

		anims.push_back(&sdlutils().images().at("cliente3IdleDown"));
		anims.push_back(&sdlutils().images().at("cliente3IdleSide"));

		anims.push_back(&sdlutils().images().at("cliente3SittingDown"));
		anims.push_back(&sdlutils().images().at("cliente3SittingSide"));

		anims.push_back(&sdlutils().images().at("cliente3WalkDown"));

	}
}

void Cliente::animUpdate(Vector2D<double> axis)
{
	// 0 Andando
	// 1 Idle Front
	// 3 Sitting Front

	if (sdlutils().currRealTime() - lastFrameTime > frameRate) {
	
		lastFrameTime = sdlutils().currRealTime();

		clip.x = frameCounter * clip.w;
		frameCounter++;

		if (frameCounter * clip.w > anims[currAnim]->width() - 10)
			frameCounter = 0;
	}
}

void Cliente::setAnim(int animNum)
{
	// 0 Andando
	// 1 Idle Front
	// 3 Sitting Front

	currAnim = animNum;
}

Texture* Cliente::getAnim()
{
	return anims[currAnim];
}

SDL_Rect Cliente::getClip()
{
	return clip;
}

void Cliente::drawPickedClient(SDL_Rect* cameraRect, SDL_Rect rect)
{
	drawRender(cameraRect, rect, anims[4], clip, flip);
}

SDL_Rect Cliente::getCollider()
{
	SDL_Rect rect = getTexBox();

	return {
		rect.x + rect.w / 4,
		rect.y + rect.h * 2 / 3,
		rect.w / 2,
		rect.h / 3
	};
}

SDL_Rect Cliente::getOverlap()
{
	SDL_Rect rect = getTexBox();

	return {
		rect.x + rect.w / 4,
		rect.y + rect.h / 4,
		rect.w / 2,
		rect.h * 3 / 4
	};
}

void Cliente::setGoshtClient(bool b)
{
	gosht = b;
}

void Cliente::clientePara()
{
	//if (currAnim == 5)
	//	currAnim = 1;
	//else currAnim = 2;

	if (currAnim == 0)
		currAnim = 2;
	else if (currAnim == 5)
		currAnim = 1;
}

void Cliente::clienteCamina()
{
	if (currAnim == 2)
		currAnim = 0;
	else if (currAnim == 1)
		currAnim = 5;
}
