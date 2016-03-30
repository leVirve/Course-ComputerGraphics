#include "graphics.h"


ModelView::ModelView(std::string folder)
{
    this->folder = folder;
    this->findAllModels(this->folder.c_str(), 0);
    this->glm_model = NULL;
    this->index = 0;
    this->size = filenames.size();
}

void ModelView::normalize(GLMmodel* model)
{
    GLfloat scale = 0, centroid, len;

    for (int k = 0; k < 3; ++k) {
        GLfloat max = -FLT_MAX, min = FLT_MAX;
        for (unsigned int i = 1; i <= model->numvertices; ++i) {
            GLfloat val = model->vertices[3 * i + k];
            if (val > max) max = val;
            if (min > val) min = val;
        }

        centroid = model->position[k] = (max + min) / 2;
        len = (max - min) / 2;
        if (len > scale) scale = len;

        for (unsigned int i = 1; i <= model->numvertices; ++i)
            model->vertices[3 * i + k] -= centroid;
    }

    for (int k = 0; k < 3; ++k)
        for (unsigned int i = 0; i <= model->numvertices; ++i)
            model->vertices[3 * i + k] /= scale;
}

void ModelView::loadOBJ()
{
    if (glm_model != NULL) glmDelete(glm_model);

    glm_model = glmReadOBJ((char*) filenames[index].c_str());
    std::cout << filenames[index] << std::endl;

    char title[1024];
    snprintf(
        title, 1024, "(%d/%d) %s - 10420 CS550000 CG HW1 Salas",
        index + 1, size, filenames[index].c_str());
    glutSetWindowTitle(title);

    this->normalize(glm_model);

    this->model.load(glm_model);
}

void ModelView::findAllModels(const char *name, int level)
{
    DIR *dir;
    struct dirent *entry;

    if (!(dir = opendir(name))) return;
    if (!(entry = readdir(dir))) return;

    do {
        char path[1024];
        int len = snprintf(path, sizeof(path) - 1, "%s/%s", name, entry->d_name);
        path[len] = '\0';

        if (entry->d_type == DT_DIR) {
            if (strcmp(entry->d_name, ".") == 0
             || strcmp(entry->d_name, "..") == 0) continue;
            printf("%*s[%s]\n", level * 2, "", entry->d_name);
            this->findAllModels(path, level + 1);
        }
        else {
            this->filenames.push_back(path);
            printf("%*s- <loaded> %s\n", level * 2, "", entry->d_name);
        }
    } while (entry = readdir(dir));
    closedir(dir);
}

GLMmodel* ModelView::get_model()
{
    return this->glm_model;
}

void ModelView::activate()
{
    this->loadOBJ();
}

void ModelView::loadNextModel()
{
    this->index = (index + 1) % size;
    this->loadOBJ();
}

void ModelView::loadPrevModel()
{
    this->index = (size + index - 1) % size;
    this->loadOBJ();
}

void ModelView::toggleSolid()
{
    this->solid = solid ? false : true;
}

ModelView::~ModelView()
{
    glmDelete(this->glm_model);
}


DisplayModel::DisplayModel()
{
    this->size = 0;
    this->arrange_array(50000);
}

DisplayModel::~DisplayModel()
{
    free(this->vertices);
    free(this->colors);
}

void DisplayModel::load(GLMmodel* m)
{
    this->size = m->numtriangles * 3;
    if (size > capacity) arrange_array(this->size);

    for (unsigned int k = 0; k < m->numtriangles; ++k) {
        for (int i = 0; i < 3; ++i) {
            int idx = m->triangles[k].vindices[i];
            for (int dim = 0; dim < 3; ++dim) {
                this->vertices[(3 * k + i) * 3 + dim] = m->vertices[idx * 3 + dim];
                this->colors[(3 * k + i) * 3 + dim] = m->colors[idx * 3 + dim];
            }
        }
    }
}

void DisplayModel::arrange_array(int n)
{
    if (vertices != NULL) free(vertices);
    if (colors != NULL) free(colors);
    this->vertices = (GLfloat*)malloc(sizeof(GLfloat) * 3 * n);
    this->colors = (GLfloat*)malloc(sizeof(GLfloat) * 3 * n);
    this->capacity = n;
    std::cerr << "Mem reallocate to capacity = " << capacity << std::endl;
}

void showShaderCompileStatus(GLuint shader, GLint *shaderCompiled)
{
    glGetShaderiv(shader, GL_COMPILE_STATUS, shaderCompiled);
    if (GL_FALSE == (*shaderCompiled))
    {
        GLint maxLength = 0;
        glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &maxLength);

        // The maxLength includes the NULL character.
        GLchar *errorLog = (GLchar*)malloc(sizeof(GLchar) * maxLength);
        glGetShaderInfoLog(shader, maxLength, &maxLength, &errorLog[0]);
        fprintf(stderr, "%s", errorLog);

        glDeleteShader(shader);
        free(errorLog);
    }
}

void setShaders()
{
    GLuint v, f, p;
    char *vs = NULL;
    char *fs = NULL;

    v = glCreateShader(GL_VERTEX_SHADER);
    f = glCreateShader(GL_FRAGMENT_SHADER);

    vs = textFileRead("shader.vert");
    fs = textFileRead("shader.frag");

    glShaderSource(v, 1, (const GLchar**)&vs, NULL);
    glShaderSource(f, 1, (const GLchar**)&fs, NULL);

    free(vs);
    free(fs);

    // compile vertex shader
    glCompileShader(v);
    GLint vShaderCompiled;
    showShaderCompileStatus(v, &vShaderCompiled);
    if (!vShaderCompiled) system("pause"), exit(123);

    // compile fragment shader
    glCompileShader(f);
    GLint fShaderCompiled;
    showShaderCompileStatus(f, &fShaderCompiled);
    if (!fShaderCompiled) system("pause"), exit(456);

    p = glCreateProgram();

    // bind shader
    glAttachShader(p, f);
    glAttachShader(p, v);

    // link program
    glLinkProgram(p);

    iLocPosition = glGetAttribLocation(p, "av4position");
    iLocColor = glGetAttribLocation(p, "av3color");

    glUseProgram(p);
}
