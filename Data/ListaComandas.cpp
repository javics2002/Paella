#include "ListaComandas.h"
#include "Comanda.h"
#include "../GameObjects/UI/EliminaComandaButton.h"
#include "../Control/Game.h"
#include "../GameObjects/UI/UIManager.h"
#include "../sdlutils/InputHandler.h"
#include "../Scenes/Tutorial.h"
ListaComandas::ListaComandas(Game* mGame,UIManager* m) :GameObject(mGame)
{

	setTexture("barra");
	Vector2D<double> p;
	p.setX(500);
	p.setY(40);
	setPosition(p);
	cX = p.getX()/5 ;
	cY = p.getY();
	setDimension(ancho, alto);
	uimt = m;
	inicx = cX;

}
ListaComandas::~ListaComandas()
{
	
}
void ListaComandas::A�adeComanda(Comanda* comanda)
{
	Comanda* c = new Comanda(*comanda);

	limit = inicx + maxvisibles * 1.5 * c->getWidth();
	if (numcomandas < maxvisibles)
	{
		
		int x = c->getX();
		if (lista.empty())
		{
			cX = inicx;//a�adimos siempre al principio y empujamos las demas
		}
		else
		{
			auto ic = lista.begin();
			
			Comanda* d=*ic;
			cX = d->getPosition().getX() - 1.5 * c->getWidth();
		}
		
		desplazamineto = cX - x;
		
		c->desplazacomandas(desplazamineto);
		c->setPosition(cX, cY);
		c->getPosition().setX(cX);
		c->setTexturecoords(cX, cY);
	
		c->setSitio(lista.insert(lista.begin(),c));
		
		EliminaComandaButton* e = new EliminaComandaButton(uimt, c, mGame, "cancela", cX, cY + c->getHeight()/2, 25, 25);
		
		if (selected != nullptr)
		{
			selected->deseleccionaComanda();
			selected = nullptr;
		}
		e->setActive(false);
		c->setEliminabutton(e);
		
		selected = c;
		c->seleccionaComanda();
		numcomandas++;
		
		auto ec = lista.end();
		ec--;
		Comanda* ed = *ec;
		if (ed->getPosition().getX() < limit)
		{
			for (auto c : lista)//desplazamos las  comandas
			{
				int nx = c->getPosition().getX() + 1.5 * c->getWidth();
				int dsp = nx - c->getPosition().getX();
				c->desplazacomandas(dsp);
				c->setPosition(nx, cY);
				c->setTexturecoords(nx, cY);
				c->getEliminabutton()->setPosition(nx, cY + c->getHeight() / 2);
			}
		}
	}
	else 
	{
	//se guardan en el buffer de comandas.
		c->desplazacomandas(-500);
		c->setPosition(-500, cY);
		EliminaComandaButton* e = new EliminaComandaButton(uimt, c, mGame, "cancela", -500, cY + c->getHeight() / 2, 25, 25);
	
		c->setEliminabutton(e);
		listanovisibles.push_front(c);
	}
	setBarraActive(true);
}
void ListaComandas::renderComandas()
{
	for (auto i: lista)
	{
		if (i->isActive())
		{
			i->render(nullptr);
			i->dibujaPedido();
		}
	}
}
void ListaComandas::finalizacomanda(Comanda* comanda)
{
	

	if (dynamic_cast<Tutorial*>(mGame->getCurrentScene()) && mGame->getCurrentScene()->getState()<TUTORIALSTATE_DAR_DE_COMER){

		mGame->getCurrentScene()->changeState(TUTORIALSTATE_PAUSA_BORRAR_COMANDA);
	}
	else {
		
		if (comanda == selected)
		{
			if (lista.size() > 1)
			{
				selected = *lista.begin();
				selected->seleccionaComanda();
			}
			else
			{
				selected = *lista.begin();
				
			}
		}
		for (auto c : lista)
		{

			if (c->getPosition().getX() < comanda->getPosition().getX())
			{
				int nx = c->getPosition().getX() + 1.5 * c->getWidth();
				int dsp = nx - c->getPosition().getX();
				c->desplazacomandas(dsp);
				c->setPosition(nx, cY);
				c->setTexturecoords(nx, cY);
				c->getEliminabutton()->setPosition(nx, cY + c->getHeight() / 2);
			}
		}
		if (lista.size() == 1)
		{
			lista.erase(lista.begin());
		}
		else
			lista.erase(comanda->getSitio());
		numcomandas--;
		if (!listanovisibles.empty())
		{


			Comanda* c = listanovisibles.back();
			cX = inicx + 1.5 * c->getWidth();
			int x = -cX - c->getX();
			x += 3 * c->getWidth() + anchobotones*4;//4 son los botones que caben en una linea
			c->setPosition(cX, cY);
			c->setTexturecoords(cX, cY);
			c->desplazacomandas(x );
			c->getEliminabutton()->setPosition(cX, cY + c->getHeight() / 2);
			c->setSitio(lista.insert(lista.begin(), c));
			listanovisibles.pop_back();
			if (selected != nullptr)
			{
				selected->deseleccionaComanda();
				selected = nullptr;
			}
			numcomandas++;


		}
		if (lista.size() > 0) {
			selected = *lista.begin();
			selected->seleccionaComanda();
		}
		else {
			toggleBarra();
		}
	}
}
void ListaComandas::update()
{
	if (listaActive&&!lista.empty())
	{
		if (ih().getKey(ih().FOCUSRIGHT))
		{

			seleccionasigcomanda(1);
		}
		if (ih().getKey(ih().FOCUSLEFT))
		{
			seleccionasigcomanda(-1);
		}
		if (ih().getKey(ih().X))
		{
			pressSelectedComanda();
		}
	}
}
void ListaComandas::seleccionaComanda(Comanda* comanda)
{
	if (selected != nullptr)
	{
		selected->deseleccionaComanda();
		selected = comanda;
	}
	else
		selected = comanda;
}
Comanda* ListaComandas::getComandaSeleccionada()
{
	return selected;
}
void ListaComandas::toggleBarra()
{
	listaActive = !listaActive;
	ih().setFocused(listaActive);
	for (auto i : lista)
	{
		i->setActive(listaActive);
	}
	if (listaActive)//actiamos la barra y colocamos el foco
	{
		if (!lista.empty())
		{
			auto it = lista.begin();
			auto c = *it;
			selected = c;
			c->seleccionaComanda();
		}
	}
	else //desactivamos la barra y el foco
	{
		if (selected != nullptr)
		{
			selected->deseleccionaComanda();
			selected = nullptr;
		}
	}
}
bool ListaComandas::isBarraActive()
{
	return listaActive;
}
void ListaComandas::setBarraActive(bool b)
{
	listaActive = b;
	
	for (auto i : lista)
	{
		i->setActive(listaActive);
	}
	if (listaActive)//activamos la barra y colocamos el foco
	{
		if (selected != nullptr)
		{
			selected->deseleccionaComanda();
			selected = nullptr;
		}
		if (!lista.empty())
		{
			auto it = lista.begin();
			auto c = *it;
			selected = c;
			c->seleccionaComanda();
		}
	}
	else //desactivamos la barra y el foco
	{
		if (selected != nullptr)
		{
			selected->deseleccionaComanda();
			selected = nullptr;
		}
	}
}

