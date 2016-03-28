#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <float.h>

#include <string>
#include <vector>

#include <GL/glew.h>

#include "glm.h"
#include "textfile.h"


extern GLint iLocPosition;
extern GLint iLocColor;
extern GLMmodel* model;
extern bool solid;
extern std::vector<std::string> modelfiles;

void loadOBJModel(int index);

void normalizeModel();

void showShaderCompileStatus(GLuint shader, GLint *shaderCompiled);

void setShaders();
