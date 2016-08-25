#include <gl\glut.h>

void Initialize();
void MouseHandler(int button, int state, int x, int y);
void KeyboardHandler(unsigned char key, int x, int y);
void MainMenuHandler(int option);
void Animate();
void Reshape(int width, int height);
void Display();

/****************************************************************************
 main()

 Setup GLUT and OpenGL, drop into the event loop
*****************************************************************************/
int main(int argc, char **argv)
{
    // Setup the basic GLUT stuff
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    // Create the window
    glutInitWindowSize(1024, 768);
    glutInitWindowPosition(100, 150);
    glutCreateWindow("BOGLGP Chapter 1");
    Initialize();
    // Register the event callback functions
    glutDisplayFunc(Display);
    glutReshapeFunc(Reshape);
    glutMouseFunc(MouseHandler);
    glutKeyboardFunc(KeyboardHandler);
    glutIdleFunc(Animate);
    // At this point, control is relinquished to the GLUT event handler.
    // Control is returned as events occur, via the callback functions.
    glutMainLoop();
    return 0;
} // end main()


/****************************************************************************
 Initialize()

 One time setup, including creating menus, creating a light, setting the
 shading mode and clear color, and loading textures.
*****************************************************************************/
void Initialize()
{
    // set up the only meny
    int mainMenu;
    mainMenu = glutCreateMenu(MainMenuHandler);
    glutSetMenu(mainMenu);
    glutAddMenuEntry("Exit", 0);
    glutAttachMenu(GLUT_RIGHT_BUTTON);
    glEnable(GL_DEPTH_TEST);
} // end Initialize()


/****************************************************************************
 MouseHandler()

 Handle mouse events. For this simple demo, just exit on a left click.
*****************************************************************************/
void MouseHandler(int button, int state, int x, int y)
{
    switch (button) {
    case GLUT_LEFT_BUTTON: {
        exit(0);
    }
    break;

    default:
        break;
    }

    // force a screen redraw
    glutPostRedisplay();
} // end MouseHandler()


/****************************************************************************
 KeyboardHandler()

 Keyboard handler. Again, we'll just exit when q is pressed.
*****************************************************************************/
void KeyboardHandler(unsigned char key, int x, int y)
{
    switch (key) {
    case 'q': { // exit
        exit(0);
    }
    break;

    default: {
    } break;
    }

    glutPostRedisplay();
} // end KeyboardHandler()


/****************************************************************************
 MainMenuHandler()

 Main menu callback.
*****************************************************************************/
void MainMenuHandler(int option)
{
    switch (option) {
    case 0: {
        exit(0);
    }
    break;

    default:
        break;
    }

    glutPostRedisplay();
} // end MainMenuHandler()


/****************************************************************************
 Animate()

 Rotate the cube by 4 degrees and force a redisplay.
*****************************************************************************/
void Animate()
{
    glutPostRedisplay();
} // end Animate()


/****************************************************************************
 Reshape()

 Reset the viewport for window changes
*****************************************************************************/
void Reshape(int width, int height)
{
    if (height == 0)
        return;

    glViewport(0, 0, (GLsizei) width, (GLsizei) height);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(90.0, width / height, 1.0, 100.0);
    glMatrixMode(GL_MODELVIEW);
} // end Reshape


/****************************************************************************
 Display()

 Clear and redraw the scene.
*****************************************************************************/
void Display()
{
    // set up the camera
    glLoadIdentity();
    gluLookAt(0.0, 1.0, 6.0,
              0.0, 0.0, 0.0,
              0.0, 1.0, 0.0);
    // clear the screen
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    // draw a triangle
    glBegin(GL_TRIANGLES);
    glColor3f(1.0, 0.0, 0.0);
    glVertex3f(2.0, 2.5, -1.0);
    glColor3f(0.0, 1.0, 0.0);
    glVertex3f(-3.5, -2.5, -1.0);
    glColor3f(0.0, 0.0, 1.0);
    glVertex3f(2.0, -4.0, 0.0);
    glEnd();
    // draw a polygon
    glBegin(GL_POLYGON);
    glColor3f(1.0, 1.0, 1.0);
    glVertex3f(-1.0, 2.0, 0.0);
    glColor3f(1.0, 1.0, 0.0);
    glVertex3f(-3.0, -0.5, 0.0);
    glColor3f(0.0, 1.0, 1.0);
    glVertex3f(-1.5, -3.0, 0.0);
    glColor3f(0.0, 0.0, 0.0);
    glVertex3f(1.0, -2.0, 0.0);
    glColor3f(1.0, 0.0, 1.0);
    glVertex3f(1.0, 1.0, 0.0);
    glEnd();
    // draw everything and swap the display buffer
    glutSwapBuffers();
} // end Display()


