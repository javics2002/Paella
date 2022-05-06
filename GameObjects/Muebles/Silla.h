#pragma once
#include "Mueble.h"

class mGame;

class Cliente;

class Silla : public Mueble
{
	const unsigned int DIMENSION_W = 1;
	const unsigned int DIMENSION_H = 1;

	Mesa* mMesa;

public:
	Silla(Game* mGame, Vector2D<double> pos, string texture);
	~Silla() = default;

	Silla* initMesa(Mesa* mesa) override;

	bool receiveGrupoClientes(GrupoClientes* gc) override;
	bool receivePaella(Paella* paella) override;
	virtual bool returnObject(Player* p) override;

	SDL_Rect getOverlap() override;
	SDL_Rect getCollider() override;

	void decirPedido() override;

	virtual void sentarCliente(Cliente* c) = 0;
};

