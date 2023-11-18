#include <cmath>
#include<stdio.h>
#include <windows.h>

#include<gl\glut.h>
#include <GL/freeglut.h>

#include "Object.h"

int g_nGLWidth = 1200, g_nGLHeight = 675;

double theta = 0, phi = 0;
Vec3<double> camPos(15, 1, 0);
Vec3<double> camDirection(0, 0, 0);
Vec3<double> camUp(0, 1, 0);
const double pi = 3.14;

//mouse 변수
int lastPos[3];
int curPos[3];
bool rightDown;
bool firstDown;

Object* cube;
Object* cube2;
void init(void)
{
	glClearColor(0.0f, 0.0f, 0.5f, 1.0f);
	glClearDepth(1.0f);
	glEnable(GL_DEPTH_TEST);

	//화면 좌표 정보 설정
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	cube = new Object("OBJ\\cube.obj",  vec3(1, 0, 0), vec3(1,1,1));
	cube2 = new Object("OBJ\\cube.obj",  vec3(5, 0, 5), vec3(1,0,0));
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

void drawObject(void)
{
	glLoadName(1);
	glPushMatrix();
	glTranslatef(cube->center.x(), cube->center.y(), cube->center.z());
	cube->drawObj();
	//glutSolidSphere(5, 30, 30);
	glPopMatrix();

	glLoadName(2);
	glPushMatrix();
	glTranslatef(cube2->center.x(), cube2->center.y(), cube2->center.z());
	cube2->drawObj();
	//glutSolidSphere(5, 30, 30);
	glPopMatrix();
}

void draw(void)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	gluLookAt(camPos[0], camPos[1], camPos[2], camPos[0] + camDirection[0], camPos[1] + camDirection[1], camPos[2] + camDirection[2], camUp[0], camUp[1], camUp[2]);
	
	drawObject();


	glPopMatrix();
	draw_axis();

	glutPostRedisplay();
	glFlush();
	glutSwapBuffers();
}

void idle(void)
{

}

void picking(int x, int y)
{
	GLuint selectBuf[256];
	glSelectBuffer(256, selectBuf);

	glRenderMode(GL_SELECT);
	glMatrixMode(GL_PROJECTION);

	glInitNames();
	glPushName(-1);

	GLint viewport[4];
	glGetIntegerv(GL_VIEWPORT, viewport);
	glPushMatrix();
	glLoadIdentity();

	gluPickMatrix(x, y, 1, 1, viewport);
	gluPerspective(45, float(g_nGLWidth) / float(g_nGLHeight), 1, 500); // 원근 투영 행렬
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	drawObject();
	glPopMatrix();
	glMatrixMode(GL_PROJECTION);
	glPopMatrix();
	glFlush();

	GLint hits = glRenderMode(GL_RENDER);
	printf("hits: %d\n", hits);

	// fifth step
	if (hits <= 0) return;
	int stack = selectBuf[0];

	for (int i = 0; i < hits * 4; i++)
	{
		if (!selectBuf[i]) break;
		printf("%u ", selectBuf[i]);
	}
	printf("\n");
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
	Vec3<double> cameraV = camDirection.cross(camUp); //카메라 uvn 좌표계 중, v축 방향

	if (key == 'w') //n 방향으로 이동, y축 방향 이동 x
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
	else if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
	{
		y = g_nGLHeight - y;
		picking(x, y);
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

		Vec3<double> front;
		front[0] = cos(phi * pi / 180) * cos(theta * pi / 180);
		front[1] = sin(theta * pi / 180);
		front[2] = cos(theta * pi / 180) * sin(phi * pi / 180);
		front.normalize();

		camDirection = front;
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