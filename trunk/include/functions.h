/* 
 * functions.h
 * Copyright (C) 2011 Michał Garapich, Eryk Dykiel
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

#ifndef FUNCTIONS_H
#define	FUNCTIONS_H

#include <iostream>
#include <sstream>

template < typename T >
T strToNum(const std::string& s) {
	std::stringstream iss(s);
	T v = (T)0;
	iss >> v;
	return v;
}

template < typename T >
std::string numToStr(const T& v) {
	std::ostringstream oss;
	oss << v;
	return oss.str();
}


#endif	/* FUNCTIONS_H */
