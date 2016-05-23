#include "cg.h"
#include "events.h"
#include "graphics.h"


std::map<char, Vector3> coord = {
    {POS_X_KEY, Vector3(1 , 0, 0)},
    {NEG_X_KEY, Vector3(-1, 0, 0)},
    {POS_Y_KEY, Vector3(0,  1, 0)},
    {NEG_Y_KEY, Vector3(0, -1, 0)},
    {POS_Z_KEY, Vector3(0, 0,  1)},
    {NEG_Z_KEY, Vector3(0, 0, -1)}
};

std::map<char, int> light_map = {
    {LIGHT::DIRECTIONAL, 0},
    {LIGHT::POINTLIGHT, 1},
    {LIGHT::SPOTLIGHT, 2},
    {LIGHT::NEXT, 1},
    {LIGHT::PREV, -1}
};


World::World(std::string folder)
{
    this->folder = folder;
    this->znear = 1, this->zfar = 20;
    this->gallery_size = 1;
    this->index = this->cur_idx = this->light_idx = 0;
    this->solid = true;
    this->parallel_project = true;
    this->spin_display = false;
    this->shading = SHADING::GOURAUD;

    this->setup_camera();
    this->setup_world_matrix();
    this->find_models(this->folder.c_str(), 0);
    this->size = filenames.size();
}

void World::activate()
{
    this->load_obj();
    setLightingSource();
}

void World::display()
{
    GLfloat _eye[] = { eye.x, eye.y, eye.z };
    glVertexAttribPointer(world.R.EyePosition, 3, GL_FLOAT, GL_FALSE, 0, _eye);

    glEnableVertexAttribArray(world.R.Position);
    glEnableVertexAttribArray(world.R.Normal);

    Matrix4 V = parallel_project ? camera_trans : pad_perspective * camera_trans;
    Matrix4 P = parallel_project ? P_parallel : P_perspective;

    for (int i = 0; i < gallery_size; ++i) {

        Matrix4 M = models[i]->get_model_trans_matrix();
        Matrix4 mvp = (P * V * M).transpose();
        M.transpose();
        V.transpose();

        glUniformMatrix4fv(world.R.MVP, 1, GL_FALSE, mvp.get());
        glUniformMatrix4fv(world.R.ModelTrans, 1, GL_FALSE, M.get());
        glUniformMatrix4fv(world.R.ViewTrans, 1, GL_FALSE, V.get());

        models[i]->draw_buffer();
    }
}

void World::update_lights()
{
    for (int i = 0; i < 3; ++i) {
#define GL_SEND(X) \
        glUniform4fv(world.R.LightSource[i].X, 1, lights[i].X)
        GL_SEND(position);
        GL_SEND(ambient);
        GL_SEND(diffuse);
        GL_SEND(specular);
        GL_SEND(spotDirection);
#undef GL_SEND
#define GL_SEND(X) \
        glUniform1f(world.R.LightSource[i].X, lights[i].X)
        GL_SEND(constantAttenuation);
        GL_SEND(linearAttenuation);
        GL_SEND(quadraticAttenuation);
        GL_SEND(spotExponent);
        GL_SEND(spotCutoff);
#undef GL_SEND
#define GL_SEND(X) \
        glUniform1i(world.R.LightSource[i].X, lights[i].X)
        GL_SEND(is_on);
#undef GL_SEND
    }
}

void World::move_light(Vector3& v)
{
    for (int i = 0; i < 3; ++i) lights[light_idx].position[i] += v[i];
    update_lights();
}

void World::handle_spotlight(char k)
{
    float scale = k == SPOTLIGHT::INC ? 0.1f : -0.1f;
    lights[light_map[LIGHT::SPOTLIGHT]].spotCutoff += scale;
    update_lights();
}

void World::handle_control(char k)
{
    switch (control_mode) {
    case CONTROL_MODE::TRANSLATE:
        cur_model->t.translate(coord[k] * 0.1f); break;
    case CONTROL_MODE::SCALE:
        cur_model->s.scale(coord[k] * 0.01f + Vector3(1, 1, 1)); break;
    case CONTROL_MODE::ROTATE:
        cur_model->r.rotate(1, coord[k]); break;
    case CONTROL_MODE::EYE:
        world.move_camera(coord[k] * 0.1f); break;
    case CONTROL_MODE::LIGHT:
        world.move_light(coord[k] * 0.1f); break;
    default: break;
    }
}

void World::setup_camera()
{
    up = Vector3(0, 1, 0),
    eye = Vector3(0, 0, 0),
    center = Vector3(0, 0, -1);
    move_camera(Vector3(0, 0, 0));
}

