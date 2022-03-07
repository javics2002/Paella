#pragma once

#include "SDL.h"

#include <iostream>
#include <string>
#include <vector>
#include <cmath>

#include "../GameObjects/Player.h"

#include "../Utils/Texture.h"

class Restaurante;
class ObjectManager;
class UIManager;

using namespace std;

enum escenasJuego { MENU, RESTAURANTE };

class Game{
    //Cambiar a scene actual
    Restaurante* restaurante;

public:
    Game();
    ~Game();

    void init();
    void start();
    void changeScene(escenasJuego n);

    void handleInput(SDL_Event& event, bool &exit);
    void update();
    void render();
    
    ObjectManager* getObjectManager();
    UIManager* getUIManager();

private:
    escenasJuego currentScene = RESTAURANTE;
};

