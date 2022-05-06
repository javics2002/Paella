#pragma once
#include "Mueble.h"
#include "../Paella.h"
#include <deque>

class Game;

class Lavavajillas : public Mueble
{
public:

	//um Contenido { Limpia, Entera, Mitad, Sucia };

	const unsigned int DIMENSION_W = 1;
	const unsigned int DIMENSION_H = 1;

	const double TIEMPO_LAVADO = 3000.0;
	double mInitTime;

	int canalSonido2;

	const int MAX_BREAK_TIME = 18000;
	const int REDUCE_BREAK_TIME = 500;
	int mCouldBreak = MAX_BREAK_TIME;
	int seg = 10;

	deque<Paella*> mPaellasSucias;
	deque<Paella*> mPaellasLimpias;

	int i = 0;
	SDL_Rect clip;

public:
	Lavavajillas(Game* mGame, Vector2D<double> pos);
	~Lavavajillas() = default;

	void update() override;
	void lavando();

	bool receivePaella(Paella* paella_) override;
	bool returnObject(Player* p)override; 

	void render(SDL_Rect* camera) override;

	SDL_Rect getOverlap() override;
	SDL_Rect getCollider() override;

	virtual bool resetCounter() override;
};



