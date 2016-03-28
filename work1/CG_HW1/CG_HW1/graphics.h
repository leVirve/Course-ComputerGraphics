#pragma once

#include "cg.h"


void loadOBJModel(int index);

void normalizeModel();

void showShaderCompileStatus(GLuint shader, GLint *shaderCompiled);

void setShaders();
