#include "events.h"

char control_mode;
char proj_mode = 0;

Vector3 up(0, 1, 0), eye(0, 0, 0), center(0, 0, -1);
Matrix4 T, S, R, N, M, V, P;
Matrix4 P_paral = Matrix4(
   1, 0, 0, 0,
   0, 1, 0, 0,
   0, 0, -1, 0,
   0, 0, 0, 1);
Matrix4 P_ortho = Matrix4(
    2 * znear / (right - left), 0, (right + left) / (right - left), 0,
    0, 2 * znear / (top - bottom), (top + bottom) / (top - bottom), 0,
    0, 0, -(zfar + znear) / (zfar - znear), -2 * zfar * znear / (zfar - znear),
    0, 0, -1, 0);
Matrix4 V_paral = Matrix4(
    1, 0, 0, 0,
    0, 1, 0, 0,
    0, 0, 1, 0,
    0, 0, 0, 1);
Matrix4 V_ortho = Matrix4(
    1, 0, 0, 0,
    0, 1, 0, 0,
    0, 0, 1, -2,
    0, 0, 0, 1);

Matrix4 move_eye(float x, float y, float z)
{
    Vector3 v(x, y, z);
    eye += v, up += v, center += v;

    Vector3 rz(center - eye);
    Vector3 rx = rz.cross(up - eye);
    Vector3 ry = rx.cross(rz);
    return Matrix4(
        rx[0],   rx[1],  rx[2], 0,
        ry[0],   ry[1],  ry[2], 0,
        -rz[0], -rz[1], -rz[2], 0,
        0, 0, 0, 1
    ) * Matrix4(
        1, 0, 0, -eye[0],
        0, 1, 0, -eye[1],
        0, 0, 1, -eye[2],
        0, 0, 0, 1
    );
}

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

    ////MVP
    Matrix4 v;
    if (proj_mode) { P = P_ortho; v = V_ortho; }
    else { P = P_paral; v = V_paral; }

    Matrix4 MVP = P*v*V*M*T*S*R*N;

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
    char k = toupper(key);
    switch (k)
    {
    case GLUT_KEY_ESC: exit(0); break;
    case CHANGE_MODE_KEY_W: mv.toggleSolid(); break;
    case NEXT_MODEL_KEY_X:  mv.loadNextModel(); break;
    case PREV_MODEL_KEY_Z:  mv.loadPrevModel(); break;
    case CH_PROJ_KEY: proj_mode = proj_mode ? 0 : 1; break;
    case MODE_TRANS_KEY:  case MODE_SCALE_KEY:
    case MODE_ROTATE_KEY: case MODE_EYE_KEY:
        control_mode = k; printf("In control mode: %s\n", mode_desc[k]); break;
    case POS_X_KEY:
        switch (control_mode) {
        case MODE_TRANS_KEY:  T.translate(0.1f, 0, 0); break;
        case MODE_SCALE_KEY:  S.scale(1.01f, 1, 1); break;
        case MODE_ROTATE_KEY: R.rotateX(1); break;
        case MODE_EYE_KEY:    V = move_eye(0.01f, 0, 0); break;
        default: break;
        }
        break;
    case NEG_X_KEY:
        switch (control_mode) {
        case MODE_TRANS_KEY:  T.translate(-0.1f, 0, 0); break;
        case MODE_SCALE_KEY:  S.scale(0.99f, 1, 1); break;
        case MODE_ROTATE_KEY: R.rotateX(-1); break;
        case MODE_EYE_KEY:    V = move_eye(-0.01f, 0, 0); break;
        default: break;
        }
        break;
    case POS_Y_KEY:
        switch (control_mode) {
        case MODE_TRANS_KEY:  T.translate(0, 0.1f, 0); break;
        case MODE_SCALE_KEY:  S.scale(1, 1.01f, 1); break;
        case MODE_ROTATE_KEY: R.rotateY(1); break;
        case MODE_EYE_KEY:    V = move_eye(0, 0.1f, 0); break;
        default: break;
        }
        break;
    case NEG_Y_KEY:
        switch (control_mode) {
        case MODE_TRANS_KEY:  T.translate(0, -0.1f, 0); break;
        case MODE_SCALE_KEY:  S.scale(1, 0.99f, 1); break;
        case MODE_ROTATE_KEY: R.rotateY(-1); break;
        case MODE_EYE_KEY:    V = move_eye(0, -0.1f, 0); break;
        default: break;
        }
        break;
    case POS_Z_KEY:
        switch (control_mode) {
        case MODE_TRANS_KEY:  T.translate(0, 0, 0.1f); break;
        case MODE_SCALE_KEY:  S.scale(1, 1, 1.01f); break;
        case MODE_ROTATE_KEY: R.rotateZ(1); break;
        case MODE_EYE_KEY:    V = move_eye(0, 0, 0.1f); break;
        default: break;
        }
        break;
    case NEG_Z_KEY:
        switch (control_mode) {
        case MODE_TRANS_KEY:  T.translate(0, 0, -0.1f); break;
        case MODE_SCALE_KEY:  S.scale(1, 1, 0.99f); break;
        case MODE_ROTATE_KEY: R.rotateZ(-1); break;
        case MODE_EYE_KEY:    V = move_eye(0, 0, -0.1f); break;
        default: break;
        }
        break;
    default:
        printf("%18s(): (%d, %d) key: %c(0x%02X)\n", __FUNCTION__, x, y, key, key);
        break;
    }
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
