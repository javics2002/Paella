#include "Game.h"
#include "ViewController.h"

#include <stdlib.h>     /* srand, rand */
#include <time.h>       /* time */

Game::Game( int width, int height) {
	name = "Paellas Rodriguez";
	this->width = width;
	this->height = height;

	doExit = false;
	//font = new Font("../Images/Monospace.ttf", 12);

	srand(time(NULL));
}

void Game::startGame() {
}

Game::~Game() {
}

string Game::getGameName()
{
	return name;
}

void Game::update() {
}

void Game::draw()
{
}

void Game::setUserExit() {
	doExit = true;
}

bool Game::isUserExit() {
	return doExit;
}

int Game::getWindowWidth() {
	return width;
}

int Game::getWindowHeight() {
	return height;
}

SDL_Renderer* Game::getRenderer() {
	return renderer;
}

void Game::setRenderer(SDL_Renderer* _renderer) {
	renderer = _renderer;
}

void Game::loadTextures() {
	if (renderer == nullptr)
		throw string("Renderer is null");

	//textureContainer = new TextureContainer(renderer);
}

void Game::renderText(string text, int x, int y, SDL_Color color) {
	//font->render(renderer, text.c_str(), x, y, color);
}
/*
Texture* Game::getTexture(TextureName name) {
	return textureContainer->getTexture(name);
}

Point2D<int> Game::getOrigin() {
	return { int(-(car->getX() - car->getWidth())), 0 };
}*/