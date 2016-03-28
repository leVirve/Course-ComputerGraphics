#pragma once

#include <stdio.h>

#include <GL/glew.h>
#include <freeglut/glut.h>

#include "glm.h"

#include "graphics.h"


#ifndef GLUT_WHEEL_UP
# define GLUT_WHEEL_UP   0x0003
# define GLUT_WHEEL_DOWN 0x0004
#endif

#ifndef GLUT_KEY_ESC
# define GLUT_KEY_ESC	 0x001B
# define CHANGE_MODE_KEYw 0x0057
# define CHANGE_MODE_KEYW 0x0077
# define NEXT_MODEL_KEY_x 0x78
# define NEXT_MODEL_KEY_X 0x58
# define PREV_MODEL_KEY_z 0x7A
# define PREV_MODEL_KEY_Z 0x5A
#endif


extern GLint iLocPosition;
extern GLint iLocColor;
extern GLMmodel* model;
extern bool solid;
extern unsigned int curr_model_index;

void onIdle();

void onDisplay(void);

void onMouse(int who, int state, int x, int y);

void onMouseMotion(int x, int y);

void onKeyboard(unsigned char key, int x, int y);

void onKeyboardSpecial(int key, int x, int y);

void onWindowReshape(int width, int height);
