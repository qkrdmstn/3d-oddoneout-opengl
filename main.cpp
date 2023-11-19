#include <cmath>
#include<stdio.h>
#include <windows.h>

#include<gl\glut.h>
#include <GL/freeglut.h>

#include "Object.h"
#include "Player.h"

int g_nGLWidth = 1200, g_nGLHeight = 675;

//camera ����
double theta = 0, phi = 0;
Vec3<double> camPos(15, 1, 0);
Vec3<double> camDirection(0, 0, 0);
Vec3<double> camUp(0, 1, 0);
const double pi = 3.14;

//mouse ����
int lastPos[3];
int curPos[3];
bool rightDown;
bool firstDown;

//object ����
vector<Object*> interObj;
vector<Object*> Obj;
Object* focusedObj;
Player* player;
bool isPicking;

void objectInit(void)
{
	//Interactable Object
	interObj.push_back(new Object("OBJ\\cube.obj", vec3(1, 0, 0), vec3(1, 1, 1)));
	interObj.push_back(new Object("OBJ\\cube.obj", vec3(5, 0, 5), vec3(1, 0, 0)));

	//Uninteractable Object
	Obj.push_back(new Object("OBJ\\cube.obj", vec3(10, 0, 5), vec3(0, 1, 0)));
}

void lightInit(void)
{
	// 0�� ���� ���� ����
	GLfloat light_ambient[] = { 0.2f, 0.2f, 0.2f, 1.0f };
	GLfloat light_diffuse[] = { 1.0f, 1.0f, 1.0f, 1.0f };
	GLfloat light_specular[] = { 1.0f, 1.0f, 1.0f, 1.0f };

	glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
	glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);

	// ���� ����ġ�� 0�� ���� ����ġ �ѱ�
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);

	//���� �ݻ� Ư�� ���� init()�� �߰�
	GLfloat ambientMat[] = { 0.2f, 0.2f, 0.2f, 1.0f };
	GLfloat diffuseMat[] = { 1.0f, 1.0f, 1.0f, 1.0f };
	GLfloat specularMat[] = { 1.0f, 1.0f, 1.0f, 1.0f };

	glMaterialfv(GL_FRONT, GL_AMBIENT, ambientMat);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, diffuseMat);
	glMaterialfv(GL_FRONT, GL_SPECULAR, specularMat);
	glMaterialf(GL_FRONT, GL_SHININESS, 64);

	//color_material
	glEnable(GL_COLOR_MATERIAL);
	glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);
}

void init(void)
{
	glClearColor(0.0f, 0.0f, 0.5f, 1.0f);
	glClearDepth(1.0f);
	glEnable(GL_DEPTH_TEST);

	//ȭ�� ��ǥ ���� ����
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	lightInit();

	player = new Player(0, &camPos, &camDirection);
	objectInit();
	//player = new Player();
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

void drawInterObject()
{
	//draw Interactable Object
	for (int i = 0; i < interObj.size(); i++)
	{
		glLoadName(i);
		glPushMatrix();
		glTranslatef(interObj[i]->center.x(), interObj[i]->center.y(), interObj[i]->center.z());
		interObj[i]->drawObj();
		glPopMatrix();
	}
}

void drawObject(void)
{
	//draw Uninteractable Object
	for (int i = 0; i < Obj.size(); i++)
	{
		glPushMatrix();
		glTranslatef(Obj[i]->center.x(), Obj[i]->center.y(), Obj[i]->center.z());
		Obj[i]->drawObj();
		glPopMatrix();
	}
}

void draw(void)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	//ī�޶� ��ġ ����
	GLfloat light_position[] = { 0.0, 0.0, 0.0, 1.0 };
	glLightfv(GL_LIGHT0, GL_POSITION, light_position);

	gluLookAt(camPos[0], camPos[1], camPos[2], camPos[0] + camDirection[0], camPos[1] + camDirection[1], camPos[2] + camDirection[2], camUp[0], camUp[1], camUp[2]);
	
	//drawObject
	drawInterObject();
	drawObject();
	//player->drawPlayer();

	glPopMatrix();
	draw_axis();

	glutPostRedisplay();
	glFlush();
	glutSwapBuffers();
}

void idle(void)
{

}

