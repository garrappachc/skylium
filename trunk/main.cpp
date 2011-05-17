/* 
 * File:   main.cpp
 * Authors: Michał Garapich, Eryk Dykiel
 *
 * Created on 1 april 2011, 18:04
 */

#include <cstdlib>
#include <iostream>
#include <GLee.h>
#include <SDL/SDL.h>
#include <SOIL/SOIL.h>

#include "include/Camera.h"
#include "include/Skylium.h"
#include "include/Scene.h"
#include "include/Shader.h"
#include "include/Timer.h"
#include "include/sFont.h"
#include "include/Hud.h"

using namespace std;

GLfloat boxTex[] = {
	0, 0,
	1, 0,
	1, 1,
	0, 1,

	1, 0,
	0, 0,
	0, 1,
	1, 1,

	0, 0,
	1, 0,
	1, 1,
	0, 1,

	0, 0,
	1, 0,
	1, 1,
	0, 1,

	1, 0,
	1, 1,
	0, 1,
	0, 0,

	0, 0,
	1, 0,
	1, 1,
	0, 1
};

int
main() {

	Skylium *s_main = new Skylium();
	
	if (!s_main -> init("Skylium", false)) {
		cout << "Błąd przy Skylium::init(). Przerywam.\n\n";
		return 1;
	}
	
	Shader *cienie = new Shader("shaders/shadow.vert", "shaders/shadow.frag");
	if (!cienie -> make())
		exit(1);

	Scene *scenka = s_main -> createScene("Scenka");

	Object *pudelko = scenka -> createBox("Pudelko");
	pudelko -> scale(-3, -3, -3);
	pudelko -> move(-10, 0, 0);
	if (!pudelko -> loadTexture("texture/box.jpg", boxTex, 48))
		cout << "Nie udało się załadować tekstury!";
	
	Object *plane = scenka -> createObject("plane");
	plane -> loadFromObj("objects/plane.obj", GET_VERTICES | GET_TEXTURE | GET_NORMALS);
	plane -> move(-10, 5.1, 10);
	plane -> scale (1.5, 1.5, 1.5);
	plane -> setColor(0, 132, 200, 1);
	plane -> rotate(0, 0, 20);

	Object *malpka = scenka -> createObject("malpka");
	malpka->loadFromObj("objects/monkey.obj", GET_VERTICES | GET_NORMALS | GET_MATERIAL);
	if (!malpka) {
		cout << "Malpka nie działa.\n";
	}
	malpka -> move(-10, 4.5, 0);
	malpka -> scale(3, 3, 3);
	malpka -> rotate(0, -45, 40);
	malpka -> setColor(88, 25, 0, 1);
	
	Object *table = scenka -> createObject("table");
	table -> loadFromObj("objects/table.obj", GET_VERTICES | GET_NORMALS | GET_MATERIAL | GET_TEXTURE);
	table -> move(-10, -2, 10);
	table -> scale (6, 6, 6);
	table -> setColor(119, 44, 0, 1);
	
	Camera* kamerka = scenka -> createCamera(5.0, 6.0, 0.0);
	kamerka -> lookAt(1, 4, -1);
	
	cienie -> bind(malpka);
	cienie -> bind(plane);
	cienie -> bind(table);

	int swiatelko = scenka -> createLight(6.0, 6.0, 0.0);
	scenka ->setAmbientLight(swiatelko, 0.5, 0.5, 0.5, 1.0);
	scenka ->toggleLight();
	
	Timer *zegarek_dla_animacji = new Timer();
	Timer *zegarek_dla_fps = new Timer();
	
	sFont *foncik = new sFont();
	
	Hud *hudek = new Hud(foncik, sSingleCoord(-100, 0), sSingleCoord(200, 20), 0.7);
	
	short fps = 0;
	
	s_main ->enableMouseCamera();
	
	sKey klawisz;
	while ((klawisz = s_main -> sEvent()) != KEY_ESC) {
		if (klawisz == KEY_DOWN)
			kamerka -> moveCamera(0.1, 0.0, 0.0);
		if (klawisz == KEY_UP)
			kamerka -> moveCamera(-0.1, 0.0, 0.0);
		if (klawisz == KEY_RIGHT)
			kamerka -> moveCamera(0.0, 0.0, -0.1);
		if (klawisz == KEY_LEFT)
			kamerka -> moveCamera(0.0, 0.0, 0.1);
		
		if (zegarek_dla_animacji -> passed(2500, MICROSECONDS)) {
			table -> rotate(0, 0.1, 0);
			plane -> rotate(0, 0.1, 0);
		}
		
		fps++;
		
		if (zegarek_dla_fps -> passed(1, SECONDS)) {
			cout << "\n	FPS: " << fps;
			fps = 0;
		}
		
		s_main -> execute();
		
		hudek -> drawBorder();
		
	}
	
	s_main -> cleanup();
	delete s_main;
	delete cienie;
	delete zegarek_dla_animacji;
	delete zegarek_dla_fps;
	delete foncik;

	return 0;
}
