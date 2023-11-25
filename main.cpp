#include <cmath>
#include <string>
#include <stdio.h>
#include <windows.h>

#include<gl\glut.h>
#include <GL/freeglut.h>

#include "Object.h"
#include "TexObject.h"
#include "Player.h"

int g_nGLWidth = 1200, g_nGLHeight = 675;

//camera 변수
double theta = 0, phi = 0;
Vec3<double> camPos(0, 1, 10);
Vec3<double> camDirection(0, 0, -10);
Vec3<double> camUp(0, 1, 0);
const double pi = 3.14;

//mouse 변수
int lastPos[3];
int curPos[3];
bool rightDown;
bool firstDown;

//object 변수
vector<Object*> interObj;
vector<Object*> Obj;
Object* focusedObj;
Player* player;

//picking & selection
int curColor = -1;
bool isPicking = false;


void objectInit(void)
{
	//Uninteractable Object
	Obj.push_back(new TexObject("OBJ\\treeN.obj", vec3(0, 2, 0), 0, vec3(1, 1, 1), 0, "OBJ\\tree_UVmap.bmp"));
	//Obj.push_back(new Object("OBJ\\treeN.obj", vec3(5, 0, 5), 0, vec3(0, 1, 0), 0));

	//Interactable Object
	interObj.push_back(new Object("OBJ\\treeN.obj", vec3(0, 2, 0), 0, vec3(1, 1, 1), 2, Obj[0]));
	interObj.push_back(new TexObject("OBJ\\treeN.obj", vec3(5, 2, 0), 0, vec3(1, 1, 1), 2, Obj[0], "OBJ\\tree_UVmap.png"));
//	interObj.push_back(new Object("OBJ\\treeN.obj", vec3(0.5, 3, 0.5), 0, vec3(1, 1, 0), 2, Obj[1]));
}

void lightInit(void)
{
	// 0번 조명 관련 설정
	GLfloat light_ambient[] = { 0.2f, 0.2f, 0.2f, 1.0f };
	GLfloat light_diffuse[] = { 1.0f, 1.0f, 1.0f, 1.0f };
	GLfloat light_specular[] = { 0.1f, 0.1f, 0.1f, 1.0f };

	glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
	glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);

	// 조명 스위치와 0번 조명 스위치 켜기
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);

	//재질 반사 특성 설정 init()에 추가
	GLfloat ambientMat[] = { 0.2f, 0.2f, 0.2f, 1.0f };
	GLfloat diffuseMat[] = { 1.0f, 1.0f, 1.0f, 1.0f };
	GLfloat specularMat[] = { 0.1f, 0.1f, 0.1f, 1.0f };

	glMaterialfv(GL_FRONT, GL_AMBIENT, ambientMat);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, diffuseMat);
	glMaterialfv(GL_FRONT, GL_SPECULAR, specularMat);
	glMaterialf(GL_FRONT, GL_SHININESS, 128);

	////color_material
	glEnable(GL_COLOR_MATERIAL);
	glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);
	//glColorMaterial(GL_FRONT, GL_SPECULAR);
}

void init(void)
{
	glClearColor(0.0f, 0.0f, 0.5f, 1.0f);
	glClearDepth(1.0f);
	glEnable(GL_DEPTH_TEST);

	//화면 좌표 정보 설정
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	lightInit();

	//texture
	glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);	//polygon의 원래 색상은 무시하고 texture로 덮음
	glEnable(GL_TEXTURE_2D);

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
		glTranslatef(interObj[i]->pos.x(), interObj[i]->pos.y(), interObj[i]->pos.z());
		glRotatef(interObj[i]->rot, 0, 1, 0);
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
		glTranslatef(Obj[i]->pos.x(), Obj[i]->pos.y(), Obj[i]->pos.z());
		glRotatef(Obj[i]->rot, 0, 1, 0);
		Obj[i]->drawObj();
		glPopMatrix();
	}
}

void draw_string(void* font, const char* str, float x_position, float y_position, float red, float green, float blue) {
	glPushAttrib(GL_LIGHTING_BIT);
	glDisable(GL_LIGHTING);
	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();
	gluOrtho2D(-10, 10, -10, 10);
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glLoadIdentity();
	glColor3f(red, green, blue);
	glRasterPos3f(x_position, y_position, 0);
	for (unsigned int i = 0; i < strlen(str); i++) {
		glutBitmapCharacter(font, str[i]);
	}
	glPopMatrix();
	glMatrixMode(GL_PROJECTION);
	glPopMatrix();
	glMatrixMode(GL_MODELVIEW);
	glPopAttrib();
}

