#pragma once

#include "Object.h"

class Player
{

public:
	int state; //1: picking mode, 2: color mode
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

		vec3 handPos = *pos + *camDirection * 2 + vec3(3.8, -2.3, 7.70);
		hand = new TexObject("OBJ\\hand.obj", handPos, 0, vec3(1, 1, 1), 0, "OBJ\\hand_UVmap.bmp");
		//brush = new TexObject("OBJ\\candyBrush.obj", handPos, 0, vec3(1,1,1), 3, "OBJ\\candy_UVmap.bmp");
		brush = new Object("OBJ\\candyBrush.obj", handPos, 0, vec3(1,1,1), 0);
	}

	~Player();

	void drawPlayer() //player 손 draw, 함수 내에서 state에 따라 brush도 그리기
	{

		if (state == 1)
		{
			glPushMatrix();
			glTranslatef(hand->pos.x(), hand->pos.y(), hand->pos.z());
			glRotatef(hand->rot, 0, 1, 0);
			hand->drawObj();
			glPopMatrix();
		}

		
		if (state == 2)
		{
			glPushMatrix();
			glTranslatef(brush->pos.x(), brush->pos.y(), brush->pos.z());
			glRotatef(brush->rot, 0, 1, 0);
			brush->drawObj();
			glPopMatrix();
		}
	}
	void animation(); //아마 손 회전하는 애니메이션 필요

};