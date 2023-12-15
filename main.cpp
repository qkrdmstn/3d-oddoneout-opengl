#include <cmath>
#include <string>
#include <stdio.h>
#include <windows.h>

#include<GL/glut.h>
#include <GL/freeglut.h>
#include "GL/glext.h"

//sound 관련 헤더
#include <mmsystem.h>
#pragma comment(lib, "winmm.lib")

#include "Object.h"
#include "TexObject.h"
#include "Player.h"
#include "GameManager.h"

int g_nGLWidth = 1200, g_nGLHeight = 675;
const double pi = 3.14;

//Stage camera 변수
double theta = 0, phi = 0;
Vec3<double> camPos(-2.53, 1.5, 13.09);
Vec3<double> camDirection(0, 0, -1);
Vec3<double> camUp(0, 1, 0);

//Hint camera 변수
double alpha = 0, beta = 90, radius = 8.0;
Vec3<double> cam(3.875, 1, 15);
Vec3<double> center(3.875, 1.5, 5.375);
Vec3<double> up(0, 1, 0);

//mouse 변수
int lastPos[3];
int curPos[3];
bool rightDown;
bool firstDown;

//object 변수
vector<Object*> interObj;
vector<Object*> Obj;
Object* field;

Object* focusedObj;
Player* player;
GameManager* gm;

//picking & selection
int curColor = -1;
bool isPicking = false;

double startT;

double lightTimer;
bool lightFlag = true;

