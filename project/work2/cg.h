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

struct GLResource {

    struct LightResource {
        GLint ambient;
        GLint diffuse;
        GLint specular;
        GLint shininess;
        GLint position;
        GLint constantAttenuation;
        GLint linearAttenuation;
        GLint quadraticAttenuation;
        GLint spotDirection;
        GLint spotExponent;
        GLint spotCutoff;
        GLint spotCosCutoff;
    };

    GLint Position;
    GLint MVP;
    GLint Normal;
    GLint ModelTrans;
    GLint ViewTrans;
    GLint EyePosition;
    LightResource Material;
    LightResource LightSource[3];
};


class Model;


class World {

public:

    World(std::string folder);
    ~World();

    void activate();

    void toggleSolid();
    void toggleGallery();

    void loadNextModel();
    void loadPrevModel();

    void selectNextModel();
    void selectPrevModel();

    bool solid;
    int gallery_size;
    
    const static int max_models = 4;
    const static int max_lights = 3;

    Model* models[max_models];

    Model* cur_model;

    GLResource R;

    LightSource lights[max_lights];

private:

    void findAllModels(const char *name, int level);

    void loadOBJ();

    int index;
    int cur_idx;
    int size;

    std::string folder;
    std::vector<std::string> filenames;

};



const int right = 1, left = -1,
          top = 1, bottom = -1,
          znear = 1, zfar = 20;

const Matrix4 P_paral = 
    Matrix4(
        1, 0,  0, 0,
        0, 1,  0, 0,
        0, 0, -1, 0,
        0, 0,  0, 1
    );

const Matrix4 P_ortho =
    Matrix4(
        2 * znear / (right - left), 0, (right + left) / (right - left), 0,
        0, 2 * znear / (top - bottom), (top + bottom) / (top - bottom), 0,
        0, 0, -(zfar + znear) / (zfar - znear), -2 * zfar * znear / (zfar - znear),
        0, 0, -1, 0
     );

const Matrix4 V_paral;

const Matrix4 V_ortho = 
    Matrix4(
        1, 0, 0,  0,
        0, 1, 0,  0,
        0, 0, 1, -2,
        0, 0, 0,  1
    );


extern LightSource lights[3];
extern World world;
