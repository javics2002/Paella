// This file is part of the course TPV2@UCM - Samir Genaim

#pragma once

#include <iostream>
#include <SDL.h>
#include <array>

#include "../utils/Singleton.h"


// Instead of a Singleton class, we could make it part of
// SDLUtils as well.

class InputHandler: public Singleton<InputHandler> {

	friend Singleton<InputHandler> ;
	
	SDL_Joystick* joystick_;

private:

	bool isMouseMotionEvent_;
	bool isMouseButtonEvent_;
	bool isKeyboardEvent_;
	SDL_Scancode keyPressed;
	bool interactPressed;

	std::pair<Sint32, Sint32> mousePos_;
	std::array<bool, 3> mbState_;
	const Uint8* kbState_;
	const int CONTROLLER_DEAD_ZONE = 8000;
	float ejeX, ejeY;
	int mx = -1;
	int my = -1;

	InputHandler() {
		kbState_ = SDL_GetKeyboardState(0);
		clearState();
		initJoystick();
		interactPressed = false;
	}

	inline void onMouseMotion(const SDL_Event& event) {
		isMouseMotionEvent_ = true;
		mousePos_.first = event.motion.x;
		mousePos_.second = event.motion.y;

	}

	inline void onMouseButtonChange(const SDL_Event& event, bool isDown) {
		isMouseButtonEvent_ = true;
		switch (event.button.button) {
		case SDL_BUTTON_LEFT:
			mbState_[LEFT] = isDown;
			cout << "CLICK IH  ";
			cout << isDown;
			SDL_GetMouseState(&mx, &my);
			/*if (uim != nullptr)
			{
				uim->uiEvent(mx, my);
			}*/

			/*	mx = -1;
				my = -1;*/
			break;
		case SDL_BUTTON_MIDDLE:
			mbState_[MIDDLE] = isDown;
			break;
		case SDL_BUTTON_RIGHT:
			mbState_[RIGHT] = isDown;
			break;
		default:
			break;
		}
		//clearState();
	}

public:
	enum MOUSEBUTTON : uint8_t {
		LEFT = 0, MIDDLE = 1, RIGHT = 2
	};

	virtual ~InputHandler() {
	}

	// clear the state
	inline void clearState() {
		isMouseButtonEvent_ = false;
		isMouseMotionEvent_ = false;
		isKeyboardEvent_ = false;
		keyPressed = SDL_SCANCODE_UNKNOWN;

		for (auto i = 0u; i < 3; i++) {
			mbState_[i] = false;
		}
	}

	// update the state with a new event
	inline void update(const SDL_Event& event) {
		
		switch (event.type) {
		case SDL_KEYDOWN:
		 	onKeyboardPressed(event.key.keysym.scancode);
			break;
		case SDL_MOUSEMOTION:
			onMouseMotion(event);
			break;
		case SDL_MOUSEBUTTONDOWN:
			onMouseButtonChange(event, true);
			break;
		case SDL_MOUSEBUTTONUP:
			onMouseButtonChange(event, false);
			break;
		case SDL_JOYAXISMOTION:
			onJoystickMotion(event);
			break;
		case SDL_KEYUP:
			if (event.key.keysym.scancode == SDL_SCANCODE_E)
				interactPressed = false;
			onKeyboardPressed(event.key.keysym.scancode);
			break;
		default:
			onKeyboardPressed(SDL_SCANCODE_UNKNOWN);
			break;
		}
	}

	inline void initJoystick()
	{
		//Check for joysticks
		if (SDL_NumJoysticks() < 1)
		{
			printf("Warning: No joysticks connected!\n");
		}
		else
		{
			//Load joystick
			joystick_ = SDL_JoystickOpen(0);
			if (joystick_ == NULL)
			{
				printf("Warning: Unable to open game controller! SDL Error: %s\n", SDL_GetError());
			}
		}
	}

	// refresh
	inline void refresh() {
		SDL_Event event;

		clearState();
		while (SDL_PollEvent(&event))
			update(event);
	}

	inline int getAxisX()
	{
		return ejeX;
	}

	inline int getAxisY()
	{
		return ejeY;
	}

	//Refactorizar para que con el KeyUp cambie los valores en vez de con la ausencia de esos KeyDowns 
	//para que no necesite GetKey() y cree un array cada vez
	inline void onKeyboardPressed(SDL_Scancode key) {
		if (GetKey(SDL_SCANCODE_A))
			ejeX = -1; // valor entre -1 y 1
		else if (GetKey(SDL_SCANCODE_D))
			ejeX = 1;
		else
			ejeX = 0;

		if (GetKey(SDL_SCANCODE_W))
			ejeY = -1; // valor entre -1 y 1
		else if (GetKey(SDL_SCANCODE_S))
			ejeY = 1;
		else
			ejeY = 0;

		if (GetKey(SDL_SCANCODE_E) && !interactPressed)
		{
			interactPressed = true;

			keyPressed = SDL_SCANCODE_E;
			isKeyboardEvent_ = true; // Intentar recoger objeto
		}
	}

	inline void onJoystickMotion(const SDL_Event& e) {
		if (e.jaxis.which == 0)//controller 0
		{
			if (e.jaxis.axis == 0)// x axis
			{
				if (e.jaxis.value < -CONTROLLER_DEAD_ZONE)
					ejeX = e.jaxis.value; // valor entre -1 y 1
				else if (e.jaxis.value > CONTROLLER_DEAD_ZONE)
					ejeX = e.jaxis.value;
				else
					ejeX = 0;
			}

			else if (e.jaxis.axis == 1)//y axis
			{
				if (e.jaxis.value < -CONTROLLER_DEAD_ZONE)
					ejeY = e.jaxis.value;
				else if (e.jaxis.value > CONTROLLER_DEAD_ZONE)
					ejeY = e.jaxis.value;
				else
					ejeY = 0;
			}
		}
	}

	inline Vector2D<double> getAxis() {
		Vector2D<double> axis(ejeX, ejeY);
		axis.normalize();
		return axis;
	}

	// mouse
	inline bool mouseMotionEvent() {
		return isMouseMotionEvent_;
		
	}

	inline bool mouseButtonEvent() {
		
		return isMouseButtonEvent_;

	}

	inline const std::pair<Sint32, Sint32>& getMousePos() {
		return mousePos_;
	}

	inline int getMouseButtonState(MOUSEBUTTON b) {
		return mbState_[b];
	}

	inline bool isKeyboardEvent() {
		return isKeyboardEvent_;
	}

	inline SDL_Scancode getKeyPressed() {
		return keyPressed;
	}

	/// prueba

	bool GetKey(Uint8 key)
	{
		return SDL_GetKeyboardState(NULL)[key];
	}

	// TODO add support for Joystick, see Chapter 4 of
	// the book 'SDL Game Development'
	int Getmx() { return mx; };
	int Getmy() { return my; };



	
};

// This macro defines a compact way for using the singleton InputHandler, instead of
// writing InputHandler::instance()->method() we write ih().method()
//
inline InputHandler& ih() {
	return *InputHandler::instance();
}
