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
    if (m->numnormals == 0) {
        VERBOSE("Auto generating normals...");
        glmFacetNormals(m);
        glmVertexNormals(m, 90.0);
    }
    return m;
}

void Model::load_texture(SubModel& g, const std::string filename)
{
    FileHeader fh;
    InfoHeader ih;
    char *image;

    std::string filepath = "TextureModels/" + filename;
    VERBOSE("read " << filepath << std::endl);

    FILE *f = fopen(filepath.c_str(), "rb");
    fread(&fh, sizeof(FileHeader), 1, f);
    fread(&ih, sizeof(InfoHeader), 1, f);

    unsigned long size = ih.Width * ih.Height * 3;
    image = new char[size * sizeof(char)];
    fread(image, size * sizeof(char), 1, f);
    fclose(f);

    glGenTextures(1, &g.texture);

    glBindTexture(GL_TEXTURE_2D, g.texture);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, ih.Width, ih.Height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);

    glGenerateMipmap(GL_TEXTURE_2D);

    delete[] image;
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

    for (GLMgroup* g = body->groups; g; g = g->next, g_id++) {

        if (strlen(body->materials[g->material].textureImageName) != 0
            && strcmp(g->name, "default") != 0 /* for compability of default group */) {
            load_texture(groups[g_id], body->materials[g->material].textureImageName);
            VERBOSE("Texture resourse id " << groups[g_id].texture << std::endl);
        }

        unsigned int num_points = g->numtriangles * points_per_triangle;
        VERBOSE("Group " << g->name << ": " << num_points << " points\n");

        groups[g_id].num_points = num_points;
        groups[g_id].vertices   = new GLfloat[num_points * coords_per_point];
        groups[g_id].normals    = new GLfloat[num_points * coords_per_point];
        groups[g_id].texcoords  = new GLfloat[num_points * tex_coords_per_point];
        groups[g_id].material   = body->materials[g->material];

        for (unsigned int k = 0; k < g->numtriangles; ++k) {

            int tri_id = g->triangles[k];

            for (int i = 0; i < points_per_triangle; ++i) {

                int v_idx = body->triangles[tri_id].vindices[i];
                int n_idx = body->triangles[tri_id].nindices[i];
                int t_idx = body->triangles[tri_id].tindices[i];

                for (int dim = 0; dim < coords_per_point; ++dim) {
                    int buff_idx = (points_per_triangle * k + i) * coords_per_point + dim;
                    groups[g_id].vertices[buff_idx] = body->vertices[v_idx * coords_per_point + dim];
                    groups[g_id].normals[buff_idx] = body->normals[n_idx * coords_per_point + dim];
                }
                for (int dim = 0; dim < tex_coords_per_point; ++dim) {
                    int buff_idx = (points_per_triangle * k + i) * tex_coords_per_point + dim;
                    groups[g_id].texcoords[buff_idx] = body->texcoords[t_idx * tex_coords_per_point + dim];
                }
            }
        }
    }
}

void Model::draw_buffer()
{
    for (int i = 0; i < num_groups; ++i) {

        glVertexAttribPointer(world.R.Position, 3, GL_FLOAT, GL_FALSE, 0, groups[i].vertices);
        glVertexAttribPointer(world.R.Normal, 3, GL_FLOAT, GL_FALSE, 0, groups[i].normals);
        glVertexAttribPointer(world.R.TexCoord, 2, GL_FLOAT, GL_FALSE, 0, groups[i].texcoords);

        glUniform4fv(world.R.Material.ambient, 1, groups[i].material.ambient);
        glUniform4fv(world.R.Material.diffuse, 1, groups[i].material.diffuse);
        glUniform4fv(world.R.Material.specular, 1, groups[i].material.specular);
        glUniform1f(world.R.Material.shininess, groups[i].material.shininess);

        glBindTexture(GL_TEXTURE_2D, groups[i].texture);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, world.texture_mag_filter);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, world.texture_min_filter);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, world.texture_wrap_mode);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, world.texture_wrap_mode);

        glDrawArrays(GL_TRIANGLES, 0, groups[i].num_points);
    }
}
