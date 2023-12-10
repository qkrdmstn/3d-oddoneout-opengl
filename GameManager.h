#pragma once

class GameManager
{
public:
	int curState; //0: start, 1: stage, 2: clear, 3: over
	int differenece;

public:
	GameManager()
	{
		curState = 0;
	}
	~GameManager()
	{

	}
};