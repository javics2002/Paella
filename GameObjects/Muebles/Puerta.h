#pragma once

#include "../GameObject.h"
#include "../../Data/Pool.h"
#include "../Cliente.h"
#include "../GrupoClientes.h"
#include "../Cola.h"
#include "../GrupoClientes.h"

class Game;

class Puerta : public GameObject
{
	const unsigned int DIMENSION_W = 70;
	const unsigned int DIMENSION_H = 120;

	const unsigned int maxGrupo = 6;

	const double SPAWN_DELAY = 4000;
	double time;

	Pool<Cliente>* poolClientes;
	Pool<GrupoClientes>* poolGrupos;


	Cola* cola;

	vector<TextureName> texturasClientes = { ingrAlcachofa, ingrCalamar, ingrCangrejo,
	ingrGamba, ingrLangosta, ingrMejillon, ingrPimientoRojo, ingrPimientoVerde, ingrPollo
	};

public:
	Puerta(Game* game, Pool<Cliente>* pool, Pool<GrupoClientes>* pool2);
	~Puerta() = default;

	void update() override;
};




