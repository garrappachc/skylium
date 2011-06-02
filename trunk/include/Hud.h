/*
    Hud.h
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


#ifndef HUD_H
#define HUD_H

#include <GL/gl.h>

#include "Singleton.h"

class Skylium;
class FontBase;


class Hud : public Singleton< Hud > {

public:
	
	/**
	 * Konstruktor, który pobiera wskaźnik na instancję Skyliuma
	 * oraz inicjalizuje fonta.
	 */
	Hud(const FontBase* = NULL);
	
	/**
	 * Desktruktor, który w sumie nic nie robi, prócz wywalania loga.
	 */
	virtual ~Hud();
	
	/**
	 * Rysuje Huda.
	 */
	void draw();
	
	/**
	 * Ustawia odpowiednią macierz projekcji w zależności o d parametru.
	 * @param flag Jeżeli true, to przygotowuje projekcję pod wyświetlenie HUDa. Jeżeli false, to
	 * wraca do standardowej projekcji 3D.
	 */
	void hudMode(bool);
	
private:
	
	Skylium * __instance;
	
	const FontBase * __font;
};

#endif // HUD_H
