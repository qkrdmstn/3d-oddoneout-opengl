#include<stdio.h>
#include <windows.h>
#include <cmath>
#include<gl\glut.h>
#include <GL/freeglut.h>

#include "Object.h"
#include "Vec3.h"

int g_nGLWidth = 1200, g_nGLHeight = 675;

double theta = 0, phi = 0;
double camPos[3] = { 15,1,0 };
double camDirection[3] = { 0,0,0 };
double camUp[3] = { 0,1,0 };
const double pi = 3.14;

//mouse 변수
int lastPos[3];
int curPos[3];
bool rightDown;
bool firstDown;

Object* cube;
void init(void)
{
	glClearColor(0.0f, 0.0f, 0.5f, 1.0f);
	glClearDepth(1.0f);
	glEnable(GL_DEPTH_TEST);

	//화면 좌표 정보 설정
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	cube = new Object("OBJ\\cube.obj",  vec3(1, 1, 0));
}

void draw_axis()
{
	glLineWidth(3);
	glBegin(GL_LINES);
	glColor3f(1, 0, 0);
	glVertex3f(0, 0, 0);
	glVertex3f(400, 0, 0);

	glColor3f(0, 1, 0);
	glVertex3f(0, 0, 0);
	glVertex3f(0, 400, 0);

	glColor3f(0, 0, 1);
	glVertex3f(0, 0, 0);
	glVertex3f(0, 0, 400);
	glEnd();
	glLineWidth(1);
}

void draw(void)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	gluLookAt(camPos[0], camPos[1], camPos[2], camPos[0] + camDirection[0], camPos[1] + camDirection[1], camPos[2] + camDirection[2], camUp[0], camUp[1], camUp[2]);
	
	glPushMatrix(); //world 좌표 push

	glTranslatef(cube->center.x(), cube->center.y(), cube->center.z());
	cube->drawObj();

	glPopMatrix();
	draw_axis();

	glutPostRedisplay();
	glFlush();
	glutSwapBuffers();
}

void idle(void)
{

}

void resize(int width, int height)
{
	glViewport(0, 0, width, height); // 뷰포트 설정
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(45, float(width) / float(height), 1, 500); // 원근 투영 행렬
	glMatrixMode(GL_MODELVIEW);
}

void keyboard(unsigned char key, int x, int y)
{
	double cameraSpeed = 2.0f;
	double cameraV[3] = { camDirection[1] * camUp[2] - camUp[1] * camDirection[2], //카메라 uvn 좌표계: v 방향
						  camDirection[2] * camUp[0] - camUp[2] * camDirection[0], 
						  camDirection[0] * camUp[1] - camUp[0] * camDirection[1] };

	if (key == 'w') //n 방향으로 이동
	{
		camPos[0] += camDirection[0] * cameraSpeed;
		//camPos[1] += camDirection[1] * cameraSpeed;
		camPos[2] += camDirection[2] * cameraSpeed;
	}
	else if (key == 's')
	{
		camPos[0] -= camDirection[0] * cameraSpeed;
		//camPos[1] -= camDirection[1] * cameraSpeed;
		camPos[2] -= camDirection[2] * cameraSpeed;
	}

	if (key == 'd') //v 방향으로 이동
	{
		camPos[0] += cameraV[0] * cameraSpeed;
		//camPos[1] += cameraH[1] * cameraSpeed;
		camPos[2] += cameraV[2] * cameraSpeed;
	}
	else if (key == 'a') 
	{
		camPos[0] -= cameraV[0] * cameraSpeed;
		//camPos[1] -= cameraH[1] * cameraSpeed;
		camPos[2] -= cameraV[2] * cameraSpeed;
	}
}

void mouse(int button, int state, int x, int y) 
{
	if (button == GLUT_RIGHT_BUTTON && state == GLUT_DOWN)
	{
		rightDown = true;
		firstDown = true;
	}
	else if (button == GLUT_RIGHT_BUTTON && state == GLUT_UP)
	{
		rightDown = false;
	}
}

void motion(int x, int y) 
{
	//mouse 움직임에 따른 카메라 회전
	if (rightDown) 
	{
		if (firstDown)
		{
			lastPos[0] = x;
			lastPos[1] = y;
			firstDown = false;
		}
		else
		{
			lastPos[0] = curPos[0];
			lastPos[1] = curPos[1];
		}

		curPos[0] = x;
		curPos[1] = y;

		double deltaX = (curPos[0] - lastPos[0]) / (double)g_nGLWidth;
		double deltaY = (lastPos[1] - curPos[1]) / (double)g_nGLWidth;

		phi += (180 * deltaX);
		theta += (180 * deltaY);

		if (theta > 90)
			theta = 90;
		else if (theta < -90)
			theta = -90;

		double front[3];
		front[0] = cos(phi * pi / 180) * cos(theta * pi / 180);
		front[1] = sin(theta * pi / 180);
		front[2] = cos(theta * pi / 180) * sin(phi * pi / 180);

		double norm = sqrt(pow(front[0], 2) + pow(front[1], 2) + pow(front[2], 2)); //normalize
		camDirection[0] = front[0] / norm;
		camDirection[1] = front[1] / norm;
		camDirection[2] = front[2] / norm;
	}
}

void specialkeyboard(int key, int x, int y)
{

}

void entry(int state)
{
	//printf("state: %d\n", state);
	//if (state == GLUT_ENTERED)
	//	firstDown = true;
}

int main(int argc, char ** argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowSize(g_nGLWidth, g_nGLHeight);
	glutInitWindowPosition(300, 300);
	glutCreateWindow("12211774_박은수");

	init();
	glutDisplayFunc(draw);
	glutReshapeFunc(resize);
	glutMouseFunc(mouse);
	glutMotionFunc(motion); //마우스 버튼 클릭한 채 이동
	glutSpecialFunc(specialkeyboard);
	glutKeyboardFunc(keyboard);
	//glutEntryFunc(entry); //window 포커스 감지

	/* Looping 시작 */
	glutMainLoop();
	return 0;
}