void objectInit(void)
{
	field = new TexObject("OBJ\\field.obj", vec3(0,0,0), 0, vec3(1,1,1), 0, "Texture\\field_UVmap.bmp");
	
	//Uninteractable Object
	//tree
	Obj.push_back(new TexObject("OBJ\\tree.obj", vec3(3.875, 1.5, 5.375), 0, vec3(1, 1, 1), 0, "Texture\\tree_UVmap.bmp"));
	Obj.push_back(new Object("OBJ\\star.obj", vec3(3.885, 4.08, 5.39), 0, vec3(1, 1, 0), 0));

	Obj.push_back(new Object("OBJ\\ball.obj", vec3(3.605, 3.2, 5.455), 0, vec3(1, 1, 0), 0));
	Obj.push_back(new Object("OBJ\\ball.obj", vec3(4.355, 2.7, 5.705), 0, vec3(1, 0.2, 0.2), 0));
	Obj.push_back(new Object("OBJ\\ball.obj", vec3(3.885, 2.3, 4.730), 0, vec3(0, 0.5, 0.2), 0));
	Obj.push_back(new Object("OBJ\\ball.obj", vec3(3.155, 2.0, 5.455), 0, vec3(0.2, 0.2, 1), 0));
	Obj.push_back(new Object("OBJ\\ball.obj", vec3(3.885, 1.7, 6.255), 0, vec3(0.5, 0.5, 1), 0));
	Obj.push_back(new Object("OBJ\\ball.obj", vec3(4.885, 1.4, 5.255), 0, vec3(1, 1, 1), 0));
	Obj.push_back(new Object("OBJ\\ball.obj", vec3(3.885, 1.2, 4.255), 0, vec3(1, 0.3, 0.1), 0));
	Obj.push_back(new Object("OBJ\\ball.obj", vec3(2.785, 0.85, 5.655), 0, vec3(0.4, 0, 0.8), 0));

	//box
	Obj.push_back(new TexObject("OBJ\\Box3.obj", vec3(2.275, 0.2, 5.5), 25, vec3(1, 1, 1), 0, "Texture\\BoxRY_UVmap.bmp"));
	Obj.push_back(new TexObject("OBJ\\Box2.obj", vec3(2.575, 0.2, 6.2), 0, vec3(1, 1, 1), 0, "Texture\\BoxYR_UVmap.bmp"));
	Obj.push_back(new TexObject("OBJ\\Box5.obj", vec3(3.075, 0.2, 6.9), 45, vec3(1, 1, 1), 0, "Texture\\BoxGR_UVmap.bmp"));
	Obj.push_back(new TexObject("OBJ\\Box6.obj", vec3(4.675, 0.4, 6.9), 100, vec3(1, 1, 1), 0, "Texture\\BoxRG_UVmap.bmp"));
	Obj.push_back(new TexObject("OBJ\\Box1.obj", vec3(5.175, 0.2, 6.3), 20, vec3(1, 1, 1), 0, "Texture\\BoxRY_UVmap.bmp"));
	Obj.push_back(new TexObject("OBJ\\Box4.obj", vec3(5.5, 0.2, 5.5), 60, vec3(1, 1, 1), 0, "Texture\\BoxRY_UVmap.bmp"));
	
	//snowman
	Obj.push_back(new TexObject("OBJ\\snowB.obj", vec3(6.5, 0.2, 2.3), 0, vec3(1, 1, 1), 0, "Texture\\snowB_UVmap.bmp"));
	Obj.push_back(new TexObject("OBJ\\snowM.obj", vec3(6.5, 0.8, 2.3), 0, vec3(1, 1, 1), 0, "Texture\\snowM_UVmap.bmp"));
	Obj.push_back(new TexObject("OBJ\\snowT.obj", vec3(6.5, 1.3, 2.3), 0, vec3(1, 1, 1), 0, "Texture\\snowT_UVmap.bmp"));
	Obj.push_back(new Object("OBJ\\snowHat.obj", vec3(6.5, 1.8, 2.3), 0, vec3(0.5, 0.5, 0.5), 0));

	//etc
	Obj.push_back(new TexObject("OBJ\\sock.obj", vec3(7.55, 1.4, 3.25), 0, vec3(1, 1, 1), 0, "Texture\\sock_UVmap1.bmp"));
	Obj.push_back(new TexObject("OBJ\\sock.obj", vec3(7.55, 1.25, 4.0), 0, vec3(1, 1, 1), 0, "Texture\\sock_UVmap2.bmp"));
	Obj.push_back(new TexObject("OBJ\\sock.obj", vec3(7.55, 1.15, 4.775), 0, vec3(1, 1, 1), 0, "Texture\\sock_UVmap3.bmp"));
	Obj.push_back(new TexObject("OBJ\\sock.obj", vec3(7.55, 1.25, 5.550), 0, vec3(1, 1, 1), 0, "Texture\\sock_UVmap4.bmp"));
	Obj.push_back(new TexObject("OBJ\\sock.obj", vec3(7.55, 1.45, 6.35), 0, vec3(1, 1, 1), 0, "Texture\\sock_UVmap5.bmp"));
	Obj.push_back(new TexObject("OBJ\\santaHat.obj", vec3(3.875, 1.6, 0.65), 70, vec3(1, 1, 1), 0, "Texture\\santaHat_UVmap.bmp"));
	Obj.push_back(new TexObject("OBJ\\candy.obj", vec3(4.45, 1.4, 6.275), 10, vec3(1, 1, 1), 0, "Texture\\candy_UVmap.bmp"));

	//Interactable Object
	//tree
	interObj.push_back(new TexObject("OBJ\\tree.obj", vec3(3.875, 1.5, 5.375), 0, vec3(1, 1, 1), 0, Obj[0], "Texture\\tree_UVmap.bmp"));
	interObj.push_back(new Object("OBJ\\star.obj", vec3(3.81, 1.85, 0.63), 0, vec3(1, 0, 0), 3, Obj[1]));

	interObj.push_back(new Object("OBJ\\ball.obj", vec3(3.605, 3.2, 5.455), 0, vec3(1, 0.3, 0.1), 3, Obj[2]));
	interObj.push_back(new Object("OBJ\\ball.obj", vec3(4.355, 2.7, 5.705), 0, vec3(1, 0.2, 0.2), 3, Obj[3]));
	interObj.push_back(new Object("OBJ\\ball.obj", vec3(3.885, 2.3, 4.730), 0, vec3(1, 1, 1), 3, Obj[4]));
	interObj.push_back(new Object("OBJ\\ball.obj", vec3(3.155, 2.0, 5.455), 0, vec3(0.2, 0.2, 1), 3, Obj[5]));
	interObj.push_back(new Object("OBJ\\ball.obj", vec3(3.885, 1.7, 6.255), 0, vec3(0.5, 0.5, 1), 3, Obj[6]));
	interObj.push_back(new Object("OBJ\\ball.obj", vec3(4.885, 1.4, 5.255), 0, vec3(0.2, 0.0, 0.8), 3, Obj[7]));
	interObj.push_back(new Object("OBJ\\ball.obj", vec3(3.885, 1.2, 4.255), 0, vec3(0.2, 1, 0), 3, Obj[8]));
	interObj.push_back(new Object("OBJ\\ball.obj", vec3(2.785, 0.85, 5.655), 0, vec3(0.1, 0.6, 1.0), 3, Obj[9]));

	//box
	interObj.push_back(new TexObject("OBJ\\Box3.obj", vec3(2.275, 0.2, 5.5), 0, vec3(1, 1, 1), 3, Obj[10], "Texture\\BoxRY_UVmap.bmp"));
	interObj.push_back(new TexObject("OBJ\\Box2.obj", vec3(4.84, 0.2, 6.86), 0, vec3(1, 1, 1), 3, Obj[11], "Texture\\BoxYR_UVmap.bmp"));
	interObj.push_back(new TexObject("OBJ\\Box5.obj", vec3(5.671705, 0.2, 5.016742), 5, vec3(1, 1, 1), 3, Obj[12], "Texture\\BoxGR_UVmap.bmp"));
	interObj.push_back(new TexObject("OBJ\\Box6.obj", vec3(2.70, 0.4, 6.36), 90, vec3(1, 1, 1), 3, Obj[13], "Texture\\BoxRG_UVmap.bmp"));
	interObj.push_back(new TexObject("OBJ\\Box1.obj", vec3(5.275, 0.2, 6.2), 20, vec3(1, 1, 1), 3, Obj[14], "Texture\\BoxRY_UVmap.bmp"));
	interObj.push_back(new TexObject("OBJ\\Box4.obj", vec3(3.45, 0.2, 6.85), 180, vec3(1, 1, 1), 3, Obj[15], "Texture\\BoxRY_UVmap.bmp"));

	//snowman
	interObj.push_back(new TexObject("OBJ\\snowB.obj", vec3(0.7, 0.3, 2.3), 90, vec3(1, 1, 1), 3, Obj[16], "Texture\\snowB_UVmap.bmp"));
	interObj.push_back(new TexObject("OBJ\\snowM.obj", vec3(6.5, 0.2, 2.3), 0, vec3(1, 1, 1), 3, Obj[17], "Texture\\snowM_UVmap.bmp"));
	interObj.push_back(new TexObject("OBJ\\snowT.obj", vec3(6.5, 0.7, 2.3), 0, vec3(1, 1, 1), 3, Obj[18], "Texture\\snowT_UVmap.bmp"));
	interObj.push_back(new Object("OBJ\\snowHat.obj", vec3(0.7, 1.0, 2.3), 0, vec3(0.5, 0.5, 0.5), 3, Obj[19]));

	//etc
	interObj.push_back(new TexObject("OBJ\\sock.obj", vec3(3.18, 1.701441, 4.88), 160, vec3(1, 1, 1), 3, Obj[20], "Texture\\sock_UVmap1.bmp"));
	interObj.push_back(new TexObject("OBJ\\sock.obj", vec3(7.55, 1.4, 3.25), 0, vec3(1, 1, 1), 3, Obj[21], "Texture\\sock_UVmap2.bmp"));
	interObj.push_back(new TexObject("OBJ\\sock.obj", vec3(7.55, 1.25, 5.550), 0, vec3(1, 1, 1), 3, Obj[22], "Texture\\sock_UVmap3.bmp"));
	interObj.push_back(new TexObject("OBJ\\sock.obj", vec3(7.55, 1.15, 4.775), 0, vec3(1, 1, 1), 3, Obj[23], "Texture\\sock_UVmap4.bmp"));
	interObj.push_back(new TexObject("OBJ\\sock.obj", vec3(4.98, 1.10, 5.84), 0, vec3(1, 1, 1), 3, Obj[24], "Texture\\sock_UVmap5.bmp"));
	interObj.push_back(new TexObject("OBJ\\santaHat.obj", vec3(6.483, 1.045, 2.2), 0, vec3(1, 1, 1), 3, Obj[25], "Texture\\santaHat_UVmap.bmp"));
	interObj.push_back(new TexObject("OBJ\\candy.obj", vec3(4.45, 1.4, 6.275), 10, vec3(1, 1, 1), 3, Obj[26], "Texture\\candy_UVmap.bmp"));
}

