#include <gl/glut.h>
#include <windows.h>
#include <stdio.h>

void init(void)
{
	//화면의 기본 색으로 dark blue 설정
	glClearColor(0.0f, 0.0f, 0.5f, 1.0f);
}

void draw(void)
{
	//Teapot 1개를 그리는 임시 루틴
	glClear(GL_COLOR_BUFFER_BIT);
	glColor3f(1.0f, 1.0f, 1.0f);
	glutWireTeapot(0.5f);
	glFlush();
}

void mouse(int button, int state, int x, int y)
{
	//인자들을 해석해서 원하는 기능을 구현
	printf("Mouse button is clicked! (%d, %d, %d, %d)\n", button, state, x, y);
}

void motion(int x, int y)
{
	printf("Mouse is moving! (%d, %d)\n", x, y);
}

void keyboard(unsigned char key, int x, int y)
{
	printf("You pressed %c\n", key);
}

void idle(void)
{
	//추후 물체의 움직임(animation) 구현에 이용됨
	printf("%d, second has elapsed since the system was started\n", GetTickCount() / 1000);
}

void size_menu_function(int option)
{
	printf("SizeMenu %d has been selected\n", option);
	switch (option)
	{
	case 1:
		glClear(GL_COLOR_BUFFER_BIT);
		glutWireTeapot(0.1f);
		glFlush();
		break;
	case 2:
		glClear(GL_COLOR_BUFFER_BIT);
		glutWireTeapot(0.5f);
		glFlush();
		break;
	case 3:
		glClear(GL_COLOR_BUFFER_BIT);
		glutWireTeapot(0.7f);
		glFlush();
		break;
	default:
		break;
	}	
}

void color_menu_function(int option)
{
	printf("ColorMenu %d has been selected\n", option);
	switch (option)
	{
	case 1:
		glClearColor(1.0f, 0.0f, 0.0f, 1.0f);
		break;
	case 2:
		glClearColor(0.0f, 1.0f, 0.0f, 1.0f);
		break;
	case 3:
		glClearColor(0.0f, 0.0f, 1.0f, 1.0f);
		break;
	default:
		break;
	}
}

void main_menu_function (int option)
{
	printf("Main menu %d has been selected\n", option);
	if (option == 999)
		exit(0);
}

int main(int argc, char** argv)
{
	int sizeMenu, colorMenu;
	int mainmenu;

	//window 초기화
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
	glutInitWindowSize(500, 500);
	glutInitWindowPosition(300, 300);
	glutCreateWindow("My First GL Program");
	init(); // 사용자 초기화 함수

	//Popup menu 생성 및 추가
	sizeMenu = glutCreateMenu(size_menu_function);
	glutAddMenuEntry("size 0.1f", 1);
	glutAddMenuEntry("size 0.5f", 2);
	glutAddMenuEntry("size 0.7f", 3);

	colorMenu = glutCreateMenu(color_menu_function);
	glutAddMenuEntry("Red", 1);
	glutAddMenuEntry("Green", 2);
	glutAddMenuEntry("Blue", 3);

	mainmenu = glutCreateMenu(main_menu_function);
	glutAddMenuEntry("Quit", 999);
	glutAddMenuEntry("Go!", 11);	
	glutAddSubMenu("Size", sizeMenu);
	glutAddSubMenu("Color", colorMenu);
	glutAttachMenu(GLUT_RIGHT_BUTTON);

	//Callback 함수 정의
	glutDisplayFunc(draw); //draw -> 실제 그리기 함수
	glutMouseFunc(mouse);
	glutMotionFunc(motion);
	glutKeyboardFunc(keyboard);
	//glutIdleFunc(idle);
	
	//Looping 시작
	glutMainLoop();

	return 0;
}


