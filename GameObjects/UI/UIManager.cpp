#include "UIManager.h"
#include "../../Data/Comanda.h"
#include "../../GameObjects/UI/RedactaComandabutton.h"
#include "../../sdlutils/InputHandler.h"
#include "../UI/Ingredientebutton.h"
#include "../UI/Numerobutton.h"
#include "../UI/Tamanobutton.h"
#include "DescartaCommandaButton.h"
#include "EnviaComandaButton.h"
#include "BorrarButton.h"
#include "AceptaPaellaButton.h"
#include "EliminaComandaButton.h"
#include "../../Data/ListaComandas.h"
#include <iostream>

using namespace std;

UIManager::UIManager(Game* game)
{
	this->game = game;
}

UIManager::~UIManager()
{
	for (auto i : interfaz)
	{
		delete i;
		i = nullptr;
	}
}

void UIManager::uiEvent(int mx, int my, bool& exit)
{
	for (int i = 0; i < interfaz.size(); ++i)
	{
		if (interfaz[i]->isActive())
		{
			if (interfaz[i]->onClick(mx, my, exit))
			{
				mx = -1;
				my = -1;
			}
		}
	}

	for (auto j : teclado)
	{
		if (j->isActive())
		{
			if (j->onClick(mx, my, exit))
			{
				mx = -1;
				my = -1;
			}
		}
	}
	for (auto j : uicomandas)
	{
		if (j->isActive())
		{
			if (j->onClick(mx, my, exit))
			{
				mx = -1;
				my = -1;
			}
		}
	}

	if (barra != nullptr)
	{
		for (auto c : barra->getlista())
		{

			if (c->getEliminabutton()->onClick(mx, my, exit))
			{
				mx = -1;
				my = -1;
			}
			if (c->onClick(mx, my, exit))
			{
				mx = -1;
				my = -1;
			}
		}
	}
}

void UIManager::handleInput(bool& exit)
{
	if (ih().getMouseButtonState(InputHandler::MOUSE_LEFT))
	{
		uiEvent(ih().getmx(), ih().getmy(), exit);
	}
}

void UIManager::update()
{
	for (auto i : interfaz)
	{
		if (i->isActive())
		{
			i->update();
		}
	}
}

void UIManager::render(SDL_Rect* rect = nullptr)
{
	if (barra != nullptr) {
		barra->render(rect);
		barra->renderComandas();
	}

	for (auto i : interfaz)
	{
		if (i->isActive())
		{
			i->render(rect);
		}
	}

	for (auto i : comandas)
	{
		if (i->isActive())
		{
			i->render(rect);
			i->dibujaPedido();
		}
	}

	for (auto i : teclado)
	{
		if (i->isActive())
		{
			i->render(rect);
		}
	}

	for (auto i : uicomandas)
	{
		if (i->isActive())
			i->render(rect);
	}
}

void UIManager::creaComanda(Game* game)
{
	actual = new Comanda(game, 2, this);
	comandas.push_back(actual);

	creaTeclado();
	actual->guardaTeclado();
	actual->toggleTeclado(false);
	actual->toggleTecaldotam(false);
	
	AceptaPaellaButton* aceptaPaellaButton = new AceptaPaellaButton(game, actual, "acepta", actual->getPosition().getX() + actual->getWidth() / 2 + anchobotones / 4, actual->getPosition().getY() + actual->getHeight() / 4, anchobotones, anchobotones);
	interfaz.push_back(aceptaPaellaButton);
	actual->guardaBoton(aceptaPaellaButton);
	DescartaCommandaButton* descartaComandaButton = new DescartaCommandaButton(actual, game, "cancela", actual->getPosition().getX() + actual->getWidth() / 2 + anchobotones / 4, actual->getPosition().getY() + actual->getHeight() / 4 - anchobotones, anchobotones, anchobotones);
	interfaz.push_back(descartaComandaButton);
	actual->guardaBoton(descartaComandaButton);
	BorrarButton* borraComandaButton = new BorrarButton(game, actual, "borra", actual->getPosition().getX() + actual->getWidth() / 2 + anchobotones / 4, actual->getPosition().getY() + actual->getHeight() / 4 - 2 * anchobotones, anchobotones, anchobotones);
	interfaz.push_back(borraComandaButton);
	actual->guardaBoton(borraComandaButton);
	EnviaComandaButton* enviaComandaButton = new EnviaComandaButton(game, "envia", actual->getPosition().getX() + actual->getWidth() / 2 + anchobotones / 4, actual->getPosition().getY() + actual->getHeight() / 4 + anchobotones, anchobotones, anchobotones);
	interfaz.push_back(enviaComandaButton);
	actual->guardaBoton(enviaComandaButton);
}

Comanda* UIManager::getComanda()
{
	return actual;
}

ListaComandas* UIManager::getBarra()
{
	return barra;
}

void UIManager::setBarra(ListaComandas* b)
{
	barra = b;
}

void UIManager::creaTeclado()
{
	int margenbotones = 5;
	double ix = actual->getPosition().getX() / 2 + margenbotones + anchobotones / 2;
	double iy = actual->getPosition().getY() / 2 + 2 * anchobotones;
	for (int i = 0; i < 9; ++i)
	{
		posicionesBotones.push_back(Point2D<double>(ix, iy));
		ix += anchobotones + margenbotones;
		if (ix >= actual->getPosition().getX() + actual->getWidth() / 2 - anchobotones / 2)
		{
			ix = actual->getPosition().getX() / 2 + margenbotones + anchobotones / 2;
			iy += anchobotones + margenbotones;
		}
	}
	int j = 0;
	for (auto i : texturasIngredienes)
	{
		//Comanda comanda,Game* game, TextureName texturename, int x, int y, int w, int h
		IngredienteButton* a = new IngredienteButton(this, game, i, (int)posicionesBotones[j].getX(), (int)posicionesBotones[j].getY(), anchobotones, anchobotones);
		teclado.push_back(a);
		//  objectmanager->creaTeclado(a);

		j++;
	}
	j = 0;
	vector <NumeroButton*> tecladonum;

	for (auto i :texturasNumeros)
	{
		//Comanda comanda,Game* game, TextureName texturename, int x, int y, int w, int h
		NumeroButton* a = new NumeroButton(this, game, i, (int)posicionesBotones[j].getX(), (int)posicionesBotones[j].getY(), anchobotones, anchobotones);
		tecladonum.push_back(a);
		uicomandas.push_back(a);
		j++;
	}

	actual->guardaTecladonum(tecladonum);

	j = 0;
	vector <TamanoButton*> tecladotam;
	for (auto i : texturasTamanos)
	{
		TamanoButton* a = new TamanoButton(this, game, i, (int)posicionesBotones[j].getX(), (int)posicionesBotones[j].getY(), anchobotones, anchobotones);

		tecladotam.push_back(a);
		uicomandas.push_back(a);
		j++;
	}
	actual->guardaTecladotam(tecladotam);
	
}

void UIManager::randomizaTeclado()
{
	vector<Point2D<double>> posdis = posicionesBotones;
	int j = rand() % posdis.size();
	for (auto i : teclado)
	{
		i->setPosition(posdis[j]);
		posdis.erase(posdis.begin() + j);
		if (posdis.size() > 0)
			j = rand() % posdis.size();
	}
}

void UIManager::addInterfaz(GameObject* elementoInterfaz)
{
	interfaz.push_back(elementoInterfaz);
}

vector<Point2D<double>> UIManager::getPosTeclado()
{
	return posicionesBotones;
}

void UIManager::setPosTeclado(vector<Point2D<double>> t)
{
	posicionesBotones = t;
}