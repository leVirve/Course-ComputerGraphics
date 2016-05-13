#pragma once

#include "cg.h"
#include "graphics.h"

#ifndef GLUT_WHEEL_UP
# define GLUT_WHEEL_UP   0x0003
# define GLUT_WHEEL_DOWN 0x0004
#endif

#ifndef GLUT_KEY_ESC
# define GLUT_KEY_ESC	  0x1B
# define CHANGE_MODE_KEY_W 'W'
# define SELECT_NEXT_KEY   'X'
# define SELECT_PREV_KEY   'Z'
# define MODE_GALLERY_KEY  'G'
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
# define RESET_WORLD_KEY   'Q'
# define HELP_DOC_KEY      'H'
#endif


static std::map<char, const char*> mode_desc = {
    { MODE_EYE_KEY,    "Eye Mode" },
    { MODE_ROTATE_KEY, "Rotate Mode" },
    { MODE_TRANS_KEY,  "Translate Mode" },
    { MODE_SCALE_KEY,  "Scale Mode" },
};

static std::string doc = "##############################################################\n"
    "*                          Help doc                          *\n"
    "***          All commands are case-insensitive             ***\n"
    "##############################################################\n\n"
    "Controls: \n"
    "-\tT : Translate Mode\n"
    "-\tR : Rotate Mode\n"
    "-\tS : Scale Mode\n"
    "-\tE : Camera(Eye) Mode\n"
    "-\tP : Toggle parallel/perspective projection Mode\n\n"
    "-\tQ : Reset model(s)\n"
    "-\tG : Toggle gallery/single model Mode (testing)\n"
    "-\tW : Toggle wireframe/solid Mode\n\n"
    "-\tH : Help menu\n\n"
    "Operations: \n"
    "-\tL / J: positive X / negative X\n"
    "-\tI / K: positive Y / negative Y\n"
    "-\tM / O: positive Z / negative Z\n\n"
    "-\tX : Select(Focus) next model\n"
    "-\tZ : Select(Focus) previous model\n"
    "-\t->: Next model(s)\n"
    "-\t<-: Previous model(s)\n"
    "-\tScroll: scale up/down model\n"
    "-\tLeft Drag: translation on model\n"
    "";


void onIdle();

void onDisplay(void);

void onMouse(int who, int state, int x, int y);

void onMouseMotion(int x, int y);

void onKeyboard(unsigned char key, int x, int y);

void onKeyboardSpecial(int key, int x, int y);

void onWindowReshape(int width, int height);
