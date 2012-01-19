/*
     ___ _            _                       
    / __| |_  __ _ __| |___ _ _  __ _ __ _ __ 
    \__ \ ' \/ _` / _` / -_) '_|/ _| '_ \ '_ \
    |___/_||_\__,_\__,_\___|_|(_)__| .__/ .__/
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
#include <string>
#include <fstream>

#include <sys/stat.h>
#include <GL/gl.h>

#include "../include/Shader.h"

#include "../include/Vectors.h"
#include "../include/Skylium.h"

#include "../include/defines.h"
#include "../include/config.h"
#include "../include/utils.h"

using namespace std;

static const unsigned int MAX_LOG_SIZE = 4096;


Shader::Shader(const std::string &_vertFileName, const std::string &_fragFileName) : 
		__vertFile(_vertFileName),
		__fragFile(_fragFileName),
		__isRunning(false) {

	__initGLExtensionsPointers();
	
	if (__vertFile.find('/') == string::npos)
		__vertFile = "shaders/" + __vertFile;
	
	if (__fragFile.find('/') == string::npos)
		__fragFile = "shaders/" + __fragFile;
	
	__vertexShader = glCreateShader(GL_VERTEX_SHADER);
	__fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	
	if ((sGlobalConfig::DEBUGGING & D_CONSTRUCTORS) == D_CONSTRUCTORS)
		cout << LOG_INFO << "Shader (\"" << __vertFile << "\", \"" << __fragFile << "\") constructed.";
}

Shader::~Shader() {
	glDetachShader(__shaderProgram, __vertexShader);
	glDetachShader(__shaderProgram, __fragmentShader);

	glDeleteProgram(__shaderProgram);

	glDeleteShader(__vertexShader);
	glDeleteShader(__fragmentShader);
	checkGLErrors(AT);
	if ((sGlobalConfig::DEBUGGING & D_DESTRUCTORS) == D_DESTRUCTORS)
		cout << LOG_INFO << "Shader (\"" << __vertFile << "\", \"" << __fragFile << "\") destructed.";

}

bool
Shader::make() {
	if (!__fileExists(__vertFile)) {
		if ((sGlobalConfig::DEBUGGING & D_ERRORS) == D_ERRORS)
			cout << LOG_ERROR << "File \"" << __vertFile << "\" not found!";
		return false;
	}
	if (!__fileExists(__fragFile)) {
		if ((sGlobalConfig::DEBUGGING & D_ERRORS) == D_ERRORS)
			cout << LOG_ERROR << "File \"" << __fragFile << "\" not found!";
		return false;
	}
	
	string HEADER = 
			"struct sMaterialParams {"
			"	vec4 emission;"
			"	vec4 ambient;"
			"	vec4 diffuse;"
			"	vec4 specular;"
			"	float shininess;"
			"};"
			
			"struct sLightParams {"
			"	vec4 ambient;"
			"	vec4 diffuse;"
			"	vec4 specular;"
			"	vec4 position;"
			"};"
			
			"struct sLightModelParameters {"
			"	vec4 ambient;"
			"};"

			"uniform vec4 sDefColor;"
			"uniform mat4 sModelViewMatrix;"
			"uniform mat4 sProjectionMatrix;"
			"uniform mat3 sNormalMatrix;"
			"uniform sMaterialParams sFrontMaterial;"
			"uniform sLightParams sLightSource[7];"
			"uniform sLightModelParameters sLightModel;";
	
	if ((sGlobalConfig::DEBUGGING & D_SHADERS) == D_SHADERS)
		cout << LOG_INFO << "Reading shaders' sources... ";
	ifstream vertFile(__vertFile.c_str());
	string vertData = "";
	while (!vertFile.eof()) {
		string temp = "";
		getline(vertFile, temp);
		vertData += temp;
		vertData += "\n";
		if (temp.find("#version") != string::npos)
			vertData += HEADER;
		
	}
	vertFile.close();

	ifstream fragFile(__fragFile.c_str());
	string fragData = "";
	while (!fragFile.eof()) {
		string temp = "";
		getline(fragFile, temp);
		fragData += temp;
		fragData += "\n";
		if (temp.find("#version") != string::npos)
			fragData += HEADER;
	}
	fragFile.close();
	
	if ((sGlobalConfig::DEBUGGING & D_SHADERS) == D_SHADERS)
		cout << "Done. " << LOG_INFO << "Compiling shaders' sources... ";
	
	const char *vert = vertData.c_str();
	const char *frag = fragData.c_str();


	GLint vlength = vertData.length();
	GLint flength = fragData.length();
	
	glShaderSource(__vertexShader, 1, (const GLchar**)&vert, &vlength);
	checkGLErrors(AT);
	glShaderSource(__fragmentShader, 1, (const GLchar**)&frag, &flength);
	checkGLErrors(AT);

	int result;

	glCompileShader(__vertexShader);
	checkGLErrors(AT);
	glGetShaderiv(__vertexShader, GL_COMPILE_STATUS, &result);
	checkGLErrors(AT);
	if (!result) {
		char msg[MAX_LOG_SIZE];
		glGetShaderInfoLog(__vertexShader, MAX_LOG_SIZE, NULL, msg);
		if ((sGlobalConfig::DEBUGGING & D_ERRORS) == D_ERRORS)
			cout << LOG_ERROR << "Error compiling vertex shader! Compilation log:\n" << msg << endl;
		return 0;
	}

	glCompileShader(__fragmentShader);
	checkGLErrors(AT);
	glGetShaderiv(__fragmentShader, GL_COMPILE_STATUS, &result);
	checkGLErrors(AT);
	if (!result) {
		char msg[MAX_LOG_SIZE];
		glGetShaderInfoLog(__fragmentShader, MAX_LOG_SIZE, NULL, msg);
		if ((sGlobalConfig::DEBUGGING & D_ERRORS) == D_ERRORS)
			cout << LOG_ERROR << "Error compiling fragment shader! Compilation log:\n" << msg << endl;
		return 0;
	}

	__shaderProgram = glCreateProgram();
	checkGLErrors(AT);
	glAttachShader(__shaderProgram, __vertexShader);
	checkGLErrors(AT);
	glAttachShader(__shaderProgram, __fragmentShader);
	checkGLErrors(AT);

	glLinkProgram(__shaderProgram);
	checkGLErrors(AT);

	glGetProgramiv(__shaderProgram, GL_LINK_STATUS, &result);
	checkGLErrors(AT);
	if (!result) {
		char msg[MAX_LOG_SIZE];
		glGetProgramInfoLog(__shaderProgram, MAX_LOG_SIZE, NULL, msg);
		if ((sGlobalConfig::DEBUGGING & D_ERRORS) == D_ERRORS)
			cout << LOG_ERROR << "Error linking shader program! Linking log:\n" << msg << endl;
		return false;
	}
	checkGLErrors(AT);

	if ((sGlobalConfig::DEBUGGING & D_SHADERS) == D_SHADERS)
		cout << "Done. No errors reported.";
	
	return true;

}

void
Shader::toggle() {
	if (!__isRunning) {
		glUseProgram(__shaderProgram);
		__isRunning = true;
	} else {
		glUseProgram(0);
		__isRunning = false;
	}
	checkGLErrors(AT);
}

void
Shader::bind(Object *_dest) {
	_dest -> __shader = this;
}

void
Shader::unbind(Object *_dest) {
	_dest -> __shader = NULL;
}

bool
Shader::isBound(Object *_dest) {
	if (_dest -> __shader == NULL)
		return false;
	else
		return true;
}

void
Shader::setUniformFloat(const string& _name, const sVec< GLfloat >& _params) const {
	GLint location = glGetUniformLocation(__shaderProgram, _name.c_str());
	checkGLErrors(AT);
	
	switch (_params.size()) {
		case 2:
			glUniform2f(location, _params[0], _params[1]);
		case 3:
			glUniform3f(location, _params[0], _params[1], _params[2]);
		case 4:
			glUniform4f(location, _params[0], _params[1], _params[2], _params[3]);
	}
	checkGLErrors(AT);
}

void
Shader::setUniformFloat(const string& _name, GLfloat _param) const {
	GLint location = glGetUniformLocation(__shaderProgram, _name.c_str());
	checkGLErrors(AT);
	
	glUniform1f(location, _param);
	checkGLErrors(AT);
}

void
Shader::setMatrixFloat(const string& _name, const sMat16& _matrix) const {
	GLint location = glGetUniformLocation(__shaderProgram, _name.c_str());
	checkGLErrors(AT);
	
	glUniformMatrix4fv(location, 1, GL_FALSE, _matrix);
	checkGLErrors(AT);
}

void
Shader::setMatrixFloat(const string& _name, const sMat9& _matrix) const {
	GLint location = glGetUniformLocation(__shaderProgram, _name.c_str());
	checkGLErrors(AT);
	
	glUniformMatrix3fv(location, 1, GL_FALSE, _matrix);
	checkGLErrors(AT);
}

void
Shader::__initGLExtensionsPointers() {
	glCreateShader = getProcAddr< decltype(glCreateShader) >("glCreateShader");
	glDetachShader = getProcAddr< decltype(glDetachShader) >("glDetachShader");
	glDeleteProgram = getProcAddr< decltype(glDeleteProgram) >("glDeleteProgram");
	glDeleteShader = getProcAddr< decltype(glDeleteShader) >("glDeleteShader");
	glShaderSource = getProcAddr< decltype(glShaderSource) >("glShaderSource");
	glCompileShader = getProcAddr< decltype(glCompileShader) >("glCompileShader");
	glGetShaderiv = getProcAddr< decltype(glGetShaderiv) >("glGetShaderiv");
	glGetShaderInfoLog = getProcAddr< decltype(glGetShaderInfoLog) >("glGetShaderInfoLog");
	glCreateProgram = getProcAddr< decltype(glCreateProgram) >("glCreateProgram");
	glAttachShader = getProcAddr< decltype(glAttachShader) >("glAttachShader");
	glLinkProgram = getProcAddr< decltype(glLinkProgram) >("glLinkProgram");
	glGetProgramiv = getProcAddr< decltype(glGetProgramiv) >("glGetProgramiv");
	glGetProgramInfoLog = getProcAddr< decltype(glGetProgramInfoLog) >("glGetProgramInfoLog");
	glUseProgram = getProcAddr< decltype(glUseProgram) >("glUseProgram");
	glGetUniformLocation = getProcAddr< decltype(glGetUniformLocation) >("glGetUniformLocation");
	glUniform1f = getProcAddr< decltype(glUniform1f) >("glUniform1f");
	glUniform2f = getProcAddr< decltype(glUniform2f) >("glUniform2f");
	glUniform3f = getProcAddr< decltype(glUniform3f) >("glUniform3f");
	glUniform4f = getProcAddr< decltype(glUniform4f) >("glUniform4f");
	glUniformMatrix4fv = getProcAddr< decltype(glUniformMatrix4fv) >("glUniformMatrix4fv");
	glUniformMatrix3fv = getProcAddr< decltype(glUniformMatrix3fv) >("glUniformMatrix3fv");
}

bool
Shader::__fileExists(const std::string &_fileName) {
	struct stat buf;
	if (stat(_fileName.c_str(), &buf) == 0)
		return 1;
	else
		return 0;
}