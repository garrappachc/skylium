/*
    Vectors.h
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

#ifndef VECTORS_H
#define VECTORS_H

#include <vector>
#include <cstring>

#include <GL/gl.h>

template < typename T >
class sVec2D {
	
private:
	T	__data[2];
	
public:
	T	&x;
	T	&y;

	T	&u;
	T	&v;
	
	sVec2D() :
			x(__data[0]),
			y(__data[1]),
			u(__data[0]),
			v(__data[1]) {
		memset(__data, 0, 2*sizeof(T));
	}
	
	sVec2D(const T &_v1, const T &_v2) :
			__data({_v1, _v2}),
			x(__data[0]),
			y(__data[1]),
			u(__data[0]),
			v(__data[1]) {}
	
	sVec2D(const T *_orig) :
			x(__data[0]),
			y(__data[1]),
			u(__data[0]),
			v(__data[1]) {
		memcpy(__data, _orig, sizeof(T*)*2);
	}
	
	operator T*() {
		return __data;
	}
	
	operator const T*() const {
		return __data;
	}
	
	T& operator [](const int &_pos) {
		return __data[_pos];
	}
	
	const T& operator [](const int &_pos) const {
		return __data[_pos];
	}
	
	bool operator ==(const sVec2D &_orig) const {
		return (__data[0] == _orig[0] && __data[1] == _orig[1]);
	}
	
	bool operator !=(const sVec2D &_orig) const {
		return (__data[0] != _orig[0] || __data[1] != _orig[1]);
	}
	
	T& operator =(const sVec2D &_orig) {
		__data[0] = _orig[0];
		__data[1] = _orig[1];
		return *__data;
	}
	
	T& operator +=(const sVec2D &_orig) {
		__data[0] += _orig[0];
		__data[1] += _orig[1];
		return *__data;
	}
	
	T& operator -=(const sVec2D &_orig) {
		__data[0] -= _orig[0];
		__data[1] -= _orig[1];
		return *__data;
	}
};



template < typename T >
class sVec3D {
	
private:
	T	__data[3];
	
public:
	T	&x;
	T	&y;
	T	&z;
	
	sVec3D() :
			x(__data[0]),
			y(__data[1]),
			z(__data[2]) {
		memset(__data, 0, 3*sizeof(T));
	}
	
	sVec3D(const T &_v1, const T &_v2, const T &_v3) :
			__data({_v1, _v2, _v3}),
			x(__data[0]),
			y(__data[1]),
			z(__data[2]) {}
	
	sVec3D(const T *_orig) :
			x(__data[0]),
			y(__data[1]),
			z(__data[2]) {
		memcpy(__data, _orig, sizeof(T*)*3);
	}
	
	operator T*() {
		return __data;
	}
	
	operator const T*() const {
		return __data;
	}
	
	T& operator [](const int &_pos) {
		return __data[_pos];
	}
	
	const T& operator [](const int &_pos) const {
		return __data[_pos];
	}
	
	bool operator ==(const sVec3D &_orig) const {
		return (__data[0] == _orig[0] && __data[1] == _orig[1] && __data[2] == _orig[2]);
	}
	
	bool operator !=(const sVec3D &_orig) const {
		return (__data[0] != _orig[0] || __data[1] != _orig[1] || __data[2] != _orig[2]);
	}
	
	T& operator =(const sVec3D &_orig) {
		__data[0] = _orig[0];
		__data[1] = _orig[1];
		__data[2] = _orig[2];
		return *__data;
	}
	
	T& operator +=(const sVec3D &_orig) {
		__data[0] += _orig[0];
		__data[1] += _orig[1];
		__data[2] += _orig[2];
		return *__data;
	}
	
	T& operator -=(const sVec3D &_orig) {
		__data[0] -= _orig[0];
		__data[1] -= _orig[1];
		__data[2] -= _orig[2];
		return *__data;
	}
};

typedef sVec3D< GLdouble > sVector;
typedef sVec3D< GLfloat > sPosition;



template < typename T >
class sVec4D {
	
private:
	T	__data[4];
	
public:
	T	&r;
	T	&g;
	T	&b;
	T	&a;
	
	/* Konstruktory */
	sVec4D() :
			r(__data[0]),
			g(__data[1]),
			b(__data[2]),
			a(__data[3]) {
		memset(__data, 0, 4*sizeof(T));
	}
	
	sVec4D(const T &_v1, const T &_v2, const T &_v3, const T &_v4) :
			__data({_v1, _v2, _v3, _v4}),
			r(__data[0]),
			g(__data[1]),
			b(__data[2]),
			a(__data[3]) {}
	
	sVec4D(const T *_orig) :
			r(__data[0]),
			g(__data[1]),
			b(__data[2]),
			a(__data[3]) {
		memcpy(__data, _orig, sizeof(T*)*4);
	}
	
	/* Operatory */
	operator T*() {
		return __data;
	}
	
	operator const T*() const {
		return __data;
	}
	
	const T & operator [](const int &_pos) const {
		return __data[_pos];
	}
	
	T & operator [](const int &_pos) {
		return __data[_pos];
	}
	
	bool operator ==(const sVec4D &_orig) const {
		return (__data[0] == _orig[0] && __data[1] == _orig[1] && __data[2] == _orig[2] && __data[3] == _orig[3]);
	}
	bool operator !=(const sVec4D &_orig) const {
		return !(__data[0] == _orig[0] && __data[1] == _orig[1] && __data[2] == _orig[2] && __data[3] == _orig[3]);
	}
	
	T& operator =(const sVec4D &_orig) {
		__data[0] = _orig[0];
		__data[1] = _orig[1];
		__data[2] = _orig[2];
		__data[3] = _orig[3];
		return *__data;
	}
	
	T& operator +=(const sVec4D& _orig) {
		__data[0] += _orig[0];
		__data[1] += _orig[1];
		__data[2] += _orig[2];
		__data[3] += _orig[3];
		return __data;
	}
	T& operator -=(const sVec4D& _orig) {
		__data[0] -= _orig[0];
		__data[1] -= _orig[1];
		__data[2] -= _orig[2];
		__data[3] -= _orig[3];
		return __data;
	}
};





typedef	sVec4D< GLfloat >	sColor;



#endif /* VECTORS_H */