void drawText() {
	if (player->state == 2 && curColor != -1 )
	{
		string R = "R: " + to_string(player->brush->color.x());
		string G = "G: " + to_string(player->brush->color.y());
		string B = "B: " + to_string(player->brush->color.z());

		char* strR = const_cast<char*>((R).c_str());
		char* strG = const_cast<char*>((G).c_str());
		char* strB = const_cast<char*>((B).c_str());

		draw_string(GLUT_BITMAP_TIMES_ROMAN_24, strR, -9.8, -8.2, player->brush->color.x(), player->brush->color.y(), player->brush->color.z()); //나중에 글자색 바꾸기
		draw_string(GLUT_BITMAP_TIMES_ROMAN_24, strG, -9.8, -8.9, player->brush->color.x(), player->brush->color.y(), player->brush->color.z()); //나중에 글자색 바꾸기
		draw_string(GLUT_BITMAP_TIMES_ROMAN_24, strB, -9.8, -9.6, player->brush->color.x(), player->brush->color.y(), player->brush->color.z()); //나중에 글자색 바꾸기
	}
}

void draw(void)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	//카메라 위치 조명
	GLfloat light_position[] = { 0.0, 10.0, 0.0, 1.0 };
	glLightfv(GL_LIGHT0, GL_POSITION, light_position);
	//glDisable(GL_LIGHT0);
	player->drawPlayer();

	gluLookAt(camPos[0], camPos[1], camPos[2], camPos[0] + camDirection[0], camPos[1] + camDirection[1], camPos[2] + camDirection[2], camUp[0], camUp[1], camUp[2]);
	draw_axis();

	drawText();

	//drawObject
	glPushMatrix();
	glTranslatef(-50, 0, 0); //<보기>맵 local 좌표
	draw_axis();
	drawObject();
	glPopMatrix();
	
	drawInterObject();


	glutPostRedisplay();
	glFlush();
	glutSwapBuffers();
}

void idle(void)
{
	int i = 0;
	for (auto interObj: interObj)
	{
		if (interObj->isCorrect())
		{
			i++;
		}
	}
	if (i == 2)
		printf("clear\n");
}

void pickingEvent()
{
	if (player->state == 1) //picking 모드
	{
		isPicking = true;
	}
	else if (player->state == 2) //color 모드
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
	gluPerspective(45, float(g_nGLWidth) / float(g_nGLHeight), 1, 500); // 원근 투영 행렬
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
	double minDist = 10.00f; //상호작용 최소 거리
	for (int i = 0; i < hits; i++)
	{
		int index = 3 + i * 4;
		int idx = selectBuf[index];
		double dist = (interObj[idx]->pos - camPos).length();
		//printf("dist: %f\n", dist);
		if (dist < minDist) //가장 가까운 object 선별
		{
			focusedObj = interObj[idx];
			minDist = dist;
			interaction = true;
		}
	}

	if (focusedObj != NULL && interaction) //focused obj 확인용
	{
		pickingEvent();
	}
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
		camPos[2] += camDirection[2] * cameraSpeed;
		if (isPicking)
		{
			focusedObj->pos[0] += camDirection[0] * cameraSpeed;
			focusedObj->pos[2] += camDirection[2] * cameraSpeed;
		}
	}
	else if (key == 's')
	{
		camPos[0] -= camDirection[0] * cameraSpeed;
		camPos[2] -= camDirection[2] * cameraSpeed;
		if (isPicking)
		{
			focusedObj->pos[0] -= camDirection[0] * cameraSpeed;
			focusedObj->pos[2] -= camDirection[2] * cameraSpeed;
		}
	}

	if (key == 'd') //v 방향으로 이동
	{
		camPos[0] += cameraV[0] * cameraSpeed;
		camPos[2] += cameraV[2] * cameraSpeed;
		if (isPicking)
		{
			focusedObj->pos[0] += cameraV[0] * cameraSpeed;
			focusedObj->pos[2] += cameraV[2] * cameraSpeed;
		}
	}
	else if (key == 'a') 
	{
		camPos[0] -= cameraV[0] * cameraSpeed;
		camPos[2] -= cameraV[2] * cameraSpeed;
		if (isPicking)
		{
			focusedObj->pos[0] -= cameraV[0] * cameraSpeed;
			focusedObj->pos[2] -= cameraV[2] * cameraSpeed;
		}
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
		vec3 normalPos = (clickPos - camPos); //카메라-마우스 클릭 좌표 방향벡터
		normalPos.normalize();

		double dist = (camPos-focusedObj->pos).length(); //움직일 때, 원래의 거리를 유지
		vec3 objPos = camPos + normalPos * dist;

		focusedObj->pos.set(objPos);
	}
}

