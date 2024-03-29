/*
      ___                                        
     / __|__ _ _ __  ___ _ _ __ _   __ _ __ _ __ 
    | (__/ _` | '  \/ -_) '_/ _` |_/ _| '_ \ '_ \
     \___\__,_|_|_|_\___|_| \__,_(_)__| .__/ .__/
                                      |_|  |_|   
                                      
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

#include <iostream>
#include <cmath>

#include "../include/Camera.h"
#include "../include/Skylium.h"
#include "../include/MatricesManager.h"

#include "../include/defines.h"
#include "../include/config.h"
#include "../include/utils.h"

static const double PI = 3.1415265359;
static const double PIdiv180 = PI/180.0;
static const double PIdiv2 = PI/2;

using namespace std;

Camera::Camera(const cType &_type) :
		__type(_type),
		__fovy(45.0),
		__zNear(0.0001),
		__zFar(1000.0),
		__eye({0, 0, 0}),
		__center({0, 0, 0}),
		__up({0, 1, 0}),
		__range(20),
		__matrices(MatricesManager::GetSingleton()) {
	log(CONSTRUCTOR, "Camera constructed.");
}

Camera::Camera(gl::Float _x, gl::Float _y, gl::Float _z, const cType &_type) :
		__type(_type),
		__fovy(45.0),
		__zNear(0.0001),
		__zFar(1000.0),
		__eye({_x, _y, _z}),
		__center({0, 0, 0}),
		__up({0, 1, 0}),
		__range(20),
		__matrices(MatricesManager::GetSingleton()) {
	if (__type == SPHERICAL)
		__eye.normalize();
	
	log(CONSTRUCTOR, "Camera (%f, %f, %f) constructed.", __eye.x, __eye.y, __eye.z);
}

Camera::~Camera() {
	log(DESTRUCTOR, "Camera destructed.");
}

void
Camera::setProjection() {
	__windowWidth = Skylium::GetSingletonPtr() -> getContextPtr() -> winWidth;
	__windowHeight = Skylium::GetSingletonPtr() -> getContextPtr() -> winHeight;
	
	gl::Float aspect = (gl::Float) __windowWidth / __windowHeight;
	gl::Viewport(0, 0, __windowWidth, __windowHeight);
	checkGLErrors(AT);
	
	__matrices.sPerspective(__fovy, aspect, __zNear, __zFar);
}

void
Camera::setView() {
	if (__type == FPP)
		__matrices.sLookAt(__eye, __center + __eye, __up);
	else if (__type == SPHERICAL)
		__matrices.sLookAt((__eye * __range) + __center, __center, __up);
	
	// we have modelViewMatrix set, we can count the normal matrix now.
	__matrices.produceNormalMatrix();
}

void
Camera::moveCamera(gl::Float movX, gl::Float movY, gl::Float movZ) {
	if (__type == FPP) {
		__eye.x += (__center.x * movZ);
		__eye.z += (__center.z * movZ);
		__eye.x += (__center.z * movX * -1);
		__eye.z += (__center.x * movX);
		__eye.y += movY / 10;
	} else if (__type == SPHERICAL) {
		__center.x += (__eye.x * movZ * -0.1);
		__center.z += (__eye.z * movZ * -0.1);
		__center.x += (__eye.z * movX * 0.1);
		__center.z += (__eye.x * movX * -0.1);
		__center.y += movY / 10;
	}
}

void
Camera::rotateCamera(gl::Float _x, gl::Float _y, gl::Float) {
	if (__type == FPP) {
		/* Update the angle */
		__angle.x -= (gl::Float)(_x / 100);
		__angle.y += (gl::Float)(_y / 200);
	
		if (__angle.y > 90 * PIdiv180) __angle.y = 90 * PIdiv180;
		else if (__angle.y < -90 * PIdiv180) __angle.y = -90 * PIdiv180;
	
		// vector up
		__center.y = sin(__angle.y);
	
		// count new position on x-z
		__center.x = cos(__angle.x - 90);
		__center.z = sin(__angle.x - 90);
	
		__center.normalize();
	} else if (__type == SPHERICAL) {
		__angle.x += (gl::Float)(_x / 200);
		__angle.y -= (gl::Float)(_y / 200);
		
		__eye.x = -1 * cos(__angle.y) * sin(__angle.x - 90);
		__eye.y = sin(__angle.y);
		__eye.z = cos(__angle.y) * cos(__angle.x - 90);
		
		__eye.normalize();
	}
}

void
Camera::lookAt(gl::Float x, gl::Float y, gl::Float z) {
	__center = sVector3D({x, y, z});
	if (__type == FPP)
		__center.normalize();

	log(PARAM, "LookAt: (%f, %f, %f)", __center.x, __center.y, __center.z);
}

sVector3D
Camera::getEye() {
	if (__type == FPP)
		return __eye;
	else 
		return __eye + __center;
}

sVector3D
Camera::getCenter() {
	if (__type == FPP)
		return __center + __eye;
	else
		return __center;
}


