#pragma once

#include "cg.h"


const int right = 1, left = -1, top = 1, bottom = -1,
          znear = 1, zfar = 20;
const int max_models = 4;

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


class ModelView {

public:

    ModelView(std::string folder);

    void activate();

    void toggleSolid();

    void toggleGallery();

    void loadNextModel();

    void loadPrevModel();

    void selectNextModel();

    void selectPrevModel();

    ~ModelView();

    bool solid;

    int gallery_size;

    Model* models[max_models];

    Model* cur_model;

private:

    void findAllModels(const char *name, int level);

    void loadOBJ();

    int index;
    int cur_idx;
    int size;

    std::string folder;
    std::vector<std::string> filenames;

};

void setLightingSource();
