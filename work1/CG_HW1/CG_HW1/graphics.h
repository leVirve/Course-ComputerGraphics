#pragma once

#include "cg.h"


class DisplayModel {

public:

    DisplayModel();

    ~DisplayModel();

    void load(GLMmodel* m);

    void arrange_array(int n);

    GLfloat* vertices;
    GLfloat* colors;

    int size;
    int capacity;

};


class ModelView {

public:

    ModelView(std::string folder);

    void activate();

    void normalize(GLMmodel* model);

    DisplayModel model;
    GLMmodel* get_model();

    void toggleSolid();

    void loadNextModel();
    void loadPrevModel();

    ~ModelView();

    bool solid;

private:

    void findAllModels(const char *name, int level);

    void loadOBJ();

    int index;
    int size;

    GLMmodel* glm_model;

    std::string folder;
    std::vector<std::string> filenames;

};

extern Matrix4 N;