void ListaComandas::pressSelectedComanda()
{
	bool fake=false;
	if (selected != nullptr)
	{
		selected->getEliminabutton()->execute(fake);
	}
}
void ListaComandas::seleccionasigcomanda(int dir)
{
	if (selected == nullptr)
		selected = *lista.begin();
	auto it = selected->getSitio();
	if (it == lista.begin())
	{
		if (lista.size() > 1)
		{
			if (dir < 0)
			{
				it = lista.end();
				it--;
			}
			else
				it++;

		}
		//else // a donde te vas a mover si en la lista solo hay 1 comanda xd
	}
	else if(it==lista.end())
	{
		if (lista.size() > 1)
		{
			if (dir > 0)
			{
				it = lista.begin();
			}
			else
				it--;
		}

	}
	else {
		if (dir > 0)
		{
			it++;
			if (it == lista.end())
				it = lista.begin();
			
				
		}
		else it--;
	
	
	}
	if (*it != nullptr)
	{
		selected->deseleccionaComanda();
		auto c = *it;
		c->seleccionaComanda();
		selected = c;
	}
	
}
int ListaComandas::queComandaBorro()
{
	int pasosdesdebegin = 0;
		auto it = selected->getSitio();
		auto itaux = lista.begin();
		auto itaux2 = lista.begin();
		auto caux = *itaux;
		itaux = caux->getSitio();
		while (it != itaux)
		{
			itaux2++;
			auto caux = *itaux2;
			itaux = caux->getSitio();
			pasosdesdebegin++;
		}

	return pasosdesdebegin;
}
void ListaComandas::borralaComandaQueteMandan(int n)
{
	auto itaux = lista.begin();
	for (int k = 0; k < n; k++)
	{
		itaux++;
	}
	
	if(selected != nullptr)
		selected->deseleccionaComanda();

	selected = *itaux;
	finalizacomanda(selected);

}