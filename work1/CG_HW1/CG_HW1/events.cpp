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

    //MVP
    Matrix4 T;
    Matrix4 S;
    Matrix4 R;

    Matrix4 M = Matrix4(
        1, 0, 0, -0.5,
        0, 1, 0, 0,
        0, 0, 1, 0,
        0, 0, 0, 1);
    Matrix4 V = Matrix4(
        1, 0, 0, 0,
        0, 1, 0, 0,
        0, 0, 1, 0,
        0, 0, 0, 1);
    Matrix4 P = Matrix4(
        1, 0, 0, 0,
        0, 1, 0, 0,
        0, 0, -1, 0,
        0, 0, 0, 1);

    Matrix4 MVP = P*V*M;

    GLfloat mvp[16];
    // row-major ---> column-major
    mvp[0] = MVP[0];  mvp[4] = MVP[1];   mvp[8]  = MVP[2];    mvp[12] = MVP[3];
    mvp[1] = MVP[4];  mvp[5] = MVP[5];   mvp[9]  = MVP[6];    mvp[13] = MVP[7];
    mvp[2] = MVP[8];  mvp[6] = MVP[9];   mvp[10] = MVP[10];   mvp[14] = MVP[11];
    mvp[3] = MVP[12]; mvp[7] = MVP[13];  mvp[11] = MVP[14];   mvp[15] = MVP[15];

    // bind array pointers to shader
    glVertexAttribPointer(iLocPosition, 3, GL_FLOAT, GL_FALSE, 0, mv.model.vertices);
    glVertexAttribPointer(iLocColor,    3, GL_FLOAT, GL_FALSE, 0, mv.model.colors);

    // bind uniform matrix to shader
    glUniformMatrix4fv(iLocMVP, 1, GL_FALSE, mvp);

    // draw the array we just bound

    glDrawArrays(GL_TRIANGLES, 0, mv.model.size);

#if 0

    GLfloat v[9], c[9];
    GLMmodel* model = mv.get_model();

    glVertexAttribPointer(iLocPosition, 3, GL_FLOAT, GL_FALSE, 0, v);
    glVertexAttribPointer(iLocColor,    3, GL_FLOAT, GL_FALSE, 0, c);

    
    for (int k = 0; k < (int) model->numtriangles; ++k) {
        for (int i = 0; i < 3; ++i) {
            int idx = model->triangles[k].vindices[i];
            for (int j = 0; j < 3; ++j) {
                v[i * 3 + j] = model->vertices[idx * 3 + j];
                c[i * 3 + j] = model->colors[idx * 3 + j];
            }
        }
        glDrawArrays(GL_TRIANGLES, 0, 3);
    }
#endif

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
    char k = toupper(key);
    switch (k)
    {
    case GLUT_KEY_ESC: /* the Esc key */
        exit(0);
        break;
    case CHANGE_MODE_KEY_W:
        mv.toggleSolid();
        break;
    case NEXT_MODEL_KEY_X:
        mv.loadNextModel();
        break;
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
