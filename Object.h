#pragma once

class Object
{

public:
	int type; //0: ��ȣ�ۿ� X, 1: ��ġ ���� O, 2: Color ���� O, 3: ��ġ & Color ���� O
	double pos[3]; //������Ʈ ��ġ

public:
	Object();
	~Object();


};