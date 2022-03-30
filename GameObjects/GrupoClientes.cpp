#include "GrupoClientes.h"

#include "../Control/Game.h"
#include "../Control/ObjectManager.h"

#include "../GameObjects/Cola.h"

#include "../../Data/Pedido.h"

#include "../GameObjects/Muebles/Mesa.h"

GrupoClientes::GrupoClientes(Game* game) : PoolObject(game), pedido(nullptr), cola(nullptr), estado_(CAMINANDO) , nPaellas(0)
{
	setDimension(DIMENSION, DIMENSION);

	setTexture("berenjena");

	texTolerancia = &sdlutils().images().at("barraTolerancia");
	showTol = false;
	tolerancia = 100;	
	lastTimeTol = 0;

	showPed = false;
	orderStart = true;
	itemNow = 0;
	lastTimePed = 0;
}

void GrupoClientes::initGrupo(Cola* cola_, vector<Cliente*> clientes_)
{
	tolerancia = 100;

	posCola = cola_->getPos();
	cola = cola_;

	clientes = clientes_;

	setState(CAMINANDO);
}

void GrupoClientes::update()
{

	if (estado_ == CAMINANDO) {

		for (auto i : clientes) {
			i->update();
		}		
	}

	else if (estado_ == ENCOLA) {
		int n = clientes.size() - 1;

		SDL_Rect rect = { clientes[n]->getX() - clientes[n]->getWidth(), clientes[n]->getY() - clientes[n]->getHeight() / 2,
			clientes[n]->getWidth() / 2, clientes[0]->getWidth() };

		for (auto i : game->getObjectManager()->getPoolGrupoClientes()->getCollisions(rect)) {
			i->colisionClientes();
		}

		bajaTolerancia();
	}

	else if (estado_ == PIDIENDO) {
		if (!showPed)
			orderStart = true;

		bajaTolerancia();
	}

	else if (estado_ == COMIENDO) {

		if (SDL_GetTicks() - lastTimeComido >= TIEMPO_COMIDA/2) {
			mesa->cambiaTexturaPaellas("paellaMedia",Contenido::Mitad);
		}

		if (SDL_GetTicks() - lastTimeComido >= TIEMPO_COMIDA) {
			mesa->cambiaTexturaPaellas("paellaSucia",Contenido::Sucia);
			estado_ = CUENTA;
		}
	}


}

void GrupoClientes::render(SDL_Rect* cameraRect)
{
	for (auto i : clientes)
		i->render(cameraRect);

	if (isPicked())
		drawRender(cameraRect);

	int bocadilloX = 80;
	int bocadilloY = 60;

	int toleranciaX = 30;
	int toleranciaY = 25;

	if (showPed && estado_ == PIDIENDO) {
		showTol = false;

		if (SDL_GetTicks() - lastTimePed >= PED_DELAY) {
			lastTimePed = SDL_GetTicks();

			itemNow = (itemNow + 1) % texPedido.size();
		}

		int rectDim = 50;

		drawRender(cameraRect, { (int)getX() - bocadilloX / 2, (int)getY() - bocadilloY, bocadilloX, bocadilloY }, texTolerancia);
		drawRender(cameraRect, { (int)getX() - rectDim / 2, (int)getY() - rectDim - 10, rectDim, rectDim }, &sdlutils().images().at(texPedido[itemNow]));

		showPed = false;
	}


	if (showTol) {
		if (!isPicked()) {

			SDL_Rect rect = {};		
			SDL_Rect rect2 = {};		

			switch (estado_)
			{
			case CAMINANDO:
			case ENCOLA:
				rect = { mitadGrupo() - bocadilloX / 2,
					(int)clientes[0]->getY() - clientes[0]->getHeight() / 2 - bocadilloY, bocadilloX, bocadilloY };

				rect2 = { mitadGrupo() - toleranciaX / 2,
					(int)clientes[0]->getY() - clientes[0]->getHeight() / 2 - 2*toleranciaY,  toleranciaX , toleranciaY};

				break;
			case PIDIENDO:
			case ESPERANDO:
			case COMIENDO:
			case CUENTA:
			default:
				break;
			}

			drawRender(cameraRect, rect, texTolerancia);
			drawRender(cameraRect, rect2, &sdlutils().images().at(texturaTolerancia[((int)tolerancia / 20)]));			
		}

		showTol = false;
	}
	if (estado_ == CUENTA) {

		SDL_Rect rect = {};
		rect = { (int)mesa->getX() - bocadilloX / 2, (int)mesa->getY() - bocadilloY, bocadilloX, bocadilloY };
		//PONER ICONO DE QUE HAN TERMINADO LA COMIDA EN EL BOCADILLO
		drawRender(cameraRect, rect, texTolerancia);
	}
}