void lightInit(void)
{
	// 0번 조명 관련 설정
	GLfloat light_ambient0[] = { 0.5f, 0.5f, 0.5f, 1.0f };
	GLfloat light_diffuse0[] = { 1.0f, 1.0f, 1.0f, 1.0f };
	GLfloat light_specular0[] = { 0.7f, 0.7f, 0.7f, 1.0f };

	glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient0);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse0);
	glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular0);

	//감쇠 속성
	glLightf(GL_LIGHT0, GL_CONSTANT_ATTENUATION, 1.0);
	glLightf(GL_LIGHT0, GL_LINEAR_ATTENUATION, 1.0);
	glLightf(GL_LIGHT0, GL_QUADRATIC_ATTENUATION, 0.0);

	// 1번 조명 관련 설정 <보기> 맵 벽난로
	GLfloat light_ambient1[] = { 0.7f, 0.7f, 0.7f, 1.0f };
	GLfloat light_diffuse1[] = { 0.7f, 0.2f, 0.0f, 1.0f };
	GLfloat light_specular1[] = { 0.7f, 0.7f, 0.7f, 1.0f };

	glLightfv(GL_LIGHT1, GL_AMBIENT, light_ambient1);
	glLightfv(GL_LIGHT1, GL_DIFFUSE, light_diffuse1);
	glLightfv(GL_LIGHT1, GL_SPECULAR, light_specular1);

	//감쇠 속성
	glLightf(GL_LIGHT1, GL_CONSTANT_ATTENUATION, 1.0);
	glLightf(GL_LIGHT1, GL_LINEAR_ATTENUATION, 0.4);
	glLightf(GL_LIGHT1, GL_QUADRATIC_ATTENUATION, 0.5);

	// 2번 조명 관련 설정 <상호작용> 맵 벽난로
	GLfloat light_ambient2[] = { 0.7f, 0.7f, 0.7f, 1.0f };
	GLfloat light_diffuse2[] = { 0.7f, 0.2f, 0.0f, 1.0f };
	GLfloat light_specular2[] = { 0.7f, 0.7f, 0.7f, 1.0f };

	glLightfv(GL_LIGHT2, GL_AMBIENT, light_ambient2);
	glLightfv(GL_LIGHT2, GL_DIFFUSE, light_diffuse2);
	glLightfv(GL_LIGHT2, GL_SPECULAR, light_specular2);

	//감쇠 속성
	glLightf(GL_LIGHT2, GL_CONSTANT_ATTENUATION, 1.0);
	glLightf(GL_LIGHT2, GL_LINEAR_ATTENUATION, 0.4);
	glLightf(GL_LIGHT2, GL_QUADRATIC_ATTENUATION, 0.5);

	// 3번 조명 관련 설정 <보기> 맵 별 조명
	GLfloat light_ambient3[] = { 0.7f, 0.7f, 0.7f, 1.0f };
	GLfloat light_diffuse3[] = { 1.0f, 1.0f, 0.0f, 1.0f };
	GLfloat light_specular3[] = { 0.7f, 0.7f, 0.7f, 1.0f };

	glLightfv(GL_LIGHT3, GL_AMBIENT, light_ambient3);
	glLightfv(GL_LIGHT3, GL_DIFFUSE, light_diffuse3);
	glLightfv(GL_LIGHT3, GL_SPECULAR, light_specular3);

	//감쇠 속성
	glLightf(GL_LIGHT3, GL_CONSTANT_ATTENUATION, 1.0);
	glLightf(GL_LIGHT3, GL_LINEAR_ATTENUATION, 0.4);
	glLightf(GL_LIGHT3, GL_QUADRATIC_ATTENUATION, 0.5);

	// 4번 조명 관련 설정 <상호작용> 맵 별 조명
	GLfloat light_ambient4[] = { 0.7f, 0.7f, 0.7f, 1.0f };
	GLfloat light_diffuse4[] = { 1.0f, 1.0f, 0.0f, 1.0f };
	GLfloat light_specular4[] = { 0.7f, 0.7f, 0.7f, 1.0f };

	glLightfv(GL_LIGHT4, GL_AMBIENT, light_ambient4);
	glLightfv(GL_LIGHT4, GL_DIFFUSE, light_diffuse4);
	glLightfv(GL_LIGHT4, GL_SPECULAR, light_specular4);

	//감쇠 속성
	glLightf(GL_LIGHT4, GL_CONSTANT_ATTENUATION, 1.0);
	glLightf(GL_LIGHT4, GL_LINEAR_ATTENUATION, 0.4);
	glLightf(GL_LIGHT4, GL_QUADRATIC_ATTENUATION, 0.5);

	// 5번 조명 관련 설정 <보기> 맵 장식 조명
	GLfloat light_ambient5[] = { 0.7f, 0.7f, 0.7f, 1.0f };
	GLfloat light_diffuse5[] = { 1.0f, 1.0f, 0.0f, 1.0f };
	GLfloat light_specular5[] = { 0.7f, 0.7f, 0.7f, 1.0f };

	glLightfv(GL_LIGHT5, GL_AMBIENT, light_ambient5);
	glLightfv(GL_LIGHT5, GL_DIFFUSE, light_diffuse5);
	glLightfv(GL_LIGHT5, GL_SPECULAR, light_specular5);

	//감쇠 속성
	glLightf(GL_LIGHT5, GL_CONSTANT_ATTENUATION, 1.0);
	glLightf(GL_LIGHT5, GL_LINEAR_ATTENUATION, 0.4);
	glLightf(GL_LIGHT5, GL_QUADRATIC_ATTENUATION, 0.5);

	// 6번 조명 관련 설정 <상호작용> 맵 장식 조명
	GLfloat light_ambient6[] = { 0.7f, 0.7f, 0.7f, 1.0f };
	GLfloat light_diffuse6[] = { 1.0f, 1.0f, 0.0f, 1.0f };
	GLfloat light_specular6[] = { 0.7f, 0.7f, 0.7f, 1.0f };

	glLightfv(GL_LIGHT6, GL_AMBIENT, light_ambient6);
	glLightfv(GL_LIGHT6, GL_DIFFUSE, light_diffuse6);
	glLightfv(GL_LIGHT6, GL_SPECULAR, light_specular6);

	//감쇠 속성
	glLightf(GL_LIGHT6, GL_CONSTANT_ATTENUATION, 1.0);
	glLightf(GL_LIGHT6, GL_LINEAR_ATTENUATION, 0.4);
	glLightf(GL_LIGHT6, GL_QUADRATIC_ATTENUATION, 0.5);

	// 조명 스위치와 0번 조명 스위치 켜기
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glEnable(GL_LIGHT1);
	glEnable(GL_LIGHT2);
	glEnable(GL_LIGHT3);
	glEnable(GL_LIGHT4);
	glEnable(GL_LIGHT5);
	glEnable(GL_LIGHT6);

	glShadeModel(GL_SMOOTH);

	//재질 반사 특성 설정 init()에 추가
	GLfloat ambientMat[] = { 0.5f, 0.5f, 0.5f, 1.0f };
	GLfloat diffuseMat[] = { 1.0f, 1.0f, 1.0f, 1.0f };
	GLfloat specularMat[] = { 0.7f, 0.7f, 0.7f, 1.0f };

	glMaterialfv(GL_FRONT, GL_AMBIENT, ambientMat);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, diffuseMat);
	glMaterialfv(GL_FRONT, GL_SPECULAR, specularMat);
	glMaterialf(GL_FRONT, GL_SHININESS, 64);

	////color_material
	glEnable(GL_COLOR_MATERIAL);
	glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);
}

