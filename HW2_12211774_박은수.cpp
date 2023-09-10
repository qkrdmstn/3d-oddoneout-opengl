#include<gl\glut.h>
#include<stdio.h>
#include <windows.h>

double spin = 0.0f;
bool spin_state = FALSE;
double horizontal = 0.0f;
double vertical = 0.0f;

double xPos = 0.0f;
double yPos = 0.0f;

void init(void)
{
    /* ȭ���� �⺻������ blue ���� */
    glClearColor(0.0f, 0.0f, 1.0f, 0.0f);

    //ȭ�� ��ǥ ���� ����
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0.0f, 500.0f, 0.0f, 500.0f);
}

void draw_point(void)
{
    //���� ����
    glColor3f(1.0f, 0.0f, 0.0f);

    //���� ũ�� (�ʱⰪ�� 1.0)
    glPointSize(4.0f);
    glBegin(GL_POINTS);
    glVertex2i(100, 200);
    glVertex2i(300, 300);
    glVertex2i(400, 200);
    glVertex2i(50, 300);
    glEnd();
}

void draw_line(void)
{
    //�� ����
    glColor3f(0.0f, 1.0f, 0.0f);

    glEnable(GL_LINE_STIPPLE);
    glLineStipple(2, 0x00FF);

    //���� �β�
    glLineWidth(10.0f);
    glBegin(GL_LINES);
    glVertex2i(50, 50);
    glVertex2i(450, 450);
    glVertex2i(50, 450);
    glVertex2i(450, 50);
    glEnd();
}
void draw_triangle(void)
{
    //���� ������ ���������
    glColor3f(1.0f, 1.0f, 0.0f);
    glBegin(GL_POLYGON);
    glVertex2i(100, 10);
    glVertex2i(50, 250);
    glVertex2i(250, 400);
    glVertex2i(450, 250);
    glVertex2i(400, 10);

    glEnd();
}

void draw_polygon(void)
{
    glFrontFace(GL_CW);
    glPolygonMode(GL_FRONT, GL_FILL);
    glCullFace(GL_BACK);
    glEnable(GL_CULL_FACE);

    //���� ������ ���������
    glColor3f(1.0f, 1.0f, 0.0f);
    glBegin(GL_POLYGON);
    glVertex2i(100, 10);
    glVertex2i(50, 250);
    glVertex2i(250, 400);
    glVertex2i(450, 250);
    glVertex2i(400, 10);
    glEnd();
}

void draw(void)
{
    glClear(GL_COLOR_BUFFER_BIT);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    //glTranslatef(horizontal, vertical, 0);
    glTranslatef(xPos, yPos, 0);

    glTranslatef(250, 250, 0);
    glRotatef(spin, 0, 0, 1);
    glTranslatef(-250, -250, 0);

    //draw_point();
    //draw_line();
    //draw_triangle();
    //draw_polygon();

    glFlush();
    glutSwapBuffers();
}

void keyboard(unsigned char key, int x, int y)
{
    /* ���ڵ��� �ؼ��ؼ� ���ϴ� ����� ���� */
    printf("You pressed %c\n", key);
    if (key == 's')
    {
        if (spin_state == TRUE)
            spin_state = FALSE;
        else
            spin_state = TRUE;
    }
}

void specialKeyboard(int key, int x, int y)
{
    if (key == GLUT_KEY_DOWN)
        vertical -= 5.0f;
    else if (key == GLUT_KEY_UP)
        vertical += 5.0f;
    else if (key == GLUT_KEY_LEFT)
        horizontal -= 5.0f;
    else if (key == GLUT_KEY_RIGHT)
        horizontal += 5.0f;

    if (vertical <= 0)
        vertical = 0.0f;
    if (horizontal <= 0)
        horizontal = 0.0f;

    printf("horizontalInput: %f , verticalInput: %f \n", horizontal, vertical);
}

void idle(void) {
    if (spin_state == TRUE)
    {
        spin = spin + 0.1;
        if (spin > 360) spin -= 360;
    }
    else
        spin = spin;
    glutPostRedisplay();
}

void transform_menu_function(int option) //���⿡ ���� �̵��Ÿ� �ݿ�
{
    printf("transformMenu %d has been selected\n", option);
    switch (option)
    {
    case 1: //up
        yPos += vertical;
        break;
    case 2: //down
        yPos -= vertical;
        break;
    case 3: //left
        xPos -= horizontal;
        break;
    case 4: //right
        xPos += horizontal;
        break;
    default:
        break;
    }
}

void main_menu_function(int option)
{
    printf("Main menu %d has been selected\n", option);
    if (option == 999)
        exit(0);
}

int main(int argc, char** argv)
{
    /* Window �ʱ�ȭ */
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    glutInitWindowSize(500, 500);
    glutInitWindowPosition(300, 300);
    glutCreateWindow("My First GL Program");
    init(); // -> ����� �ʱ�ȭ �Լ�

    int transMenu;
    int mainmenu;
    transMenu = glutCreateMenu(transform_menu_function); //subMenu : �̵����� ����
    glutAddMenuEntry("UP", 1);
    glutAddMenuEntry("DOWN", 2);
    glutAddMenuEntry("LEFT", 3);
    glutAddMenuEntry("RIGHT", 4);

    mainmenu = glutCreateMenu(main_menu_function); //mainMenu
    glutAddMenuEntry("Quit", 999);
    glutAddMenuEntry("Go!", 11);
    glutAddSubMenu("Transform", transMenu);
    glutAttachMenu(GLUT_RIGHT_BUTTON); //���콺 ������ ��ư�� ����

    /* Callback �Լ� ���� */
    glutDisplayFunc(draw); /* draw() -> ���� �׸��� �Լ� */
    glutKeyboardFunc(keyboard); //keyboard() -> Ű���� �Է� ���� �Լ�
    glutSpecialFunc(specialKeyboard);
    glutIdleFunc(idle);

    /* Looping ���� */
    glutMainLoop();
    return 0;
}