bool GrupoClientes::collide(SDL_Rect rect)
{
	for (auto i : clientes) {
		if (i->collide(rect)) {
			return true;
		}
	}

	return false;
}

bool GrupoClientes::colisionClientes()
{
	if (estado_ == CAMINANDO) 
		setState(ENCOLA);
	return true;
}

bool GrupoClientes::ratonEncima()
{
	cout << tolerancia << endl;

	showTol = true;

	return true;
}

void GrupoClientes::bajaTolerancia()
{
	if (tolerancia > 0 && SDL_GetTicks() - lastTimeTol >= DIMIN_TIME) {

		tolerancia -= DIMIN_TOLERANCIA;

		if (tolerancia < 0) tolerancia = 0;

		lastTimeTol = SDL_GetTicks();
	}
}

void GrupoClientes::setState(EstadoClientes est)
{
	estado_ = est;
	lastTimeTol = SDL_GetTicks();
}

EstadoClientes GrupoClientes::getState()
{
	return estado_;
}

int GrupoClientes::numIntegrantes()
{
	return clientes.size();
}

vector<Cliente*> GrupoClientes::getIntegrantes()
{
	return clientes;
}

void GrupoClientes::onObjectDropped()
{
	switch (estado_)
	{
	case CAMINANDO:
		break;
	case ENCOLA:
		estado_ = PIDIENDO;
		cola->remove(posCola, clientes.size());
		break;
	case PIDIENDO:
		break;
	case COMIENDO:
		break;
	case ESPERANDO:
		break;
	case CUENTA:
		break;
	default:
		break;
	}
}

void GrupoClientes::onObjectPicked()
{
	switch (estado_)
	{
	case CAMINANDO:
		break;
	case ENCOLA:
		break;
	case PIDIENDO:
		break;
	case COMIENDO:
		break;
	case ESPERANDO:
		break;
	case CUENTA:
		break;
	default:
		break;
	}
}

bool GrupoClientes::canPick()
{
	return estado_ == ENCOLA || estado_==CUENTA;
}

bool GrupoClientes::canDrop() {

	return  estado_== ENCOLA;
}

int GrupoClientes::mitadGrupo()
{
	int mitad = 0;

	for (auto i : clientes) {
		mitad += i->getX();
	}

	return mitad / clientes.size();
}

void GrupoClientes::onDesactivate()
{
	if (estado_ == CUENTA) {
		mesa->clienteSeVa();
		pedido->puntuarPedido(mesa->getPaellasEntregadas());
	}

	auto list = game->getObjectManager()->getPoolClientes();

	for (auto i : clientes) 
		list->remove(i->getIterator());	
}

void GrupoClientes::hacerPedido(int tamMesa,Mesa* m)
{
	mesa = m;

	pedido = new Pedido(clientes.size(), tamMesa);

	texPedido = pedido->getPedidoTex();
}

void GrupoClientes::decirPedidio()
{
	if (estado_ == PIDIENDO) {
		if (orderStart) {
			lastTimePed = SDL_GetTicks();
			itemNow = 0;
			orderStart = false;		
		}
		showPed = true;
	}
		
}



bool GrupoClientes::paellasPedidas() {
	if (estado_ == ESPERANDO || estado_ == PIDIENDO) {

		if (nPaellas + 1 <= pedido->getPedido().size()) {
			nPaellas++;
			if (nPaellas == pedido->getPedido().size()) {
				estado_ = COMIENDO;
				lastTimeComido = SDL_GetTicks();
			}
			else estado_ = ESPERANDO;

			return true;
		}
	}

	return false;
}


