/*
     ___                  __  __                                           
    / __| __ ___ _ _  ___|  \/  |__ _ _ _  __ _ __ _ ___ _ _  __ _ __ _ __ 
    \__ \/ _/ -_) ' \/ -_) |\/| / _` | ' \/ _` / _` / -_) '_|/ _| '_ \ '_ \
    |___/\__\___|_||_\___|_|  |_\__,_|_||_\__,_\__, \___|_|(_)__| .__/ .__/
                                               |___/            |_|  |_|   
                                               
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

#include "../include/glCalls.h"

#include "../include/SceneManager.h"

#include "../include/Skylium.h"

#include "../include/defines.h"
#include "../include/config.h"
#include "../include/utils.h"

using namespace std;

SceneManager::SceneManager() :
		__sceneList(0),
		__activeScene(NULL) {
	log(CONSTRUCTOR, "SceneManager constructed.");
}

SceneManager::~SceneManager() {
	while (!__sceneList.empty())
		delete __sceneList.back(), __sceneList.pop_back();
	
	log(DESTRUCTOR, "SceneManager destructed.");
}

Scene *
SceneManager::createScene(const string &_name) {
	Scene *newScene = new Scene(_name);
	__sceneList.push_back(newScene);
	if (!__activeScene)
		__activeScene = newScene;
	return newScene;
}

void
SceneManager::displayActiveScene() {
	
	__setRenderingOptions();
	
	if (__activeScene) {
		__activeScene -> show();	
	}
	
	__unsetRenderingOptions();
}

bool
SceneManager::setActive(const Scene *_toSet) {
	if (__sceneList.size() <= 0) {
		log(WARN, "There are no scenes! Active scene was NOT set.");
		return false;
	}
	
	for (unsigned i = 0; i < __sceneList.size(); i++) {
		if (__sceneList[i] == _toSet) {
			__activeScene = __sceneList[i];
			return true;
		}
	}
	log(WARN, "There is no such scene in the vector! Remember to create new scenes via SceneManager::createScene().");
	return false;
}

void
SceneManager::__setRenderingOptions() {
	gl::Clear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
	gl::ClearColor(1.0, 1.0, 1.0, 1.0);
	checkGLErrors(AT);
	
	gl::Enable(GL_BLEND);
	gl::BlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	checkGLErrors(AT);
	
	gl::Enable(GL_DEPTH_TEST);
	checkGLErrors(AT);
	
	gl::Enable(GL_POLYGON_SMOOTH);
	checkGLErrors(AT);
	
	gl::Hint(GL_LINE_SMOOTH_HINT, GL_NICEST);
	gl::Hint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
	
}

void
SceneManager::__unsetRenderingOptions() {
	gl::Disable(GL_BLEND);
	gl::Disable(GL_DEPTH_TEST);
	
	checkGLErrors(AT);
}