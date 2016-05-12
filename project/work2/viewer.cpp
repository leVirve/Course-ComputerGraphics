#include "graphics.h"


ModelView::ModelView(std::string folder)
{
    this->folder = folder;
    this->findAllModels(this->folder.c_str(), 0);
    this->index = this->cur_idx = 0;
    this->size = filenames.size();
    this->gallery_size = 1; // max_models;
    this->solid = true;
}

void ModelView::loadOBJ()
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

void ModelView::findAllModels(const char *name, int level)
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

void ModelView::activate()
{
    this->loadOBJ();
}

void ModelView::selectNextModel()
{
    this->cur_idx = (cur_idx + 1) % max_models;
    this->cur_model = this->models[cur_idx];
    printf("- Model#%d is selected\n", cur_idx + 1);
}

void ModelView::selectPrevModel()
{
    this->cur_idx = (cur_idx + max_models - 1) % max_models;
    this->cur_model = this->models[cur_idx];
    printf("- Model#%d is selected\n", cur_idx + 1);
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

void ModelView::toggleGallery()
{
    this->gallery_size = gallery_size == 4 ? 1 : 4;
    this->loadOBJ();
}

void ModelView::toggleSolid()
{
    this->solid = solid ? false : true;
}

ModelView::~ModelView()
{
    for (int i = 0; i < gallery_size; ++i) delete this->models[i];
}
