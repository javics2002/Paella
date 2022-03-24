#pragma once
#include "../GameObject.h"

constexpr auto TILE_SIZE = 64;

class Game;
class Paella;
class Ingrediente;
class GrupoClientes;
class Player;

class Mueble : public GameObject
{
public:
	Mueble(Game* game, Vector2D<double> position, int width, int height, string claveTextura);
	~Mueble() = default;

    virtual bool receiveIngrediente(Ingrediente* ingr) { return false; };
    virtual bool receiveGrupoClientes(GrupoClientes* gc) { return false; };
    virtual bool receivePaella(Paella* pa) { return false; };
    virtual bool returnObject(Player* p) { return false; }
};



