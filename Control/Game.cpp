﻿#include "Game.h"

#include <stdlib.h>     /* srand, rand */
#include <time.h>       /* time */

#include "../Scenes/Menu.h"
#ifdef _DEBUG
#include "../Scenes/Restaurante.h"
#include "../Scenes/GameOver.h"
#include "../Scenes/HostClient.h"
#endif // _DEBUG
#include "../sdlutils/SDLUtils.h"
#include "../sdlutils/InputHandler.h"
#include "../Control/ObjectManager.h"
#include "../GameObjects/UI/UIManager.h"
#include "../Control/NetworkManager.h"

Game::Game() {
	srand(time(NULL));
}

Game::~Game() {

}

void Game::init()
{
	SDLUtils::init("Paellas", 1280, 720, "../../../Assets/resources.json");
	SDLUtils::instance()->showCursor();

#ifdef _DEBUG
	currentScene = new Menu(this);
#else
	currentScene = new Menu(this);
#endif // _DEBUG

	nm = new NetworkManager(this);
}

void Game::start()
{
	bool exit = false;
	SDL_Event event;

#ifndef _DEBUG
	sdlutils().toggleFullScreen();
#endif // _DEBUG


	while (!exit) {
		Uint32 startTime = sdlutils().currRealTime();

		handleInput(event, exit);

		update();

		render();

		Uint32 frameTime = sdlutils().currRealTime() - startTime;

		if (frameTime < 20)
			SDL_Delay(20 - frameTime);
	}
}

void Game::changeScene(Scene* scene) {
	//Cambio de escena 
	currentScene = nullptr;
	currentScene = scene;
}

void Game::handleInput(SDL_Event& event, bool& exit) {
	while (SDL_PollEvent(&event))
		ih().update(event, exit);

	currentScene->handleInput(exit);
}

void Game::update()
{
	currentScene->update();

	// update nm
	if(nm != nullptr)
		nm->update();
}

void Game::render()
{
	sdlutils().clearRenderer();

	currentScene->render();
#ifdef _DEBUG
	currentScene->debug();
#endif // _DEBUG

	sdlutils().presentRenderer();
}

ObjectManager* Game::getObjectManager()
{
	return currentScene->getObjectManager();
}
UIManager* Game::getUIManager()
{
	return currentScene->getUIManager();
}

NetworkManager* Game::getNetworkManager() {
	return nm;
}