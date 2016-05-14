#include "events.h"
#include "shaders.h"
#include "graphics.h"

#pragma comment (lib, "glew32.lib")
#pragma comment (lib, "freeglut.lib")


World world("./NormalModels");


int main(int argc, char **argv)
{
    // glut init
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);

    // create window
    glutInitWindowPosition(500, 100);
    glutInitWindowSize(600, 600);
    glutCreateWindow("10420 CS550000 CG HW Salas");

    glewInit();
    if (glewIsSupported("GL_VERSION_2_0")) {
        printf("Ready for OpenGL 2.0\n");
    } else {
        printf("OpenGL 2.0 not supported\n");
        system("pause");
        exit(1);
    }

    world.activate();

    // register glut callback functions
    glutDisplayFunc (onDisplay);
    glutIdleFunc    (onIdle);
    glutKeyboardFunc(onKeyboard);
    glutSpecialFunc (onKeyboardSpecial);
    glutMouseFunc   (onMouse);
    glutMotionFunc  (onMouseMotion);
    glutReshapeFunc (onWindowReshape);

    setShaders();

    glEnable(GL_DEPTH_TEST);

    // main loop
    glutMainLoop();

    return 0;
}