void init(void)
{
	glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
	glClearDepth(1.0f);
	glEnable(GL_DEPTH_TEST);

	//화면 좌표 정보 설정
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	lightInit();

	player = new Player(1, &camPos, &camDirection);
	gm = new GameManager();
	objectInit();
	gm->cubeTexture();
	PlaySound(TEXT("sound\\BGM.wav"), NULL, SND_ASYNC | SND_ALIAS | SND_LOOP);
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
		//draw_axis();
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
		//draw_axis();
		Obj[i]->drawObj();
		glPopMatrix();
	}
}

void draw_string(void* font, const char* str, float x_position, float y_position, float r, float g, float b) {
	glPushAttrib(GL_LIGHTING_BIT);
	glDisable(GL_LIGHTING);
	glMatrixMode(GL_PROJECTION);

	glPushMatrix();
	glLoadIdentity();
	gluOrtho2D(-10, 10, -10, 10);
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glLoadIdentity();

	glColor3f(r, g, b);
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
	if (gm->curState == 0)
	{
		string tutorial = "Press any key to start the game";
		char* Ctutorial = const_cast<char*>((tutorial).c_str());
		draw_string(GLUT_BITMAP_TIMES_ROMAN_24, Ctutorial, -2.5, -2, 0,0,0);
	}
	else if (gm->curState == 1)
	{
		if (player->state == 2 && curColor != -1)
		{
			string R = "R: " + to_string(player->brush->color.x());
			string G = "G: " + to_string(player->brush->color.y());
			string B = "B: " + to_string(player->brush->color.z());

			char* strR = const_cast<char*>((R).c_str());
			char* strG = const_cast<char*>((G).c_str());
			char* strB = const_cast<char*>((B).c_str());
			vec3 color(player->brush->color.x(), player->brush->color.y(), player->brush->color.z());
			draw_string(GLUT_BITMAP_TIMES_ROMAN_24, strR, -9.8, -8.2, color.x(), color.y(), color.z());
			draw_string(GLUT_BITMAP_TIMES_ROMAN_24, strG, -9.8, -8.9, color.x(), color.y(), color.z());
			draw_string(GLUT_BITMAP_TIMES_ROMAN_24, strB, -9.8, -9.6, color.x(), color.y(), color.z());
		}

		string diff = "Differences: " + to_string(gm->differenece);
		char* strDiff = const_cast<char*>((diff).c_str());
		draw_string(GLUT_BITMAP_TIMES_ROMAN_24, strDiff, -9.8, 9.4, 1, 1, 1);

		int intTime = gm->timer; //내림
		int floatTime = (gm->timer - intTime) * 100; //소수점 아래 둘째자리
		string time = "Timer: " + to_string(intTime) + "." + to_string(floatTime);
		char* strTime = const_cast<char*>((time).c_str());
		if (gm->timer < 30)
		{
			double delta = gm->timer / 30.0f;
			draw_string(GLUT_BITMAP_TIMES_ROMAN_24, strTime, -1, 9.4, 1, 1 * delta, 1 * delta);

		}
		else
			draw_string(GLUT_BITMAP_TIMES_ROMAN_24, strTime, -1, 9.4, 1, 1, 1);
	}
	else if (gm->curState == 2)
	{
		string Clear = "Game Clear!!!";
		string Restart = "Press any key to Restart the game";
		char* strClear = const_cast<char*>((Clear).c_str());
		char* strRestart = const_cast<char*>((Restart).c_str());
		draw_string(GLUT_BITMAP_TIMES_ROMAN_24, strClear, -1.0, -1.2, 0, 0, 0);
		draw_string(GLUT_BITMAP_TIMES_ROMAN_24, strRestart, -2.5, -2, 0, 0, 0);
	}
	else if (gm->curState == 3)
	{
		string Over = "Game Over!!!";
		string Restart = "Press any key to Restart the game";
		char* strOver = const_cast<char*>((Over).c_str());
		char* strRestart = const_cast<char*>((Restart).c_str());
		draw_string(GLUT_BITMAP_TIMES_ROMAN_24, strOver, -1.0, -1.2, 0, 0, 0);
		draw_string(GLUT_BITMAP_TIMES_ROMAN_24, strRestart, -2.5, -2, 0, 0, 0);
	}
	else if (gm->curState == 4)
	{
		string diff = "Differences: " + to_string(gm->differenece);
		char* strDiff = const_cast<char*>((diff).c_str());
		draw_string(GLUT_BITMAP_TIMES_ROMAN_24, strDiff, -9.9, 9.3, 0,0,0);

		//Hint Timer Text
		int intTime = gm->Htimer; //내림
		int floatTime = (gm->Htimer - intTime) * 100; //소수점 아래 둘째자리
		string time = "Timer: " + to_string(intTime) + "." + to_string(floatTime);
		char* strTime = const_cast<char*>((time).c_str());

		draw_string(GLUT_BITMAP_TIMES_ROMAN_24, strTime, -0.5, 9.3, 0, 0, 0);
	}
}

