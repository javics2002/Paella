#pragma once
#include "../Data/Pool.h"
#include <queue>
#include <vector>


class Ingrediente;
//class GrupoClientes;
//class Paella;

using namespace std;

class ObjectManager
{
	//Vector de paelleras
	// 
	//vector<Paella*> paelleras;

	//Pool de clientes
	// 
	//Pool<GrupoClientes*> clientes;
	//queue<GrupoClientes*> cola;
	//int capacidadCola;

	//Pool de ingredientes

	Pool<Ingrediente*> *ingredientes;

public:
	ObjectManager();
	~ObjectManager();
};

