#include "cg.h"
#include "events.h"
#include "graphics.h"


World::World(std::string folder)
{
    this->folder = folder;
    this->findAllModels(this->folder.c_str(), 0);
    this->index = this->cur_idx = 0;
    this->size = filenames.size();
    this->gallery_size = 1;
    this->solid = true;
    up = Vector3(0, 1, 0), eye = Vector3(0, 0, 0), center = Vector3(0, 0, -1);
}

void World::loadOBJ()
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

void World::findAllModels(const char *name, int level)
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
            this->findAllModels(path, level + 1);
        }
        else {
            if (strstr(entry->d_name, ".mtl")) continue;
            this->filenames.push_back(path);
            printf("%*s- <loaded> %s\n", level * 2, "", entry->d_name);
        }
    } while (entry = readdir(dir));
    closedir(dir);
}

void World::update_lights()
{
    for (int i = 0; i < 3; ++i) {
        glUniform4fv(world.R.LightSource[i].position, 1, world.lights[i].position);
        glUniform4fv(world.R.LightSource[i].ambient, 1, world.lights[i].ambient);
        glUniform4fv(world.R.LightSource[i].diffuse, 1, world.lights[i].diffuse);
        glUniform4fv(world.R.LightSource[i].specular, 1, world.lights[i].specular);
        glUniform4fv(world.R.LightSource[i].spotDirection, 1, world.lights[i].position);
        glUniform1f(world.R.LightSource[i].constantAttenuation, world.lights[i].constantAttenuation);
        glUniform1f(world.R.LightSource[i].linearAttenuation, world.lights[i].linearAttenuation);
        glUniform1f(world.R.LightSource[i].quadraticAttenuation, world.lights[i].quadraticAttenuation);
        glUniform1f(world.R.LightSource[i].spotExponent, world.lights[i].spotExponent);
        glUniform1f(world.R.LightSource[i].spotCutoff, world.lights[i].spotCutoff);
        glUniform1i(world.R.LightSource[i].is_on, world.lights[i].is_on);
    }
}

void World::activate()
{
    this->loadOBJ();
}

void World::selectNextModel()
{
    this->cur_idx = (cur_idx + 1) % max_models;
    this->cur_model = this->models[cur_idx];
    printf("- Model#%d is selected\n", cur_idx + 1);
}

void World::selectPrevModel()
{
    this->cur_idx = (cur_idx + max_models - 1) % max_models;
    this->cur_model = this->models[cur_idx];
    printf("- Model#%d is selected\n", cur_idx + 1);
}

void World::loadNextModel()
{
    this->index = (index + 1) % size;
    this->loadOBJ();
}

void World::loadPrevModel()
{
    this->index = (size + index - 1) % size;
    this->loadOBJ();
}

void World::toggleGallery()
{
    this->gallery_size = gallery_size == 4 ? 1 : 4;
    this->loadOBJ();
}

void World::toggleSolid()
{
    this->solid = solid ? false : true;
}

void World::toggleLight(int idx)
{
    glUniform1i(world.R.LightSource[idx].is_on, lights[idx].toggle());
}

World::~World()
{
    for (int i = 0; i < gallery_size; ++i) delete this->models[i];
}

void World::move_camera(Vector3& v)
{
    eye += v, up += v, center += v;

    Vector3 rz(center - eye),
            rx = rz.cross(up - eye),
            ry = rx.cross(rz);

    V = Matrix4(
         rx[0],  rx[1],  rx[2], 0,
         ry[0],  ry[1],  ry[2], 0,
        -rz[0], -rz[1], -rz[2], 0,
         0,      0,      0,     1
    ) * Matrix4().translate(-eye);
}

void World::move_light(Vector3& v)
{

}


std::map<char, Vector3> coord = {
    {POS_X_KEY, Vector3(1 , 0, 0)},
    {NEG_X_KEY, Vector3(-1, 0, 0)},
    {POS_Y_KEY, Vector3(0,  1, 0)},
    {NEG_Y_KEY, Vector3(0, -1, 0)},
    {POS_Z_KEY, Vector3(0, 0,  1)},
    {NEG_Z_KEY, Vector3(0, 0, -1)}
};


void World::control(char k)
{
    switch (control_mode) {
    case CONTROL_MODE::TRANSLATE:
        cur_model->t.translate(coord[k] * 0.1); break;
    case CONTROL_MODE::SCALE:
        cur_model->s.scale(coord[k] * 0.01 + Vector3(1, 1 , 1)); break;
    case CONTROL_MODE::ROTATE:
        cur_model->r.rotate(1, coord[k]); break;
    case CONTROL_MODE::EYE:
        world.move_camera(coord[k] * 0.1); break;
    case CONTROL_MODE::LIGHT:
        world.move_light(coord[k] * 0.1); break;
    default: break;
    }
}
