/*
    defines.h
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

#ifndef DEFINES_H
#define DEFINES_H

#define LOG_INFO "\n\e[36m(II)\e[0m "
#define LOG_ERROR "\n\e[31m(EE)\e[0m "
#define LOG_WARN "\n\e[33m(WW)\e[0m "

/** 
 * Minimalny rozmiar tablicy wierzchołków, żeby została załadowana do VBO.
 * Domyślna wartość - 256 bajtów.
 */
static const unsigned MIN_SIZE_OF_VERTEX_ARRAY = 256;

/**
 * Maksymalny rozmiar tablicy wierzchołków, aby została cała załadowana do VBO.
 * Domyślna wartość - 256 MB.
 */
static const unsigned MAX_SIZE_OF_VERTEX_ARRAY = 268435456;

#endif  /* DEFINES_H */