void drawStageState()
{
	glViewport(0, 0, g_nGLWidth, g_nGLHeight);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glEnable(GL_DEPTH_TEST);

	//카메라 위치 조명
	GLfloat light_position0[] = { 0.0, 0.0, 0.0, 1.0 };
	glLightfv(GL_LIGHT0, GL_POSITION, light_position0);
	//glDisable(GL_LIGHT0);
	player->drawPlayer();

	gluLookAt(camPos[0], camPos[1], camPos[2], camPos[0] + camDirection[0], camPos[1] + camDirection[1], camPos[2] + camDirection[2], camUp[0], camUp[1], camUp[2]);
	drawText();

	gm->drawSkyBox();

	glPushMatrix();
	glTranslatef(-2.53, 0, 8.09);
	field->drawObj();
	glPopMatrix();

	//drawObject
	glPushMatrix();
	glTranslatef(-12.75, 0, 0); //<보기>맵 local 좌표

	GLfloat light_position1[] = { 3.875, 0.0, 0.0, 1.0 };
	GLfloat light_position3[] = { Obj[1]->pos.x(), Obj[1]->pos.y(), Obj[1]->pos.z(), 1.0 };
	GLfloat light_position5[] = { Obj[5]->pos.x(), Obj[5]->pos.y(), Obj[5]->pos.z(), 1.0 };

	glLightfv(GL_LIGHT1, GL_POSITION, light_position1);
	glLightfv(GL_LIGHT3, GL_POSITION, light_position3);
	glLightfv(GL_LIGHT5, GL_POSITION, light_position5);

	drawObject();  	//draw_axis();
	glPopMatrix();

	//InterObject draw
	drawInterObject(); 	//draw_axis();
	GLfloat light_position2[] = { 3.875, 0.0, 0.0, 1.0 };
	GLfloat light_position4[] = { interObj[1]->pos.x(), interObj[1]->pos.y(), interObj[1]->pos.z(), 1.0 };
	GLfloat light_diffuse4[] = { interObj[1]->color.x(), interObj[1]->color.y(), interObj[1]->color.z(), 1.0f }; //조명 색 변경
	GLfloat light_position6[] = { interObj[5]->pos.x(), interObj[5]->pos.y(), interObj[5]->pos.z(), 1.0 };
	GLfloat light_diffuse6[] = { interObj[5]->color.x(), interObj[5]->color.y(), interObj[5]->color.z(), 1.0f }; //조명 색 변경
	glLightfv(GL_LIGHT4, GL_DIFFUSE, light_diffuse4);
	glLightfv(GL_LIGHT6, GL_DIFFUSE, light_diffuse6);

	glLightfv(GL_LIGHT2, GL_POSITION, light_position2);
	glLightfv(GL_LIGHT4, GL_POSITION, light_position4);
	glLightfv(GL_LIGHT6, GL_POSITION, light_position6);

	if (lightFlag)
	{
		glEnable(GL_LIGHT3);
		glEnable(GL_LIGHT4);
		glDisable(GL_LIGHT5);
		glDisable(GL_LIGHT6);
	}
	else
	{
		glDisable(GL_LIGHT3);
		glDisable(GL_LIGHT4);
		glEnable(GL_LIGHT5);
		glEnable(GL_LIGHT6);
	}

	glFlush();
	glutSwapBuffers();
}