void pickingEvent()
{
	if (player->state == 1) //picking ���
	{
		isPicking = true;
	}
	else if (player->state == 2) //color ���
	{
		focusedObj->color.set(player->brush->color);
	}
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

	gluPickMatrix(x, y, 0.1, 0.1, viewport);
	gluPerspective(45, float(g_nGLWidth) / float(g_nGLHeight), 1, 500); // ���� ���� ���
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	drawInterObject();
	glPopMatrix();
	glMatrixMode(GL_PROJECTION);
	glPopMatrix();
	glFlush();

	GLint hits = glRenderMode(GL_RENDER);
	//printf("hits: %d\n", hits);

	// fifth step
	if (hits <= 0) return;

	//for (int i = 0; i < hits * 4; i++)
	//{
	//	//if (!selectBuf[i]) break;
	//	printf("%u ", selectBuf[i]);
	//}
	//printf("\n");

	bool interaction = false;
	double minDist = 10.00f; //��ȣ�ۿ� �ּ� �Ÿ�
	for (int i = 0; i < hits; i++)
	{
		int index = 3 + i * 4;
		int idx = selectBuf[index];
		double dist = (interObj[idx]->center - camPos).length();
		//printf("dist: %f\n", dist);
		if (dist < minDist) //���� ����� object ����
		{
			focusedObj = interObj[idx];
			minDist = dist;
			interaction = true;
		}
	}

	if (focusedObj != NULL && interaction) //focused obj Ȯ�ο�
	{
		pickingEvent();
	}
}


void resize(int width, int height)
{
	glViewport(0, 0, width, height); // ����Ʈ ����
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(45, float(width) / float(height), 1, 500); // ���� ���� ���
	glMatrixMode(GL_MODELVIEW);
}

void keyboard(unsigned char key, int x, int y)
{
	double cameraSpeed = 2.0f;
	Vec3<double> cameraV = camDirection.cross(camUp); //ī�޶� uvn ��ǥ�� ��, v�� ����

	if (key == 'w') //n �������� �̵�, y�� ���� �̵� x
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

	if (key == 'd') //v �������� �̵�
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

	if (key == '1')
	{
		player->state = 1;
		printf("player is Picking Mode\n");
	}	
	else if (key == '2')
	{
		player->state = 2;
		printf("player is Color Mode\n");
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
	else if (button == GLUT_LEFT_BUTTON && state == GLUT_UP)
	{
		isPicking = false;
	}
}

void motion(int x, int y) 
{
	//mouse �����ӿ� ���� ī�޶� ȸ��
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

	if (isPicking)
	{
		GLint viewport[4];
		GLdouble modelview[16];
		GLdouble projection[16];
		GLfloat winX, winY, winZ;
		GLdouble posX, posY, posZ;

		glGetDoublev(GL_MODELVIEW_MATRIX, modelview);
		glGetDoublev(GL_PROJECTION_MATRIX, projection);
		glGetIntegerv(GL_VIEWPORT, viewport);

		winX = (float)x;
		winY = (float)viewport[3] - (float)y;
		glReadPixels(x, int(winY), 1, 1, GL_DEPTH_COMPONENT, GL_FLOAT, &winZ);

		gluUnProject(winX, winY, winZ, modelview, projection, viewport, &posX, &posY, &posZ);
		//printf("x: %f, y: %f, z: %f\n", posX, posY, posZ);

		vec3 clickPos(posX, posY, posZ);
		vec3 normalPos = (clickPos - camPos); //ī�޶�-���콺 Ŭ�� ��ǥ ���⺤��
		normalPos.normalize();

		double dist = (camPos-focusedObj->center).length(); //������ ��, ������ �Ÿ��� ����
		vec3 objPos = camPos + normalPos * dist;

		focusedObj->center.set(objPos);
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
	glutCreateWindow("12211774_������");

	init();
	glutDisplayFunc(draw);
	glutReshapeFunc(resize);
	glutMouseFunc(mouse);
	glutMotionFunc(motion); //���콺 ��ư Ŭ���� ä �̵�
	glutSpecialFunc(specialkeyboard);
	glutKeyboardFunc(keyboard);
	//glutEntryFunc(entry); //window ��Ŀ�� ����

	/* Looping ���� */
	glutMainLoop();
	return 0;
}