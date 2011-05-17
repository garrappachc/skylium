/* 
 * SceneManager.cpp
 * Copyright (C) 2011 Michał Garapich
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program. If not, see http://www.gnu.org/licenses/.
 */

#include "../include/SceneManager.h"
#include "../include/Scene.h"
#include "../include/defines.h"

using namespace std;

SceneManager::SceneManager() :
		__sceneList(0),
		__activeScene(NULL) {}

SceneManager::~SceneManager() {
#ifdef __DEBUG__
	cout << LOG_INFO << "Destruktor: ~SceneManager()";
#endif
	while (!__sceneList.empty())
		delete __sceneList.back(), __sceneList.pop_back();
}

Scene *
SceneManager::createScene(const string &_newSceneName) {
	Scene *newScene = new Scene(_newSceneName);
	__sceneList.push_back(newScene);
	if (!__activeScene) {
		__activeScene = newScene;
	}
	return newScene;
}

bool
SceneManager::displayActiveScene() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
#ifdef __DEBUG_STRONG__
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
#endif

	if (__activeScene) {
		__activeScene -> show();
		return true;
	}
	return false;
}

bool
SceneManager::setActive(const string &_sceneName) {
	if (__sceneList.size() == 0)
		return false;
	for (int i = 0; i < (int)__sceneList.size(); i++) {
		if (__sceneList[i] -> name == _sceneName) {
			__activeScene = __sceneList[i];
			return true;
		}
	}
	return false;
}

bool
SceneManager::setActive(const Scene *_scenePointer) {
	if (__sceneList.size() == 0)
		return false;
	for (int i = 0; i < (int)__sceneList.size(); i++) {
		if (__sceneList[i] == _scenePointer) {
			__activeScene = __sceneList[i];
			return true;
		}
	}
	return false;
 }

Scene *
SceneManager::getSceneByName(const string &_sceneName) {
	if (__sceneList.size() == 0)
		return 0;
	for (int i = 0; i < (int)__sceneList.size(); i++) {
		if (__sceneList[i] -> name == _sceneName)
			return __sceneList[i];
	}
	return 0;
}

Scene *
SceneManager::getActiveScene() {
	return __activeScene;
}