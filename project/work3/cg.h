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
#include "Texture.h"
#include "Matrices.h"


struct LightSource {
    float ambient[4];
    float diffuse[4];
    float specular[4];
    float position[4];
    float spotDirection[4];
    float spotExponent;
    float spotCutoff; // (range: [0.0,90.0], 180.0)
    float spotCosCutoff; // (range: [1.0,0.0],-1.0)
    float constantAttenuation;
    float linearAttenuation;
    float quadraticAttenuation;
    int is_on;

    int toggle() {
        is_on = is_on ? 0 : 1;
        return is_on;
    }
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
        GLint is_on;
    };

    GLint Shading;
    GLint Position;
    GLint MVP;
    GLint Normal;
    GLint TexCoord;
    GLint ModelTrans;
    GLint ViewTrans;
    GLint EyePosition;
    LightResource Material;
    LightResource LightSource[3];
};

enum CONTROL_MODE {
    TRANSLATE = 'T', ROTATE = 'R', SCALE = 'S', EYE = 'E', LIGHT = 'D' };
enum LIGHT {
    DIRECTIONAL='1', POINTLIGHT='2', SPOTLIGHT='3',
    PREV='7', NEXT='9',
    SHADE='5'
};
enum SPOTLIGHT { INC='+', DEC='-' };
enum SHADING { GOURAUD, PHONE };

class Model;


class World {

public:

    World(std::string folder);
    ~World();

    void display();

    void setup_camera();
    void setup_world_matrix();

    void handle_control(char k);
    void handle_spotlight(char k);

    void activate();

    void toggle_gallery();
    void toggle_light(char k);
    void toggle_shading(char k);

    void switch_model(char k);
    void select_model(char k);
    void select_light(char k);

    void move_camera(Vector3 & v);
    void move_light(Vector3 & v);
    void update_lights();

    bool solid;
    bool spin_display;
    bool parallel_project;
    int gallery_size;
    char control_mode;

    const static int max_models = 4;
    const static int max_lights = 3;

    Model* models[max_models];

    Model* cur_model;

    GLResource R;

    LightSource lights[max_lights];

    Vector3 up, eye, center;

    float znear, zfar;

private:

    void find_models(const char *name, int level);

    void load_obj();

    int index;
    int cur_idx;
    int light_idx;
    int size;

    int shading;

    Matrix4 camera_trans, pad_perspective;
    Matrix4 P_parallel, P_perspective;

    std::string folder;
    std::vector<std::string> filenames;

};

extern LightSource lights[3];
extern World world;

#ifdef VERBOSE
  #define VERBOSE(x) do { std::cout << x; } while (0)
  #define VERBOSE_P(X) X
#else
  #define VERBOSE(X)
  #define VERBOSE_P(X)
#endif // VERBOSE
