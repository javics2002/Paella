#pragma once
#include "../Scenes/Scene.h"

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
class NetworkManager;

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

class Jornada : public Scene
{
	const float LERP_INTERPOLATION = 0.2f;

	MapInfo mapInfo;

	list<double> puntuacionesComandas;

	double puntuaci�nTotal;

	Vector2D<int> tamRestaurante = Vector2D<int>(0, 1216);

	bool paused = false;

	int nJornada = 0;

public:
	Jornada(Game* game, string tilemap,int numeroJornada);
	~Jornada();

	void handleInput(bool& exit)  override;
	void update() override;
	void addPuntuaciones(double puntosComanda) override;

	void refresh() override;
	void mediaPuntuaciones();

	void loadMap(string const& path);
	void togglePause();
};
