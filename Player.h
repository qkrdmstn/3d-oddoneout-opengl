#pragma once

#include "Object.h"

class Player
{

public:
	int state; //1: picking mode, 2: color mode
	Vec3<double> pos[3]; //�÷��̾� ��ġ

	Object* brush; //���� ��ǥ�� �� ���� �پ�����.
	
public:

	Player();
	~Player();
	void draw(); //player �� darw, �Լ� ������ state�� ���� brush�� �׸���
	void animation(); //�Ƹ� �� ȸ���ϴ� �ִϸ��̼� �ʿ�

};