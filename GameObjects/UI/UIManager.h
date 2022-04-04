#pragma once
#include "../UI/UiButton.h"
class Game;
class IngredienteButton;
class Comanda;
class ListaComandas;
using namespace std;
using tweeny::easing;

class UIManager
{
	vector<string> texturasIngredienes = { "alcachofa",  "calamar", "cangrejo", "gamba",
		   "langosta", "mejillon", "pimientoRojo", "pimientoVerde", "pollo" };
	vector<string> texturasNumeros = { "uno",  "dos", "tres", "cuatro",
	   "cinco", "seis", "siete", "ocho", "nueve" };
	vector<string> texturasTamanos = { "small","medium","large" };
	vector<Point2D<double> >posicionesBotones;
	vector<GameObject*> interfaz;//el resto de iconos 
	vector<Comanda*> comandas;//comandas de la lista de comandas
	vector<	IngredienteButton*> teclado;//iconos que se usan en la comanda qeu esta redactando
	vector<	UiButton*> uicomandas;
	//vector <GameObject*> pedido;//pedido que se est� redactando lo lleva la comanda

	float uiscale = 1.1;
	vector<GameObject*> pauseMenu; // menú de pausa
	vector<GameObject*> optionsMenu; // menú de opciones
	vector<UiButton*> pauseButtons; // botones del menú de pausa
	vector<UiButton*> optionsButtons; // botones del menú de opciones
	vector<UiButton*> sliders; // sliders del menú de opciones

	list<tweeny::tween<float>> activeTweens;

	int mx;
	int my;
	int anchobotones = 25;
	float volumenMusica;
	float volumenSonido;
	Game* game;
	Comanda* actual;
	ListaComandas* barra;

	UiButton* slideMusica;
	UiButton* slideSonido;

	bool clickPrevio = true;

public:
	UIManager(Game* game);
	~UIManager();
	void uiEvent(int mx, int my, bool& exit, bool paused = false);

	void handleInput(bool& exit, bool paused = false);
	void update(bool paused);

	// void render();
	void render(SDL_Rect* rect);

	void creaComanda(Game* game);
	Comanda* getComanda();
	ListaComandas* getBarra();
	void setBarra(ListaComandas* b);
	vector< Point2D<double>> getPosTeclado();
	vector<IngredienteButton*> getTeclado() { return teclado; };
	void setPosTeclado(vector< Point2D<double>> t);
	void creaTeclado();
	void randomizaTeclado();

	void addInterfaz(GameObject* comanda);

	void creaMenuPausa();
	void creaMenuOpciones();
	void togglePause();
	void toggleOpciones();
	void salirOpciones();

	void desactivaBot();
	void activaBot();

	vector<GameObject*> getPauseMenu() { return pauseMenu; }
	vector<GameObject*> getInterfaz() { return interfaz; }

	tweeny::tween<float>& addTween(float from, float to, float during);
};

