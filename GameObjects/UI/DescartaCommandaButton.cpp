#include "DescartaCommandaButton.h"
#include "../../Control/Game.h"
#include "../../Data/Comanda.h"
DescartaCommandaButton::DescartaCommandaButton(Comanda* c,Game* game, string texturename, int x, int y, int w, int h) :UiButton(game, texturename, x, y, w, h)
{
	gamet = game;
	comandat = c;
}
DescartaCommandaButton::~DescartaCommandaButton()
{
}
void DescartaCommandaButton::execute()
{
	//gamet->getObjectManager()->borracomanda();
}
