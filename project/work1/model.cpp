#include "graphics.h"


Model::Model(const char* filename)
{
    this->vertices = this->colors = NULL;
    this->size = 0;
    this->body = this->normalize(glmReadOBJ((char*)filename));
    this->arrange_array(50000);
    this->load();
}

Model::~Model()
{
    if (this->vertices != NULL) free(this->vertices);
    if (this->colors != NULL) free(this->colors);
    if (this->body != NULL) glmDelete(this->body);
}

GLMmodel* Model::normalize(GLMmodel* m)
{
    GLfloat scale = 0, len;
    Vector3 centroid;

    for (int k = 0; k < 3; ++k) {
        GLfloat max = -FLT_MAX, min = FLT_MAX;
        for (unsigned int i = 1; i <= m->numvertices; ++i) {
            GLfloat val = m->vertices[3 * i + k];
            if (val > max) max = val;
            if (min > val) min = val;
        }

        centroid[k] = (max + min) / 2;
        len = (max - min) / 2;
        if (len > scale) scale = len;
        m->position[k] = 0;
    }

    Matrix4 _t, _s;
    _t.translate(-centroid / scale);
    _s.scale(1 / scale);
    this->n = _t * _s;

    return m;
}

void Model::load()
{
    this->size = body->numtriangles * 3;
    if (size > capacity) arrange_array(this->size);

    for (unsigned int k = 0; k < body->numtriangles; ++k) {
        for (int i = 0; i < 3; ++i) {
            int idx = body->triangles[k].vindices[i];
            for (int dim = 0; dim < 3; ++dim) {
                this->vertices[(3 * k + i) * 3 + dim] = body->vertices[idx * 3 + dim];
                this->colors[(3 * k + i) * 3 + dim] = body->colors[idx * 3 + dim];
            }
        }
    }
}

void Model::arrange_array(int n)
{
    if (vertices != NULL) free(vertices);
    if (colors != NULL) free(colors);
    this->vertices = (GLfloat*)malloc(sizeof(GLfloat) * 3 * n);
    this->colors = (GLfloat*)malloc(sizeof(GLfloat) * 3 * n);
    this->capacity = n;
}
