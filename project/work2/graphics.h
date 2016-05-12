#pragma once

#include "cg.h"


const int right = 1, left = -1, top = 1, bottom = -1,
          znear = 1, zfar = 20;
const int max_models = 4;


class Model {

public:

    Model(const char* filename);

    ~Model();

    GLMmodel * get_obj_model(const char * filename);

    Matrix4 t, s, r, n;
    GLfloat* vertices;
    GLfloat* normals;
    int size;

private:

    void load_to_buffer();

    void arrange_array(int n);

    Matrix4 get_normalize_matix(GLMmodel* m);

    GLMmodel* body;

    int capacity;
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
