#include "graphics.h"


Model::Model(const char* filename)
{
    this->vertices = this->normals = NULL;
    this->body = get_obj_model(filename);
    this->load_to_buffer();
}

Model::~Model()
{
    for (int i = 0; i < num_groups; ++i) {
        delete groups[i].vertices;
        delete groups[i].normals;
    }
}

GLMmodel* Model::get_obj_model(const char* filename)
{    
    GLMmodel* m = glmReadOBJ((char*) filename);
    glmFacetNormals(m);
    glmVertexNormals(m, 90.0);
    return m;
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
    this->n = get_normalize_matix(this->body);

    int g_id = 0;
    num_groups = body->numgroups;
    groups = new SubModel[num_groups];

    for (GLMgroup* g = body->groups; g; g = g->next) {
        unsigned int num_points = g->numtriangles * points_per_triangle;
        groups[g_id].num_points = num_points;
        groups[g_id].vertices = new GLfloat[num_points * coords_per_point];
        groups[g_id].normals = new GLfloat[num_points * coords_per_point];
        groups[g_id].material = body->materials[g->material];

        for (unsigned int k = 0; k < g->numtriangles; ++k) {
            for (int i = 0; i < 3; ++i) {
                int tri_id = g->triangles[k];
                int v_idx = body->triangles[tri_id].vindices[i];
                int n_idx = body->triangles[tri_id].nindices[i];
                for (int dim = 0; dim < 3; ++dim) {
                    groups[g_id].vertices[(3 * k + i) * 3 + dim] = body->vertices[v_idx * 3 + dim];
                    groups[g_id].normals[(3 * k + i) * 3 + dim] = body->normals[n_idx * 3 + dim];
                }
            }
        }
        g_id++;
    }
}
