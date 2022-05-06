#include "Disconnected.h"
#include "../Control/NetworkManager.h"
#include "../Scenes/Menu.h"

Disconnected::Disconnected(Game* mGame) : Scene(mGame)
{
	mBackground->setTexture("disconnectedBg");
	mBackground->setPosition(sdlutils().width() / 2, sdlutils().height() / 2);
	mBackground->setDimension(sdlutils().width(), sdlutils().height() + 100);

	UiButton* mReturn = new UiButton(mGame, "back", 200, sdlutils().height() - 100, 280, 120);
	mReturn->setInitialDimension(mReturn->getWidth(), mReturn->getHeight());
	mReturn->setAction([](Game* mGame, bool& exit) {
		mGame->sendMessageScene(new Menu(mGame));
		});

	mUiManager->addButton(mReturn);

}



