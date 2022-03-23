#include "RedactaComandabutton.h"
#include "../../Control/game.h"
#include "../../Control/ObjectManager.h";
#include "../../GameObjects/UI/UIManager.h"

RedactaComandabutton::RedactaComandabutton(Game* game, UIManager* uiManager, string claveTextura, int x, int y, int w, int h) : UiButton(game, claveTextura, x, y, w, h)
{
	uim = uiManager;
}

RedactaComandabutton::~RedactaComandabutton()
{
}

void RedactaComandabutton::execute(bool& exit)
{
	if (uim->getComanda() == nullptr)
	{
		uim->creaComanda(game);
	}
	else
	{
		uim->getComanda()->toggleactive();
	}
}

