#include "ExitButton.h"
#include "../../Control/game.h"
#include "../../Control/ObjectManager.h";
#include "../../GameObjects/UI/UIManager.h"
#include "../../Scenes/Menu.h"

ExitButton::ExitButton(Game* game, string claveTextura, int x, int y, int w, int h) : UiButton(game, claveTextura, x, y, w, h)
{
    gamet = game;


}

ExitButton::~ExitButton()
{
}

void ExitButton::execute()
{
    uim = gamet->getUIManager();
    //exit game
}