#include "graphics.h"


Model::Model(const char* filename)
{
    this->vertices = this->normals = NULL;
    this->size = this->capacity = 0;
    this->body = glmReadOBJ((char*) filename);
    
    glmFacetNormals(this->body);
    glmVertexNormals(this->body, 90.0);

    this->n = get_normalize_matix(this->body);
    this->load_to_buffer();
}

Model::~Model()
{
    if (this->vertices != NULL) free(this->vertices);
    if (this->normals != NULL) free(this->normals);
    if (this->body != NULL) glmDelete(this->body);
}

Matrix4 Model::get_normalize_matix(GLMmodel* m)
{
    GLfloat scale = 0, len;
    Vector3 centroid;
    Matrix4 _t, _s;

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

    _t.translate(-centroid / scale);
    _s.scale(1 / scale);

    return _t * _s;
}

void Model::load_to_buffer()
{
    this->size = body->numtriangles * 3;
    if (size > capacity) arrange_array(this->size);

    for (unsigned int k = 0; k < body->numtriangles; ++k) {
        for (int i = 0; i < 3; ++i) {
            int idx = body->triangles[k].vindices[i];
            for (int dim = 0; dim < 3; ++dim) {
                this->vertices[(3 * k + i) * 3 + dim] = body->vertices[idx * 3 + dim];
                this->normals[(3 * k + i) * 3 + dim] = body->normals[idx * 3 + dim];
            }
        }
    }
}

void Model::arrange_array(int n)
{
    if (vertices != NULL) free(vertices);
    if (normals != NULL) free(normals);
    this->vertices = (GLfloat*)malloc(sizeof(GLfloat) * 3 * n);
    this->normals = (GLfloat*)malloc(sizeof(GLfloat) * 3 * n);
    this->capacity = n;
}
