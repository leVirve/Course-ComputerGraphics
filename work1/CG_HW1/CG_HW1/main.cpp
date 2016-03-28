#include "dirent.h"

#include "events.h"
#include "graphics.h"

#pragma comment (lib, "glew32.lib")
#pragma comment (lib, "freeglut.lib")

using namespace std;

// Shader attributes
GLint iLocPosition;
GLint iLocColor;
GLMmodel* model;

bool solid = false;
unsigned int curr_model_index = 0;
vector<string> modelfiles;


void listObjdir(const char *name, int level)
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
            listObjdir(path, level + 1);
        } else {
            modelfiles.push_back(path);
            printf("%*s- <loaded> %s\n", level * 2, "", entry->d_name);
        }
    } while (entry = readdir(dir));
    closedir(dir);
}

int main(int argc, char **argv)
{
    // glut init
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);

    // create window
    glutInitWindowPosition(500, 100);
    glutInitWindowSize(600, 600);
    glutCreateWindow("10420 CS550000 CG HW1 Salas");

    glewInit();
    if (glewIsSupported("GL_VERSION_2_0")) {
        printf("Ready for OpenGL 2.0\n");
    } else {
        printf("OpenGL 2.0 not supported\n");
        system("pause");
        exit(1);
    }

    listObjdir("./ColorModels", 0);

    // load obj models through glm
    loadOBJModel(curr_model_index);

    // register glut callback functions
    glutDisplayFunc (onDisplay);
    glutIdleFunc    (onIdle);
    glutKeyboardFunc(onKeyboard);
    glutSpecialFunc (onKeyboardSpecial);
    glutMouseFunc   (onMouse);
    glutMotionFunc  (onMouseMotion);
    glutReshapeFunc (onWindowReshape);

    // set up shaders here
    setShaders();

    glEnable(GL_DEPTH_TEST);

    // main loop
    glutMainLoop();

    // free
    glmDelete(model);

    return 0;
}

