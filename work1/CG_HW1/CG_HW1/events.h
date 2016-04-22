#pragma once

#include "cg.h"
#include "graphics.h"

#ifndef GLUT_WHEEL_UP
# define GLUT_WHEEL_UP   0x0003
# define GLUT_WHEEL_DOWN 0x0004
#endif

#ifndef GLUT_KEY_ESC
# define GLUT_KEY_ESC	  0x1B
# define CHANGE_MODE_KEY_W 0x57
# define NEXT_MODEL_KEY_X  0x58
# define PREV_MODEL_KEY_Z  0x5A
# define MODE_TRANS_KEY    'T'
# define MODE_SCALE_KEY    'S'
# define MODE_ROTATE_KEY   'R'
# define MODE_EYE_KEY      'E'
# define CH_PROJ_KEY       'P'
# define POS_X_KEY         'L'
# define NEG_X_KEY         'J'
# define POS_Y_KEY         'I'
# define NEG_Y_KEY         'K'
# define POS_Z_KEY         'M'
# define NEG_Z_KEY         'O'
#endif

extern ModelView mv;


void onIdle();

void onDisplay(void);

void onMouse(int who, int state, int x, int y);

void onMouseMotion(int x, int y);

void onKeyboard(unsigned char key, int x, int y);

void onKeyboardSpecial(int key, int x, int y);

void onWindowReshape(int width, int height);
