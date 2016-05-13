#pragma once

#include "cg.h"


const int points_per_triangle = 3;
const int coords_per_point = 3;

struct SubModel {

    GLfloat *vertices, *normals;

    GLMmaterial material;

    unsigned int num_points;
};


class Model {

public:

    Model(const char* filename);

    ~Model();

    GLMmodel * get_obj_model(const char * filename);

    Matrix4 t, s, r, n;

    GLfloat* vertices;

    GLfloat* normals;

    SubModel* groups;

    int num_groups;

    void draw_buffer();

private:

    void load_to_buffer();

    Matrix4 get_normalize_matix(GLMmodel* m);

    GLMmodel* body;

};

void setLightingSource();
