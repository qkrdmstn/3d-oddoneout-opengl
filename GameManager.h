#pragma once

class GameManager
{
public:
	int curState; //0: start, 1: stage, 2: clear, 3: over
	int differenece;
	double limitTime;
	double timer;
public:
	GameManager()
	{
		differenece = 0;
		curState = 0;
		limitTime = 10; //sec
	}
	~GameManager()
	{

	}

	void GameClear()
	{
		printf("Game Clear!\n");
	}

	void GameOver()
	{
		printf("Game Over!\n");
	}
};