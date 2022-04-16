#pragma once
#pragma once

#include "../../sdlutils/Font.h"
#include "../Utils/Texture.h"
#include "../sdlutils/SDLUtils.h"
#include <vector>

using namespace std;

class VirtualTimer;
class Game;

class TextManager
{;

public:

	TextManager(Game* game_, string font);
	~TextManager() {}

	void update();
	void render();

	void activaTexto(string dialogo);
	void desactivaTexto();

	bool desactivado() { return desactivado_; }

	bool esUltimoParrafo() { return ultimoParrafo; }
	void reiniciaParrafo() { terminado = false; lineas.clear(); numeroLinea = 0; };
	bool TerminadoEscribir() { return dialogoTerminado; }
	bool terminadoParrado() { return terminado; }
	void anadeLetra();

private:

	Game* game;
	string fuenteLetra, dialogo_, ultimoCaracter;
	vector<string> lineas;

	bool terminado=true, dialogoTerminado=true,ultimoParrafo,desactivado_=true;
	int offsetYLinea,tiempoCreaccionLetra, numeroLinea,anchoLetra, alturaLetra, anchoTexto;;

};
