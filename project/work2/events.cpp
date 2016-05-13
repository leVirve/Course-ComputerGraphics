#include "events.h"

bool rotating = false;
char proj_mode = 0;
int lastX = -1, lastY = -1;

Matrix4 P;


void onIdle()
{
    //if (rotating) world.cur_model->r.rotateY(0.1);
    glutPostRedisplay();
}

void onDisplay(void)
{
    unsigned int mode = world.solid ? GL_FILL : GL_LINE;

    // clear canvas
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glPolygonMode(GL_FRONT_AND_BACK, mode);

    glEnableVertexAttribArray(world.R.Position);
    glEnableVertexAttribArray(world.R.Normal);

    static GLfloat eye[]={0, 0, 0};
    glVertexAttribPointer(world.R.EyePosition, 3, GL_FLOAT, GL_FALSE, 0, eye);

    Matrix4 v;
    if (proj_mode)
        { P = P_ortho; v = V_ortho; }
    else
        { P = P_paral; v = V_paral; }

    Matrix4 VP = P * v * world.V;

    for (int i = 0; i < world.gallery_size; ++i) {

        Matrix4 model_trans = (
            world.models[i]->t
            * world.models[i]->s
            * world.models[i]->r
            * world.models[i]->n);
        Matrix4 view_trans = (v * world.V).transpose();
        Matrix4 mvp = (VP * model_trans).transpose();
        model_trans.transpose();

        glUniformMatrix4fv(world.R.MVP, 1, GL_FALSE, mvp.get());
        glUniformMatrix4fv(world.R.ModelTrans, 1, GL_FALSE, model_trans.get());
        glUniformMatrix4fv(world.R.ViewTrans, 1, GL_FALSE, view_trans.get());

        world.models[i]->draw_buffer();
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
    case GLUT_WHEEL_UP:      world.cur_model->s.scale(1.01f); break;
    case GLUT_WHEEL_DOWN:    world.cur_model->s.scale(0.99f); break;
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
    world.cur_model->t.translate(dx / 800, -dy / 800, 0);
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
        world.toggleSolid(); break;
    case CH_PROJ_KEY:
        proj_mode = proj_mode ? 0 : 1; break;
    case MODE_GALLERY_KEY:
        world.toggleGallery(); break;
    case SELECT_NEXT_KEY:
        world.selectNextModel(); break;
    case SELECT_PREV_KEY:
        world.selectPrevModel(); break;
    case '1': world.toggleLight(0); break;
    case '2': world.toggleLight(1); break;
    case '3': world.toggleLight(2); break;
    case RESET_WORLD_KEY:
        world.up = Vector3(0, 1, 0), world.eye = Vector3(0, 0, 0), world.center = Vector3(0, 0, -1);
        world.move_camera(Vector3(0, 0, 0)); break;
    case CONTROL_MODE::TRANSLATE:
    case CONTROL_MODE::SCALE:
    case CONTROL_MODE::ROTATE:
    case CONTROL_MODE::EYE:
        if (k == MODE_ROTATE_KEY) rotating = rotating ? false : true;
        world.control_mode = k; printf("** In control mode: %s **\n", mode_desc[k]); break;
    case POS_X_KEY: case NEG_X_KEY:
    case POS_Y_KEY: case NEG_Y_KEY:
    case POS_Z_KEY: case NEG_Z_KEY:
        world.control(k); break;
    default:
        printf("%18s(): (%d, %d) key: %c(0x%02X)\n", __FUNCTION__, x, y, key, key);
        break;
    }
}

void onKeyboardSpecial(int key, int x, int y) {
    switch (key)
    {
    case GLUT_KEY_LEFT:  world.loadPrevModel(); break;
    case GLUT_KEY_RIGHT: world.loadNextModel(); break;
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
