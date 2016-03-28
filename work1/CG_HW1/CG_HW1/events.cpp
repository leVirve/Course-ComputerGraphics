#include "events.h"


void onIdle()
{
    glutPostRedisplay();
}

void onDisplay(void)
{
    unsigned int mode = mv.solid ? GL_FILL : GL_LINE;

    // clear canvas
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glPolygonMode(GL_FRONT_AND_BACK, mode);

    glEnableVertexAttribArray(iLocPosition);
    glEnableVertexAttribArray(iLocColor);

    GLfloat vertex[9], color[9];
    GLMmodel* model = mv.get_model();

    for (int k = 0; k < (int) model->numtriangles; ++k) {

        for (int i = 0; i < 3; ++i) {
            int idx = model->triangles[k].vindices[i];
            for (int j = 0; j < 3; ++j) {
                vertex[i * 3 + j] = model->vertices[idx * 3 + j];
                color[i * 3 + j] = model->colors[idx * 3 + j];
            }
        }

        glVertexAttribPointer(iLocPosition, 3, GL_FLOAT, GL_FALSE, 0, vertex);
        glVertexAttribPointer(iLocColor, 3, GL_FLOAT, GL_FALSE, 0, color);

        // draw the array we just bound
        glDrawArrays(GL_TRIANGLES, 0, 3);
    }

    glutSwapBuffers();
}

void onMouse(int who, int state, int x, int y)
{
    printf("%18s(): (%d, %d) ", __FUNCTION__, x, y);

    switch (who)
    {
    case GLUT_LEFT_BUTTON:   printf("left button   "); break;
    case GLUT_MIDDLE_BUTTON: printf("middle button "); break;
    case GLUT_RIGHT_BUTTON:  printf("right button  "); break;
    case GLUT_WHEEL_UP:      printf("wheel up      "); break;
    case GLUT_WHEEL_DOWN:    printf("wheel down    "); break;
    default:                 printf("0x%02X          ", who); break;
    }

    switch (state)
    {
    case GLUT_DOWN: printf("start "); break;
    case GLUT_UP:   printf("end   "); break;
    }

    printf("\n");
}

void onMouseMotion(int x, int y)
{
    printf("%18s(): (%d, %d) mouse move\n", __FUNCTION__, x, y);
}

void onKeyboard(unsigned char key, int x, int y)
{
    printf("%18s(): (%d, %d) key: %c(0x%02X) ", __FUNCTION__, x, y, key, key);
    switch (key)
    {
    case GLUT_KEY_ESC: /* the Esc key */
        exit(0);
        break;
    case CHANGE_MODE_KEYw:
    case CHANGE_MODE_KEYW:
        mv.solid = mv.solid ? false : true;
        break;
    case NEXT_MODEL_KEY_x:
    case NEXT_MODEL_KEY_X:
        mv.loadNextModel();
        break;
    case PREV_MODEL_KEY_z:
    case PREV_MODEL_KEY_Z:
        mv.loadPrevModel();
        break;
    }
    printf("\n");
}

void onKeyboardSpecial(int key, int x, int y) {
    printf("%18s(): (%d, %d) ", __FUNCTION__, x, y);
    switch (key)
    {
    case GLUT_KEY_LEFT:
        printf("key: LEFT ARROW");
        break;

    case GLUT_KEY_RIGHT:
        printf("key: RIGHT ARROW");
        break;

    default:
        printf("key: 0x%02X      ", key);
        break;
    }
    printf("\n");
}


void onWindowReshape(int width, int height)
{
    printf("%18s(): %dx%d\n", __FUNCTION__, width, height);
}
