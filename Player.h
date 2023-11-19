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

		vec3 handPos = *pos + *camDirection * 2;
		vec3 brushPos = handPos + *camDirection;
		hand = new Object("OBJ\\cube.obj", handPos, vec3(1, 0, 0));
		brush = new Object("OBJ\\cube.obj", *pos + 3, vec3(1, 1, 1));

		
	}

	~Player();

	void drawPlayer() //player 손 darw, 함수 내에서 state에 따라 brush도 그리기
	{
		vec3 handPos = *pos + vec3(0,-1,0);
		hand->center = handPos;

		vec3 brushPos = handPos + *camDirection;

		glPushMatrix();
		glTranslatef(hand->center.x(), hand->center.y(), hand->center.z());
		hand->drawObj();
		glPopMatrix();
		
		//if (state == 2)
			//brush->drawObj();
	}

	void animation(); //아마 손 회전하는 애니메이션 필요

};