#pragma once

class Object
{

public:
	int type; //0: 상호작용 X, 1: 위치 변경 O, 2: Color 변경 O, 3: 위치 & Color 변경 O
	double pos[3]; //오브젝트 위치

public:
	Object();
	~Object();


};