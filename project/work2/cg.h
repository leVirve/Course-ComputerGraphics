#pragma once

// C headers
#include <stdio.h>
#include <stdlib.h>
#include <float.h>

// C++ headers
#include <iostream>
#include <string>
#include <vector>
#include <map>

// depebdencies
#include <GL/glew.h>
#include <freeglut/glut.h>

// self-headers
#include "glm.h"
#include "dirent.h"
#include "Matrices.h"


struct LightSource {
    float ambient[4];
    float diffuse[4];
    float specular[4];
    float position[4];
    float halfVector[4];
    float spotDirection[4];
    float spotExponent;
    float spotCutoff; // (range: [0.0,90.0], 180.0)
    float spotCosCutoff; // (range: [1.0,0.0],-1.0)
    float constantAttenuation;
    float linearAttenuation;
    float quadraticAttenuation;
};

struct iLoc {
    GLint ambient;
    GLint diffuse;
    GLint specular;
    GLint shininess;
    GLint position;
};

extern GLint iLocPosition;
extern GLint iLocMVP;
extern GLint iLocNormal;
extern GLint iLocModelTrans;
extern GLint iLocViewTrans;
extern GLint iLocEyePos;
extern iLoc iLocMaterial;
extern iLoc iLocLight[3];

extern LightSource lights[3];
