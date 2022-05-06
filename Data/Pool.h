#pragma once
#include <vector>
#include <iostream>
#include <List>

#include "../GameObjects/Ingrediente.h"
#include "../GameObjects/Ingredienteletal.h"
#include "../GameObjects/Arroz.h"
#include "../Utils/Collider.h"

using namespace std;

class Game;

template<typename T>
class Pool
{
	Game* mGame;

	//lista con solo los objetos de la pool que est�n activos
	list<T*> aliveObjects;

	//vector con toda la pool
	vector<T*> v;
	//numero de objetos en la Pool
	int numElems;
	//siguiente objeto a añadir al juego (o ultimo en haberse a�adido)
	int nextElem;

	int mNextId;

	void findNextElem() {
		nextElem = (nextElem + 1) % numElems;
		int cont = 0;

		//el bucle no itera ninguna vez en la mayor�a de casos, sobre todo en los ingredientes
		//(es posible que en el caso de los clientes o paellas tarde m�s en encontrar al siguiente)
		while (cont < numElems && v[nextElem]->isActive())
		{
			cont++;
			nextElem = (nextElem + 1) % numElems;
		}
		//si no encuentra ninguno, se duplica el tama�o de la pool
		if (cont == numElems) {
			for (int i = 0; i < numElems; i++) {
				T* o = new T(mGame);
				o->setActive(false);
				o->setId(mNextId);
				mNextId++;
				v.push_back(o);
			}			
		
			numElems *= 2;

			while (cont < numElems && v[nextElem]->isActive())
			{
				cont++;
				nextElem = (nextElem + 1) % numElems;
			}		
		}
	}

public:
	Pool(Game* mGame, int n) : mGame(mGame), numElems(n), nextElem(-1) {
		for (int i = 0; i < n; i++) {
			T* o = new T(mGame);
			o->setActive(false);
			o->setId(mNextId);
			mNextId++;
			v.push_back(o);
		}
			
	}

	~Pool() {
		for (auto i : v) {
			delete i;
			i = nullptr;
		}
	};

	//metodo que busca el siguiente objeto y lo activa
	T* add(Vector2D<double> pos) {
		findNextElem();

		auto& elem = v[nextElem];
		aliveObjects.push_front(elem);
		elem->setPosition(pos);
		elem->setActive(true);
		elem->onActivate();

		return elem;
	}

	T* add() {
		findNextElem();

		auto& elem = v[nextElem];
		aliveObjects.push_front(elem);
		elem->setActive(true);
		elem->onActivate();

		return elem;
	}

	vector<T*> getOverlaps(SDL_Rect GOcollider) {
		vector<T*> c;

		for (auto it : aliveObjects) {
			if (it->overlap(GOcollider))
				c.push_back(it);
		}

		return c;
	}

	vector<T*> getCollisions(SDL_Rect GOcollider) {
		vector<T*> c;

		for (auto it : aliveObjects) {
			if (it->collide(GOcollider))
				c.push_back(it);
		}

		return c;
	}
	
	vector<T*> getActiveObjects() {
		vector<T*> c;

		for (auto it : aliveObjects) {
				c.push_back(it);
		}

		return c;
	}


	void render() {	
		for (auto it : aliveObjects)
			it->render();
	}

	void render(SDL_Rect* rect) {
		for (auto it : aliveObjects)
			it->render(rect);
	}

	void refresh() {
		//Sacamos los objetos desactivados
		auto it = aliveObjects.begin();
		while (it != aliveObjects.end()) {
			if (!(*it)->isActive()) {
				(*it)->onDeactivate();
				it = aliveObjects.erase(it);
			}
			else
				it++;
		}
	}

	void debug() {
		for (auto it : aliveObjects) {
			it->renderDebug();
		}
	}

	void debug(SDL_Rect* rect) {
		for (auto it : aliveObjects) {
			it->renderDebug(rect);
		}
	}

	void update() {
		for (auto object : aliveObjects) {
			object->update();
		}
	}
};



