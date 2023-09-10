#include<gl\glut.h>
#include<stdio.h>
#include <windows.h>

double spin = 0.0f;
bool spin_state = FALSE;
double horizontal = 0.0f;
double vertical = 0.0f;

void init(void)
{
    /* 화면의 기본색으로 blue 설정 */
    glClearColor(0.0f, 0.0f, 1.0f, 0.0f);

    //화면 좌표 정보 설정
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0.0f, 500.0f, 0.0f, 500.0f);
}

void draw_point(void)
{
    //점의 색상
    glColor3f(1.0f, 0.0f, 0.0f);

    //점의 크기 (초기값은 1.0)
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
    //색 설정
    glColor3f(0.0f, 1.0f, 0.0f);

    glEnable(GL_LINE_STIPPLE);
    glLineStipple(2, 0x00FF);

    //선의 두께
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
    //면의 색상을 노란색으로
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

    //면의 색상을 노란색으로
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

    glTranslatef(horizontal, vertical, 0);

    /*glTranslatef(250, 250, 0);
    glRotatef(spin, 0, 0, 1);
    glTranslatef(-250, -250, 0);*/

    //draw_point();
    //draw_line();
    //draw_triangle();
    draw_polygon();

    glFlush();
    glutSwapBuffers(); //double buffer swap
}


void mouse(int button, int state, int x, int y) {
    /* 인자들을 해석해서 원하는 기능을 구현 */
    printf("Mouse button is clicked! (%d, %d, %d, %d)\n",
        button, state, x, y);
}

void motion(int x, int y)
{
    /* 인자들을 해석해서 원하는 기능을 구현 */
    printf("Mouse is moving! (%d, %d)\n", x, y);
}

void keyboard(unsigned char key, int x, int y)
{
    /* 인자들을 해석해서 원하는 기능을 구현 */
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
    {
        vertical -= 5.0f;
        printf("You pressed DOWN \n");
    }
    else if (key == GLUT_KEY_UP)
    {
        vertical += 5.0f;
        printf("You pressed UP \n");
    }
    else if (key == GLUT_KEY_LEFT)
    {
        horizontal -= 5.0f;
        printf("You pressed LEFT \n");
    }
    else if (key == GLUT_KEY_RIGHT)
    {
        horizontal += 5.0f;
        printf("You pressed RIGHT \n");
    }
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

int main(int argc, char** argv)
{

    /* Window 초기화 */
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    glutInitWindowSize(500, 500);
    glutInitWindowPosition(300, 300);
    glutCreateWindow("My First GL Program");
    init(); // -> 사용자 초기화 함수

    /* Callback 함수 정의 */
    glutDisplayFunc(draw); /* draw() -> 실제 그리기 함수 */
    glutKeyboardFunc(keyboard); //keyboard() -> 키보드 입력 감지 함수
    glutSpecialFunc(specialKeyboard); //방향키 입력 감지 함수
    glutIdleFunc(idle);

    /* Looping 시작 */
    glutMainLoop();
    return 0;
}

//방향키-> speacial keyboard
//회전 flag => 키보드 입력에 따라 toggle