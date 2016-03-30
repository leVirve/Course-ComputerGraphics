#pragma once

#include "cg.h"


void showShaderCompileStatus(GLuint shader, GLint *shaderCompiled);

void setShaders();

char *textFileRead(char *fn);

int textFileWrite(char *fn, char *s);
