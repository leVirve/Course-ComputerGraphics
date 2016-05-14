#include "events.h"

int lastX = -1, lastY = -1;

void onIdle()
{
    if (world.spin_display) world.cur_model->r.rotateY(0.1f);
    glutPostRedisplay();
}

void onDisplay(void)
{
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glPolygonMode(GL_FRONT_AND_BACK, world.solid ? GL_FILL : GL_LINE);

    world.display();

    glutSwapBuffers();
}

void onMouse(int who, int state, int x, int y)
{
    switch (who) {
    case GLUT_LEFT_BUTTON:
    case GLUT_MIDDLE_BUTTON:
    case GLUT_RIGHT_BUTTON:  break;
    case GLUT_WHEEL_UP:      world.cur_model->s.scale(1.01f); break;
    case GLUT_WHEEL_DOWN:    world.cur_model->s.scale(0.99f); break;
    default: printf("%18s(): (%d, %d) 0x%02X", __FUNCTION__, x, y, who); break;
    }

    switch (state) {
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
    switch (k) {
    case GLUT_KEY_ESC: exit(0); break;
    case HELP_DOC_KEY: system("clear"); puts(doc.c_str()); break;
    case RESET_WORLD_KEY: world.setup_camera(); break;
    case CHANGE_MODE_KEY_W: world.solid = !world.solid; break;
    case CH_PROJ_KEY: world.parallel_project = !world.parallel_project; break;
    case MODE_GALLERY_KEY:
        world.toggle_gallery(); break;
    case SELECT_NEXT_KEY: case SELECT_PREV_KEY:
        world.select_model(k); break;
    case LIGHT::PREV: case LIGHT::NEXT:
        world.select_light(k); break;
    case LIGHT::DIRECTIONAL:
    case LIGHT::POINTLIGHT:
    case LIGHT::SPOTLIGHT:
        world.toggle_light(k); break;
    case CONTROL_MODE::TRANSLATE:
    case CONTROL_MODE::SCALE:
    case CONTROL_MODE::ROTATE:
    case CONTROL_MODE::EYE:
    case CONTROL_MODE::LIGHT:
        if (k == CONTROL_MODE::ROTATE) world.spin_display = !world.spin_display;
        world.control_mode = k;
        printf("** In control mode: %s **\n", mode_desc[k]); break;
    case SPOTLIGHT::INC: case SPOTLIGHT::DEC:
        world.handle_spotlight(k); break;
    case POS_X_KEY: case NEG_X_KEY:
    case POS_Y_KEY: case NEG_Y_KEY:
    case POS_Z_KEY: case NEG_Z_KEY:
        world.handle_control(k); break;
    default:
        printf("%18s(): (%d, %d) key: %c(0x%02X)\n", __FUNCTION__, x, y, key, key);
        break;
    }
}

void onKeyboardSpecial(int key, int x, int y) {
    switch (key) {
    case GLUT_KEY_LEFT:
    case GLUT_KEY_RIGHT:
        world.switch_model(key); break;
    default: break;
    }
}


void onWindowReshape(int width, int height)
{
    if (height == 0) height = 1;
    GLfloat fAspect = (GLfloat) width / (GLfloat) height;

    glViewport(0, 0, width, height);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(90.0f, fAspect, world.znear, world.zfar);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}
