/*
    Skylium.h
    Copyright (C) 2011  Michał Garapich garrappachc@gmail.com

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/


#ifndef SKYLIUM_H
#define SKYLIUM_H

#include <SDL/SDL.h>

#include "Singleton.h"
#include "SceneManager.h"
#include "Scene.h"

/* Obsługa klawiszy */
typedef enum {
	KEY_NOKEY = 0,
	KEY_ESC = 1,
	KEY_UP = 2,
	KEY_DOWN = 4,
	KEY_RIGHT = 8,
	KEY_LEFT = 16,
	KEY_TAB = 32
} sKey;

class Skylium : public Singleton < Skylium > {

public:
	
	/**
	 * Konstruktor tworzy nowego SceneManagera - singletona
	 */
	Skylium();
	
	virtual ~Skylium();
	
	/**
	 * Metoda init ustawia kontekst renderowania i robi kilka standardowych rzeczy.
	 * @param windowName Nazwa okna.
	 * @param fullScreen Jeżeli true, będzie fullscreen.
	 * @return False, jeżeli coś poszło nie tak.
	 */
	bool init(const std::string&, const bool& = false);
	
	/**
	 * Przechwytuje zdarzenia z kolejki, a następnie renderuje scenę.
	 */
	void execute();
	
	/**
	 * Przestawia bufory.
	 */
	void swapBuffers() { SDL_GL_SwapBuffers(); }
	
	sKey sEvent() { return __pendingKeys; }
	
	void toggleMouseCamera() { __isMouseMotionEnabled = !__isMouseMotionEnabled; }
	
	Scene * createScene(const std::string&);
	
	SceneManager*& Scenes;
	
	
private:
	
	/**
	 * Renderuje scenę.
	 */
	void __render();
	
	/**
	 * Łapie eventy z kolejki, obsługuje je odpowiednio, a naciśnięte klawisze
	 * wrzuca do __pendingKeys.
	 */
	void __catchEvents();
	
	
	/* Instancja SceneManagera */
	SceneManager * __sceneManagement;
	
	/* Przechowuje wciśnięte klawisze. Patrz metoda sEvent(). */
	sKey __pendingKeys;
	
	/* Do kamerki z myszką. */
	bool __isMouseMotionEnabled;
	
	/* Kontekst renderowania SDLa */
	SDL_Surface * __surfDisplay;
	
	/* Przechowuje pozycję x i y myszy. */
	int __lastMousePositionX;
	int __lastMousePositionY;
	
};

#endif // SKYLIUM_H
