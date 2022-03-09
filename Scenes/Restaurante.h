#pragma once
#include "../Scenes/Scene.h"
#include "SDL.h"

#include "./tmxlite/Map.hpp"
#include "./tmxlite/Layer.hpp"
#include "./tmxlite/TileLayer.hpp"
#include "./tmxlite/ObjectGroup.hpp"
#include "./tmxlite/Tileset.hpp"

#include "../Control/Camera.h"

#include <unordered_map>

using namespace std;
using namespace tmx;
using uint = unsigned int;

class Player;
class Mueble;
class Game;

struct MapInfo {
	Map* tilemap;
	string ruta;
	int filas, columnas;
	int anchoTile, altoTile;
	int anchoFondo, altoFondo;
	unordered_map<uint, Texture*> tilesets;

	MapInfo() {
		tilemap = nullptr;
		ruta = "";
		filas = columnas = anchoTile = altoTile = 0;
	}
	~MapInfo() {
		if (tilemap != nullptr)
			delete tilemap;
	}
};

class Restaurante : public Scene
{
	const float LERP_INTERPOLATION = 0.2f;
	 
	Game* game;

	//Punteros a los jugadores (Quizas deberian llamarse yo y amigo, en relativo)
	Player* host;
	Player* client;

	Camera* camara;

	MapInfo mapInfo;
	Fondo* fondo;

	Vector2D<int> tamRestaurante = Vector2D<int>(0, 1216);


public:
	Restaurante(Game* game);
	~Restaurante();

	void handleInput()  override;
	void update() override;
	void render() override;
	void debug() override;


	void loadMap(string const& path);
};