#include "Paella.h"
#include "../sdlutils/SDLUtils.h"
#include "Ingrediente.h"
#include "Ingredienteletal.h"
#include "Arroz.h"
#include "../Utils/ParticleExample.h"
#include <map>

Paella::Paella(Game* mGame, int tipo) : ObjetoPortable(mGame), miTipo(tipo)
{
	setDepth(2);

	switch (miTipo)
	{
	case Pequena:
		setDimension(40, 40);
		break;
	case Mediana:
		setDimension(52, 52);
		mSumaIntervalo = 5000;
		break;
	case Grande:
		setDimension(65, 65);
		mSumaIntervalo = 10000;
		break;
	default:
		break;
	}

	setTexture("paellaLimpia");

	ingrEnPaella = vector<bool>(tipoIngrediente::LAST, false);

	humo = new ParticleExample();
	humo->setRenderer(sdlutils().renderer());
	humo->setStyle(ParticleExample::NONE);

	currentCoccionSound = "paella";
}

void Paella::anadeIngr(Ingrediente* ingr_)
{
	//Si ya he echado arroz
	if (mArroz) {
		//A�adimos el ingrediente
		sdlutils().soundEffects().at("dejarIngrediente2").play(0, mGame->UI);
		if (ingr_->esLetal())
			contaminada = true;
		ingredientes.push_back(ingr_->getTipo());
		ingrEnPaella[ingr_->getTipo()] = true;
		ingr_->setActive(false);
	}
}

void Paella::anadeArroz(Arroz* arroz)
{
	//Si aun no tengo arroz
	if (!mArroz) {
		//A�adimos arroz a la paella
		sdlutils().soundEffects().at("dejarIngrediente2").play(0, mGame->UI);
		mArroz = true;
		estadoCoccion = Cruda;
		setContenido(Entera);
		setTexture("paellaCruda");
		arroz->setActive(false);
	}
}

void Paella::setState(EstadoPaellas estado_)
{
	estado = estado_;
	mTiempo = sdlutils().currRealTime();

	//Sonido
	if (estado == Coccion) {
		currentCoccionSound = "paellaCociendo";
		initCocTime = sdlutils().virtualTimer().currTime();
		canalSonido = sdlutils().soundEffects().at(currentCoccionSound).play(-1);

	}
	else
		sdlutils().soundEffects().at(currentCoccionSound).haltChannel(canalSonido);
}

void Paella::paellaRecogida()
{
	vel = { 0,0 };
}

void Paella::update()
{
	switch (estado)
	{
	case Preparacion:
		break;
	case Coccion:
		if (estadoCoccion < tiemposDeCoccion.size() && sdlutils().virtualTimer().currTime() - initCocTime >= tiemposDeCoccion[estadoCoccion]) {
			estadoCoccion++;
			setTexture(coccionTex[estadoCoccion]);

			if (estadoCoccion == MuyHecha) {
				sdlutils().soundEffects().at(currentCoccionSound).haltChannel(canalSonido);
				currentCoccionSound = "paella";
				canalSonido = sdlutils().soundEffects().at(currentCoccionSound).play(-1);
			}
			if (estadoCoccion == Quemada) {
				humo->setStyle(ParticleExample::FIRE);
				sdlutils().soundEffects().at("fuego").play();
			}
			if (estadoCoccion == Incomestible) {
				humo->setStyle(ParticleExample::SMOKE);
			}
		}
		break;
	case Hecha:
		if (sdlutils().virtualTimer().currTime() - initHumoTime >= mTiempoHumo) {
			humo->setStyle(ParticleExample::NONE);
		}
		break;
	}

	humo->setPosition(getX(), getY());
	humo->update();
}

void Paella::finLavado()
{
	contenido = Limpia;
	estado = Preparacion;
	contaminada = false;
	setTexture("paellaLimpia");
}

void Paella::iniLavado()
{
	estado = Lavandose;
}

void Paella::onObjectPicked()
{
	if (estado == Coccion) {
		setState(Hecha);
	}
	else if(estado == Hecha)
		if (estadoCoccion >= Quemada) {
			//humo->setStyle(ParticleExample::SMOKE);
			initHumoTime = sdlutils().virtualTimer().currTime();
		}
}

void Paella::onObjectDropped()
{
}

bool Paella::canPick()
{
	return contenido == Limpia && !mEnMesa;
}

bool Paella::conArroz()
{
	return mArroz;
}

void Paella::enLaMesa(bool estaEnLaMesa)
{
	mEnMesa = estaEnLaMesa;
}

list<tipoIngrediente> Paella::getVIngredientes()
{
	return ingredientes;
}
vector<bool> Paella::getIngrPaella()
{
	return ingrEnPaella;
}

int Paella::getTipo()
{
	return miTipo;
}
int Paella::getContenido()
{
	return contenido;
}

int Paella::getCoccion()
{
	return estadoCoccion;
}

void Paella::setEnsuciada()
{
	ingredientes.clear();
	mArroz = false;
	for (int i : ingrEnPaella) {
		ingrEnPaella[i] = false;
	}
}

void Paella::setContenido(Contenido contenidoP)
{
	contenido = contenidoP;
	humo->setStyle(ParticleExample::NONE);
}

bool Paella::ingrValido(Ingrediente* ingr)
{
	return ingredientes.size() < MAX_INGR && estado == Cruda && !ingrEnPaella[ingr->getTipo()];
}

EstadoPaellas Paella::getState()
{
	return estado;
}

void Paella::comerPaella()
{
	contenido++;

	if (contenido == Mitad) 
		setTexture(coccionTex[estadoCoccion] + "Mitad");
	else {
		setTexture("paellaSucia");
		mArroz = false;
	}
}
void Paella::contaminaPaella()
{
	contaminada = true;
}
bool Paella::estaContaminada()
{
	return contaminada;
}

int Paella::ingredientesEnPaella()
{
	return ingredientes.size();
}

void Paella::render(SDL_Rect* cameraRect)
{
	if (estado != Lavandose) {
		drawRender(cameraRect);

		if (contenido == Entera) {
			for (auto i : ingredientes) {
				drawRender(cameraRect, getTexBox(), &sdlutils().images().at(texturaIngrediente[i] + "C"));
			}
		}
		else if (contenido == Mitad) {
			for (auto i : ingredientes) {
				drawRender(cameraRect, getTexBox(), &sdlutils().images().at(texturaIngrediente[i] + "M"));
			}
		}

		humo->draw(cameraRect);
	}	
}
