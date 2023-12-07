#pragma once

#include "Object.h"

class Player
{

public:
	int state; //1: picking mode, 2: color mode
	Vec3<double>* pos; //�÷��̾� ��ġ
	Vec3<double>* camDirection;
	Object* hand;
	Object* brush; //���� ��ǥ�� �� ���� �پ�����.
	
public:

	Player(int _state, vec3* _camPos, vec3* _camDirection)
	{
		state = _state;
		pos = _camPos;
		camDirection = _camDirection;

		vec3 handPos = *pos + *camDirection * 2 + vec3(4,-4,3);
		vec3 brushPos = handPos + *camDirection * 0.5;
		//hand = new Object("OBJ\\cube1.obj", handPos, -35, vec3(1, 0, 0), 0);
		//brush = new Object("OBJ\\cube1.obj", brushPos , 0, vec3(1, 1, 1), 0);
	}

	~Player();

	void drawPlayer() //player �� draw, �Լ� ������ state�� ���� brush�� �׸���
	{

		glPushMatrix();
		glTranslatef(hand->pos.x(), hand->pos.y(), hand->pos.z());
		glRotatef(hand->rot, 0, 1, 0);
		hand->drawObj();
		glPopMatrix();
		
		if (state == 2)
		{
			glPushMatrix();
			glTranslatef(brush->pos.x(), brush->pos.y(), brush->pos.z());
			glRotatef(brush->rot, 0, 1, 0);
			brush->drawObj();
			glPopMatrix();
		}
	}

	void animation(); //�Ƹ� �� ȸ���ϴ� �ִϸ��̼� �ʿ�

};