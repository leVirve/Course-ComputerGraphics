#include "events.h"

char control_mode;
char proj_mode = 0;
int lastX = -1, lastY = -1;

Vector3 up(0, 1, 0), eye(0, 0, 0), center(0, 0, -1);
Matrix4 M, V, P;

Matrix4 move_eye(float x, float y, float z)
{
    Vector3 v(x, y, z);
    eye += v, up += v, center += v;

    Vector3 rz(center - eye),
            rx = rz.cross(up - eye),
            ry = rx.cross(rz);
    return Matrix4(
         rx[0],  rx[1],  rx[2], 0,
         ry[0],  ry[1],  ry[2], 0,
        -rz[0], -rz[1], -rz[2], 0,
         0,      0,      0,     1
    ) * Matrix4().translate(-eye);
}

void onIdle()
{
    mv.cur_model->r.rotateY(0.1);
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
    glEnableVertexAttribArray(iLocNormal);

    static GLfloat eye[]={0, 0, 0};
    glVertexAttribPointer(iLocEyePos, 3, GL_FLOAT, GL_FALSE, 0, eye);

    Matrix4 v;
    if (proj_mode)
        { P = P_ortho; v = V_ortho; }
    else
        { P = P_paral; v = V_paral; }

    Matrix4 MVP = P * v * V * M;

    for (int i = 0; i < mv.gallery_size; ++i) {

        Matrix4 model_trans = (
            mv.models[i]->t
            * mv.models[i]->s
            * mv.models[i]->r
            * mv.models[i]->n);
        Matrix4 view_trans = (v * V).transpose();
        Matrix4 mvp = (MVP * model_trans).transpose();
        model_trans.transpose();

        glUniformMatrix4fv(iLocMVP, 1, GL_FALSE, mvp.get());
        glUniformMatrix4fv(iLocModelTrans, 1, GL_FALSE, model_trans.get());
        glUniformMatrix4fv(iLocViewTrans, 1, GL_FALSE, view_trans.get());

        mv.cur_model->draw_buffer();
    }
    glutSwapBuffers();
}

void onMouse(int who, int state, int x, int y)
{
    switch (who)
    {
    case GLUT_LEFT_BUTTON:   break;
    case GLUT_MIDDLE_BUTTON: break;
    case GLUT_RIGHT_BUTTON:  break;
    case GLUT_WHEEL_UP:      mv.cur_model->s.scale(1.01f); break;
    case GLUT_WHEEL_DOWN:    mv.cur_model->s.scale(0.99f); break;
    default:
        printf("%18s(): (%d, %d) ", __FUNCTION__, x, y);
        printf("0x%02X          ", who);
        break;
    }

    switch (state)
    {
    case GLUT_DOWN: lastX = x, lastY = y; break;
    case GLUT_UP:   lastX = lastY = -1;   break;
    }
}

void onMouseMotion(int x, int y)
{
    float dx = (float) x - lastX, dy = (float) y - lastY;
    mv.cur_model->t.translate(dx / 800, -dy / 800, 0);
    lastX = x, lastY = y;
}

void onKeyboard(unsigned char key, int x, int y)
{
    char k = toupper(key);
    switch (k)
    {
    case GLUT_KEY_ESC:
        exit(0); break;
    case HELP_DOC_KEY:
        system("clear");
        std::cout << doc << "\n"; break;
    case CHANGE_MODE_KEY_W:
        mv.toggleSolid(); break;
    case CH_PROJ_KEY:
        proj_mode = proj_mode ? 0 : 1; break;
    case MODE_GALLERY_KEY:
        mv.toggleGallery(); break;
    case SELECT_NEXT_KEY:
        mv.selectNextModel(); break;
    case SELECT_PREV_KEY:
        mv.selectPrevModel(); break;
    case RESET_WORLD_KEY:
        up = Vector3(0, 1, 0), eye = Vector3(0, 0, 0), center = Vector3(0, 0, -1);
        V = move_eye(0, 0, 0); break;
    case MODE_TRANS_KEY:
    case MODE_SCALE_KEY:
    case MODE_ROTATE_KEY:
    case MODE_EYE_KEY:
        control_mode = k; printf("** In control mode: %s **\n", mode_desc[k]); break;
    case POS_X_KEY:
        switch (control_mode) {
        case MODE_TRANS_KEY:  mv.cur_model->t.translate(0.1f, 0, 0); break;
        case MODE_SCALE_KEY:  mv.cur_model->s.scale(1.01f, 1, 1); break;
        case MODE_ROTATE_KEY: mv.cur_model->r.rotateX(1); break;
        case MODE_EYE_KEY:    V = move_eye(0.01f, 0, 0); break;
        default: break;
        }
        break;
    case NEG_X_KEY:
        switch (control_mode) {
        case MODE_TRANS_KEY:  mv.cur_model->t.translate(-0.1f, 0, 0); break;
        case MODE_SCALE_KEY:  mv.cur_model->s.scale(0.99f, 1, 1); break;
        case MODE_ROTATE_KEY: mv.cur_model->r.rotateX(-1); break;
        case MODE_EYE_KEY:    V = move_eye(-0.01f, 0, 0); break;
        default: break;
        }
        break;
    case POS_Y_KEY:
        switch (control_mode) {
        case MODE_TRANS_KEY:  mv.cur_model->t.translate(0, 0.1f, 0); break;
        case MODE_SCALE_KEY:  mv.cur_model->s.scale(1, 1.01f, 1); break;
        case MODE_ROTATE_KEY: mv.cur_model->r.rotateY(1); break;
        case MODE_EYE_KEY:    V = move_eye(0, 0.1f, 0); break;
        default: break;
        }
        break;
    case NEG_Y_KEY:
        switch (control_mode) {
        case MODE_TRANS_KEY:  mv.cur_model->t.translate(0, -0.1f, 0); break;
        case MODE_SCALE_KEY:  mv.cur_model->s.scale(1, 0.99f, 1); break;
        case MODE_ROTATE_KEY: mv.cur_model->r.rotateY(-1); break;
        case MODE_EYE_KEY:    V = move_eye(0, -0.1f, 0); break;
        default: break;
        }
        break;
    case POS_Z_KEY:
        switch (control_mode) {
        case MODE_TRANS_KEY:  mv.cur_model->t.translate(0, 0, 0.1f); break;
        case MODE_SCALE_KEY:  mv.cur_model->s.scale(1, 1, 1.01f); break;
        case MODE_ROTATE_KEY: mv.cur_model->r.rotateZ(1); break;
        case MODE_EYE_KEY:    V = move_eye(0, 0, 0.1f); break;
        default: break;
        }
        break;
    case NEG_Z_KEY:
        switch (control_mode) {
        case MODE_TRANS_KEY:  mv.cur_model->t.translate(0, 0, -0.1f); break;
        case MODE_SCALE_KEY:  mv.cur_model->s.scale(1, 1, 0.99f); break;
        case MODE_ROTATE_KEY: mv.cur_model->r.rotateZ(-1); break;
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
    switch (key)
    {
    case GLUT_KEY_LEFT:  mv.loadPrevModel(); break;
    case GLUT_KEY_RIGHT: mv.loadNextModel(); break;
    default: break;
    }
}


void onWindowReshape(int width, int height)
{
    if (height == 0) height = 1;

    // Set Viewport to window dimensions
    glViewport(0, 0, width, height);

    GLfloat fAspect = (GLfloat)width / (GLfloat)height;

    // Reset coordinate system
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    // Produce the perspective projection
    gluPerspective(90.0f, fAspect, znear, zfar);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}
