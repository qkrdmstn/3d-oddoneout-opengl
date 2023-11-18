#pragma once

#include "Object.h"

class Player
{

public:
	int state; //1: picking mode, 2: color mode
	Vec3<double> pos[3]; //플레이어 위치

	Object* brush; //로컬 좌표로 손 끝에 붙어있음.
	
public:

	Player();
	~Player();
	void draw(); //player 손 darw, 함수 내에서 state에 따라 brush도 그리기
	void animation(); //아마 손 회전하는 애니메이션 필요

};