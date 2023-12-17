#pragma once

#include<GL/glut.h>
#include <GL/freeglut.h>
#include "Vec3.h"
#include <stdlib.h>
#include <time.h>

struct Snow
{
	vec3 curPos;
	vec3 nxtPos;
	vec3 vel;
};

class Snowing
{
public:
	Snow s[400];

public:

	Snowing()
	{
		CreateSnow();
	}

	~Snowing()
	{

	}

	void CreateSnow() //snow 위치 및 속도를 범위 내에 랜덤 생성
	{
		srand(time(NULL));
		
		for (int i = 0; i < 400; i++)
		{
			float x = ((rand() % 250) / 10.0) - 20;
			float y = (rand() % 15);
			float z = ((rand() % 170) / 10.0) - 1;
			s[i].curPos.set(x, y, z);

			float Vx = ((rand() % 5) - 2) * 0.01;
			float Vy = ((rand() % 3) + 1) * 0.01;
			float Vz = ((rand() % 5) - 2) * 0.01;
			if(Vx == 0)
				Vx = ((rand() % 3) + 1) * 0.01;
			if (Vy > 0)
				Vy = Vy * -1;
			if (Vz == 0)
				Vz = ((rand() % 3) + 1) * 0.01;
			s[i].vel.set(Vx, Vy, Vz);
		};
	}

	void DrawSnow()
	{
		for (int i = 0; i < 400; i++)
		{
			glColor3f(1, 1, 1);
			glPushMatrix();
			glTranslatef(s[i].curPos.x(), s[i].curPos.y(), s[i].curPos.z());
			glutSolidSphere(0.1, 10, 10);
			glPopMatrix();

;			s[i].nxtPos = s[i].curPos + s[i].vel;
			s[i].curPos = s[i].nxtPos;

			if (s[i].curPos.x() < -15) //일정 범위 밖으로 넘어갈 경우, 반대편으로 위치 변경
				s[i].curPos[0] = 10;
			else if(s[i].curPos.x() > 10)
				s[i].curPos[0] = -15;

			if (s[i].curPos.y() < 0)
				s[i].curPos[1] = 15;

			if (s[i].curPos.z() < -1)
				s[i].curPos[2] = 15;
			else if (s[i].curPos.z() > 15)
				s[i].curPos[2] = -1;
		}
	}

};