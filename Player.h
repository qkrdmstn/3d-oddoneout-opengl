#pragma once

#include "Object.h"

class Player
{

public:
	int state; //1: picking mode, 2: color mode
	double anim; //animation 회전 각도
	bool animFlag;
	int animCount;

	Vec3<double>* pos; //플레이어 위치
	Vec3<double>* camDirection;
	Object* hand;
	Object* brush; //로컬 좌표로 손 끝에 붙어있음.
public:

	Player(int _state, vec3* _camPos, vec3* _camDirection)
	{
		state = _state;
		pos = _camPos;
		camDirection = _camDirection;
		anim = 0;
		animFlag = false;
		animCount = 0;

		vec3 handPos = *pos + *camDirection * 2 + vec3(3.6, -2.3, -13.1);
		hand = new Object("OBJ\\hand.obj", handPos, 0, vec3(1, 0.8, 0.6), 0);
		brush = new Object("OBJ\\candyBrush.obj", handPos, 0, vec3(1,1,1), 0);
	}

	~Player()
	{

	}

	void drawPlayer() //player 손 draw, 함수 내에서 state에 따라 brush도 그리기
	{

		if (state == 1)
		{
			glPushMatrix();
			glTranslatef(hand->pos.x(), hand->pos.y(), hand->pos.z());
			glRotatef(hand->rot, 0, 1, 0);
			glRotatef(anim, 1, 0, 0);
			hand->drawObj();
			glPopMatrix();
		}
		
		if (state == 2)
		{
			glPushMatrix();
			glTranslatef(brush->pos.x(), brush->pos.y(), brush->pos.z());
			glRotatef(brush->rot, 0, 1, 0);
			glRotatef(anim, 1, 0, 0);
			brush->drawObj();
			glPopMatrix();
		}
	}

	void animation() //손 회전하는 애니메이션
	{
		if (animCount < 30)
		{
			anim -= 1;
			animCount++;
		}
		else if (animCount < 60)
		{
			anim += 1;
			animCount++;
		}
		else
		{
			animCount = 0;
			animFlag = false;
		}
	}

};