void World::setup_world_matrix()
{
    const float right = 1, left = -1, top = 1, bottom = -1;
    P_parallel = Matrix4(
        1, 0,  0, 0,
        0, 1,  0, 0,
        0, 0, -1, 0,
        0, 0,  0, 1
    );
    P_perspective = Matrix4(
        2 * znear / (right - left), 0, (right + left) / (right - left), 0,
        0, 2 * znear / (top - bottom), (top + bottom) / (top - bottom), 0,
        0, 0, -(zfar + znear) / (zfar - znear), -2 * zfar * znear / (zfar - znear),
        0, 0, -1, 0
    );
    pad_perspective = Matrix4(
        1, 0, 0,  0,
        0, 1, 0,  0,
        0, 0, 1, -2,
        0, 0, 0,  1
    );
}

void World::move_camera(Vector3& v)
{
    eye += v, up += v, center += v;
    Vector3 rz(center - eye),
            rx = rz.cross(up - eye),
            ry = rx.cross(rz);
    camera_trans = Matrix4(
         rx[0],  rx[1],  rx[2], 0,
         ry[0],  ry[1],  ry[2], 0,
        -rz[0], -rz[1], -rz[2], 0,
         0,      0,      0,     1
    ) * Matrix4().translate(-eye);
}

void World::select_model(char k)
{
    int next = k == SELECT_NEXT_KEY ? (cur_idx + 1) : (cur_idx + gallery_size - 1);
    this->cur_idx = next % gallery_size;
    this->cur_model = models[cur_idx];
    printf("- Model#%d is selected\n", cur_idx + 1);
}

void World::select_light(char k)
{
    int next = k == LIGHT::NEXT ? light_idx + 1 : light_idx + 3 - 1;
    this->light_idx = next % 3;
    printf("- LightSource#%d is selected\n", light_idx + 1);
}

void World::switch_model(char k)
{
    int next = k == GLUT_KEY_RIGHT ? (index + 1) : (size + index - 1);
    this->index = next % size;
    this->load_obj();
}

void World::toggle_gallery()
{
    for (int i = 0; i < gallery_size; ++i) delete this->models[i];
    this->gallery_size = gallery_size == 4 ? 1 : 4;
    this->load_obj();
}

void World::toggle_light(char k)
{
    int idx = light_map[k];
    glUniform1i(world.R.LightSource[idx].is_on, lights[idx].toggle());
}

void World::toggle_shading(char k)
{
    this->shading = shading == SHADING::GOURAUD ? SHADING::PHONE : SHADING::GOURAUD;
    glUniform1i(world.R.Shading, shading);
}

void World::load_obj()
{
    char title[1024];
    _snprintf(
        title, 1024, "(%d~%d/%d) %s - 10420 CS550000 CG HW Salas",
        index + 1, (index + gallery_size) % size, size, filenames[index].c_str());
    glutSetWindowTitle(title);

    for (int i = 0; i < gallery_size; ++i) {
        this->models[i] = new Model(filenames[(index + i) % size].c_str());
        if (gallery_size != 1) this->models[i]->s.scale(0.4f);
    }
    if (gallery_size == 4) {
        this->models[0]->t.translate(-0.5, 0.5, 0);
        this->models[1]->t.translate(0.5, 0.5, 0);
        this->models[2]->t.translate(-0.5, -0.5, 0);
        this->models[3]->t.translate(0.5, -0.5, 0);
    }
    this->cur_model = models[0];
    this->cur_idx = 0;
    printf("- Model#%d is focused\n", cur_idx + 1);
}

void World::find_models(const char *name, int level)
{
    DIR *dir;
    struct dirent *entry;

    if (!(dir = opendir(name))) return;
    if (!(entry = readdir(dir))) return;

    do {
        char path[1024];
        int len = _snprintf(path, sizeof(path) - 1, "%s/%s", name, entry->d_name);
        path[len] = '\0';

        if (entry->d_type == DT_DIR) {
            if (strcmp(entry->d_name, ".") == 0
             || strcmp(entry->d_name, "..") == 0) continue;
            printf("%*s[%s]\n", level * 2, "", entry->d_name);
            this->find_models(path, level + 1);
        }
        else {
            if (strstr(entry->d_name, ".mtl") || strstr(entry->d_name, ".bmp")) continue;
            this->filenames.push_back(path);
            printf("%*s- <loaded> %s\n", level * 2, "", entry->d_name);
        }
    } while (entry = readdir(dir));
    closedir(dir);
}

World::~World()
{
    for (int i = 0; i < gallery_size; ++i) delete this->models[i];
}
