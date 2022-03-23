#pragma once
#include "UiButton.h"
#include "../../Data/Comanda.h"

class UIManager;

class IngredienteButton : public UiButton
{
	UIManager* uim;
	
public:
	IngredienteButton(UIManager* uim, Game* game, string texturename, int x, int y, int w, int h);
	~IngredienteButton();
	void execute(bool& exit) override;
	void update() override;
};

