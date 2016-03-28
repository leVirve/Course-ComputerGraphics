#pragma once

#include "cg.h"


class ModelView {

public:

    ModelView(std::string folder);

    void activate();

    void normalize(GLMmodel * model);

    GLMmodel* get_model();

    void loadNextModel();
    void loadPrevModel();

    ~ModelView();

    bool solid;

private:

    void findAllModels(const char *name, int level);

    void loadOBJ();

    int index;
    int size;
    GLMmodel* model;
    std::string folder;
    std::vector<std::string> filenames;

};

void showShaderCompileStatus(GLuint shader, GLint *shaderCompiled);

void setShaders();