void colorModeWheelInput(int direction)
{
	if (curColor == 0)
	{
		if (direction > 0)
		{
			player->brush->color[0] += 0.1;
			if (player->brush->color[0] > 1)
				player->brush->color[0] = 1;
		}
		else
		{
			player->brush->color[0] -= 0.1;
			if (player->brush->color[0] < 0)
				player->brush->color[0] = 0;
		}
	}
	else if (curColor == 1)
	{
		if (direction > 0)
		{
			player->brush->color[1] += 0.1;
			if (player->brush->color[1] > 1)
				player->brush->color[1] = 1;
		}
		else
		{
			player->brush->color[1] -= 0.1;
			if (player->brush->color[1] < 0)
				player->brush->color[1] = 0;
		}
	}
	else if (curColor == 2)
	{
		if (direction > 0)
		{
			player->brush->color[2] += 0.1;
			if (player->brush->color[2] > 1)
				player->brush->color[2] = 1;
		}
		else
		{
			player->brush->color[2] -= 0.1;
			if (player->brush->color[2] < 0)
				player->brush->color[2] = 0;
		}
	}
}

void pickModeWheelInput(int direction)
{
	if (focusedObj != NULL)
	{
		 if (direction > 0)
		{
			focusedObj->rot -= 10;
			if (focusedObj->rot < 0)
				focusedObj->rot += 360;
		}
		else
		{
			focusedObj->rot += 10;
			if (focusedObj->rot > 360)
				focusedObj->rot -= 360;
		}

	}

}

void specialkeyboard(int key, int x, int y)
{

}

void mouse_wheel(int wheel, int direction, int x, int y) {

	if (player->state == 1)
	{
		pickModeWheelInput(direction);
	}
	else if (player->state == 2)
	{
		colorModeWheelInput(direction);
	}

}

void entry(int state)
{
	//printf("state: %d\n", state);
	//if (state == GLUT_ENTERED)
	//	firstDown = true;
}

void color_menu_function(int option) //기본색 변경 & draw 함수 호출해서 다시 기본색으로 그리기 (크기 0.5f 고정)
{
	switch (option)
	{
	case 1:
		curColor = 0;
		break;
	case 2:
		curColor = 1;
		break;
	case 3:
		curColor = 2;
		break;
	case 4:
		curColor = -1;
		break;
	default:
		break;
	}
}

void main_menu_function(int option)
{
	if (option == 1 && focusedObj != NULL)
	{
		focusedObj->rot = 0;
	}
}

void menuSetting()
{
	int colorMenu, mainMenu;
	colorMenu = glutCreateMenu(color_menu_function); //subMenu : 배경 기본색 변경
	glutAddMenuEntry("Red", 1);
	glutAddMenuEntry("Green", 2);
	glutAddMenuEntry("Blue", 3);
	glutAddMenuEntry("Close", 4);

	mainMenu = glutCreateMenu(main_menu_function); //mainMenu : Quit, Go, Size(sub), Color(sub)
	glutAddMenuEntry("Init Rotation", 1);
	glutAddSubMenu("ColorSelect", colorMenu);
	glutAttachMenu(GLUT_MIDDLE_BUTTON); //마우스 오른쪽 버튼에 연결
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
	glutMouseWheelFunc(mouse_wheel);
	glutMotionFunc(motion); //마우스 버튼 클릭한 채 이동
	glutSpecialFunc(specialkeyboard);
	glutKeyboardFunc(keyboard);
	glutIdleFunc(idle);
	//glutEntryFunc(entry); //window 포커스 감지

	menuSetting();

	/* Looping 시작 */
	glutMainLoop();
	return 0;
}