void drawMultiView1()
{
	glViewport(0, 0, g_nGLWidth / 2, g_nGLHeight);
	glLoadIdentity();
	gluLookAt(cam[0], cam[1], cam[2], center[0], center[1], center[2], up[0], up[1], up[2]);

	drawText();

	GLfloat light_position3[] = { Obj[1]->pos.x(), Obj[1]->pos.y(), Obj[1]->pos.z(), 1.0 };
	GLfloat light_position5[] = { Obj[5]->pos.x(), Obj[5]->pos.y(), Obj[5]->pos.z(), 1.0 };

	glLightfv(GL_LIGHT3, GL_POSITION, light_position3);
	glLightfv(GL_LIGHT5, GL_POSITION, light_position5);

	drawObject();
	glFlush();
}

void drawMultiView2()
{
	glViewport(g_nGLWidth / 2, 0, g_nGLWidth / 2, g_nGLHeight);
	glLoadIdentity();
	gluLookAt(cam[0], cam[1], cam[2], center[0], center[1], center[2], up[0], up[1], up[2]);

	GLfloat light_position4[] = { interObj[1]->pos.x(), interObj[1]->pos.y(), interObj[1]->pos.z(), 1.0 };
	GLfloat light_diffuse4[] = { interObj[1]->color.x(), interObj[1]->color.y(), interObj[1]->color.z(), 1.0f }; //조명 색 변경
	GLfloat light_position6[] = { interObj[5]->pos.x(), interObj[5]->pos.y(), interObj[5]->pos.z(), 1.0 };
	GLfloat light_diffuse6[] = { interObj[5]->color.x(), interObj[5]->color.y(), interObj[5]->color.z(), 1.0f }; //조명 색 변경
	glLightfv(GL_LIGHT4, GL_DIFFUSE, light_diffuse4);
	glLightfv(GL_LIGHT6, GL_DIFFUSE, light_diffuse6);

	glLightfv(GL_LIGHT4, GL_POSITION, light_position4);
	glLightfv(GL_LIGHT6, GL_POSITION, light_position6);

	drawInterObject();
	glFlush();
}

