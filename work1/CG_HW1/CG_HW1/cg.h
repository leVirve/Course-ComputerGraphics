#pragma once

// C headers
#include <stdio.h>
#include <stdlib.h>
#include <float.h>

// C++ headers
#include <string>
#include <vector>

// depebdencies
#include <GL/glew.h>
#include <freeglut/glut.h>

// self-headers
#include "glm.h"
#include "textfile.h"


extern GLint iLocPosition;
extern GLint iLocColor;
extern GLMmodel* model;
extern bool solid;
extern unsigned int curr_model_index;
extern std::vector<std::string> modelfiles;
