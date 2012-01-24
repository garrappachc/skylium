/*
      ___  _     _        _     _    
     / _ \| |__ (_)___ __| |_  | |_  
    | (_) | '_ \| / -_) _|  _|_| ' \ 
     \___/|_.__// \___\__|\__(_)_||_|
              |__/                   

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

#ifndef OBJECT_H
#define OBJECT_H

#include <iostream>
#include <vector>
#include <map>

#include "Vectors.h"
#include "Material.h"
#include "Mesh.h"

class Shader;
class Skylium;
class MatricesManager;
class ShaderDataHandler;
struct Position;
struct TexCoords;
struct Normal;
struct Index;
struct IndexComp;

enum {
	TEXTURE = 1,
	NORMALS = 2,
	NORMAL_MAP = 4
};

typedef std::map< Index, long, IndexComp > indicesMap;

class Object {
	
	friend class Shader;

public:
	
	/**
	 * Default ctor, gets the name.
	 */
	Object(const std::string& = "");
	
	/**
	 * Copy ctor.
	 */
	Object(const Object&, const std::string& = "");
	
	/**
	 * Destroys children, meshes and materials.
	 */
	virtual ~Object();
	
	/**
	 * Transforms and renders.
	 */
	virtual void show();
	
	/**
	 * Moves the oject.
	 * @param x factor;
	 * @param y factor;
	 * @param z factor.
	 */
	void move(GLdouble, GLdouble, GLdouble);
	
	/**
	 * Scales the object.
	 * @param x;
	 * @param y;
	 * @param z.
	 */
	void scale(GLdouble, GLdouble, GLdouble);
	
	/**
	 * Rotates the object.
	 * @param rotX X angle;
	 * @param rotY Y angle;
	 * @param rotZ Z angle.
	 */
	void rotate(GLdouble, GLdouble, GLdouble);
	
	/**
	 * Sets the object general colour.
	 * @param R Red;
	 * @param G Green;
	 * @param B Blue;
	 * @param A Alpha.
	 * @return False if one of the values given is not between [0; 1].
	 */
	bool setColor(GLfloat, GLfloat, GLfloat, GLfloat = 1.0);
	
	/**
	 * Sets the object general colour.
	 * @param R Red;
	 * @param G Green;
	 * @param B Blue
	 * @param A Alpha.
	 * @return False if one of the values given is not between [0; 255] or alpha is not between [0; 1].
	 */
	bool setColor(int, int, int, GLfloat = 1.0);
	
	/**
	 * Loads the object form .obj and .mtl.
	 * @param objFile .obj file localization.
	 * @return False if something went wrong.
	 */
	bool loadFromObj(const std::string&);
	
	/**
	 * Loads the whole object into the VBO.
	 */
	virtual void loadIntoVBO();
	
	/**
	 * Locates the material by the name.
	 * @param name Material's name to be found.
	 * @return Pointer to the material or 0, if not found.
	 */
	Material * getMaterialByName(const std::string&);
	
	/**
	 * Adds the pointer to the children's vector.
	 * @param childPtr Child's pointer.
	 */
	void addChild(Object*);
	
	/**
	 * @return True, if the object was already rendered.
	 */
	bool wasShown() { return __wasShown; }
	
	/**
	 * Called by the Scene at the end of each frame.
	 */
	void endFrame() { __wasShown = false; }
	
	/**
	 * @return True if object has any texture.
	 */
	bool isTextured() { return (__content & TEXTURE); }
	
	std::string name;
	
protected:
	
	/* Genera colour of the object.
	 * Does not have to be set, if texture enabled. */
	sColor __defColor;
	
	/* Vectors that the ModelView Matrix will be
	 * multiplied by in MatricesManager::translate, ::rotate
	 * and ::scale. */
	sVector __mov;
	sVector __rot;
	sVector __scale;
	
	/* A pointer to object's shader */
	Shader * __shader;
	
	/* To indicate if the object was already rendered in the current frame. */
	bool __wasShown;
	
private:
	
	/**
	 * Parses the .obj file and put results in current
	 * object's instance.
	 * @param fileName Name of the .obj file.
	 */
	void __parseObj(const std::string&);
	
	/**
	 * Helpful function to shorten a bit parsing the obj file.
	 * It just parses the one face's line.
	 */
	void __parseFace(std::istringstream&, Mesh*&,
			std::vector< Position >&, std::vector< TexCoords >&,
			std::vector< Normal >&, indicesMap&,
			unsigned, unsigned, unsigned,
			long&, unsigned);
	
	/**
	 * Parses the .mtl file.
	 */
	void __parseMtl(const std::string&);
	
	/**
	 * @param fileName Name of the file to be checked.
	 * @return True, if the file exists.
	 */
	bool __fileExists(const std::string&);
	
	/**
	 * According to what object has in __content,
	 * binds the appropriate shader. Default shaders'
	 * instances can be found in global Skylium's
	 * instance.
	 */
	void __bindAppropriateShader();
	
	/* Vector of children's pointers. */
	std::vector< Object* > __children;
	
	std::vector< Object* >::const_iterator __childrenIterator;
	
	std::vector< Mesh* > __meshes;
	
	std::vector< Mesh* >::const_iterator __meshesIterator;
	
	std::vector< Material* > __materials;
	
	short unsigned __content;
	
	MatricesManager& __matrices;
	
	ShaderDataHandler& __shaders;
	
};

#endif // OBJECT_H