void drawHintState()
{
	/* 화면을 깨끗하게 지우기 */
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);      // 물체의 앞/뒤 관계를 위한 DEPTH 추가
	// glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glEnable(GL_DEPTH_TEST);   // 뒤에 있는 object가 앞으로 보이는 현상 제거
	 
	cam[0] = radius * cos(beta * pi / 180) * cos(alpha * pi / 180) + 3.875;
	cam[1] = radius * sin(alpha * pi / 180) + 1.5;
	cam[2] = radius * cos(alpha * pi / 180) * sin(beta * pi / 180) + 5.375;
	up[0] = 0;
	up[1] = cos(alpha * pi / 180);
	up[2] = 0;

	glClear(GL_COLOR_BUFFER_BIT);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	glPopMatrix();
	drawMultiView1(); // 왼쪽에 그리기
	drawMultiView2(); // 오른쪽에 그리기

	if (lightFlag)
	{
		glEnable(GL_LIGHT3);
		glEnable(GL_LIGHT4);
		glDisable(GL_LIGHT5);
		glDisable(GL_LIGHT6);
	}
	else
	{
		glDisable(GL_LIGHT3);
		glDisable(GL_LIGHT4);
		glEnable(GL_LIGHT5);
		glEnable(GL_LIGHT6);
	}

	glutSwapBuffers();
}

void draw(void)
{
	if (gm->curState == 1 || gm->curState == 0) //stage 상태일 때,
	{
		drawStageState();
	}
	else if (gm->curState == 4) //Hint 상태일 때 (Multi ViewPort)
	{
		drawHintState();
	}
}

void idle(void)
{
	//deltaTime
	int timeSinceStart = glutGet(GLUT_ELAPSED_TIME);
	int deltaTime = timeSinceStart - startT;
	startT = timeSinceStart;

	if (gm->curState == 1)
	{
		//정답 확인
		int i = 0;
		for (auto interObj : interObj)
		{
			if (!interObj->isCorrect())
			{
				i++;
			}
		}
		gm->differenece = i;
		if (gm->differenece == 0)
		{
			gm->GameClear();
		}

		//제한시간 타이머
		if (gm->timer > 0)
			gm->timer -= deltaTime * 0.001;
		else
		{
			gm->timer = 0.0f;
			gm->GameOver();
		}

		if (player->animFlag)
		{
			player->animation();
		}
	}
	else if (gm->curState == 4)
	{
		if (gm->Htimer > 0)
			gm->Htimer -= deltaTime * 0.001;
		else
		{
			gm->Htimer = 0.0f;
			gm->curState = 1;
		}
	}

	lightTimer += deltaTime * 0.001;
	if (lightTimer < 2)
		lightFlag = true;
	else if (lightTimer < 4)
		lightFlag = false;
	else
		lightTimer = 0;

	glutPostRedisplay();
}

void pickingEvent()
{
	if (player->state == 1 && (focusedObj->type == 1 || focusedObj->type == 3)) //picking 모드
	{
		isPicking = true;
	}
	else if (player->state == 2 && (focusedObj->type == 2 || focusedObj->type == 3)) //color 모드
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
	if (gm->curState == 1)
	{
		//player 이동
		double cameraSpeed = 0.7f;
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

		if (key == '1') //mode 변경
		{
			player->state = 1;
			printf("player is Picking Mode\n");
		}
		else if (key == '2')
		{
			player->state = 2;
			printf("player is Color Mode\n");
		}

		if (key == 'h' && gm->Htimer > 0)
		{
			gm->curState = 4;
			//Hint camera 변수
			alpha = 0, beta = 90, radius = 8.0;
			cam.set(3.875, 1, 15);
			center.set(3.875, 1.5, 5.375);
			up.set(0, 1, 0);
			gm->Htimer = gm->HlimitTime;
			printf("Hint state\n");
		}
	}
	else if(gm->curState != 4) //Start
	{
		if (gm->curState == 2 || gm->curState == 3) //Restart
		{
			for (auto Obj: interObj)
			{
				Obj->~Object();
			}
			for (auto Obj : Obj)
			{
				Obj->~Object();
			}
			interObj.clear();
			Obj.clear();

			objectInit();
			printf("restart\n");
			PlaySound(TEXT("sound\\BGM.wav"), NULL, SND_ASYNC | SND_ALIAS | SND_LOOP);
		}
		gm->curState = 1;
		startT = glutGet(GLUT_ELAPSED_TIME);
		gm->timer = gm->limitTime;
	}
	glutPostRedisplay();
}

void specialkeyboard(int key, int x, int y)
{
	if (gm->curState == 4)
	{
		if (key == GLUT_KEY_LEFT)
		{
			beta += 5;
		}
		else if (key == GLUT_KEY_RIGHT)
		{
			beta -= 5;
		}
		else if (key == GLUT_KEY_DOWN)
		{
			alpha -= 5;
		}
		else if (key == GLUT_KEY_UP)
		{
			alpha += 5;
		}
	}

	glutPostRedisplay();
}

void mouse(int button, int state, int x, int y) 
{
	if (gm->curState == 1)
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
			player->animFlag = true;
		}
		else if (button == GLUT_LEFT_BUTTON && state == GLUT_UP)
		{
			isPicking = false;
		}
	}
	glutPostRedisplay();
}

void motion(int x, int y) 
{
	if (gm->curState == 1)
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

			double dist = (camPos - focusedObj->pos).length(); //움직일 때, 원래의 거리를 유지
			vec3 objPos = camPos + normalPos * dist;

			focusedObj->pos.set(objPos);
			//printf("%f, %f, %f    %f\n", focusedObj->pos.x(), focusedObj->pos.y(), focusedObj->pos.z(), focusedObj->rot);
		}
	}
	glutPostRedisplay();
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
	glutPostRedisplay();
}

void pickModeWheelInput(int direction)
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
	glutPostRedisplay();
}

void mouse_wheel(int wheel, int direction, int x, int y) {

	if (gm->curState == 1)
	{
		if (focusedObj != NULL && player->state == 1 && (focusedObj->type == 1 || focusedObj->type == 3))
		{
			pickModeWheelInput(direction);
		}
		else if (player->state == 2)
		{
			colorModeWheelInput(direction);
		}
	}
	else if (gm->curState == 4)
	{
		if (direction > 0)
		{
			radius -= 1;
		}
		else
		{
			radius += 1;
		}
	}
	glutPostRedisplay();
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
	glutPostRedisplay();
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
	glutKeyboardFunc(keyboard);
	glutSpecialFunc(specialkeyboard);
	glutIdleFunc(idle);
	//glutEntryFunc(entry); //window 포커스 감지

	menuSetting();

	/* Looping 시작 */
	glutMainLoop();
	return 0;
}