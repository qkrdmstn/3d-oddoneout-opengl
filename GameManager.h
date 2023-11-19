#pragma once

class GameManager
{
public:
	int curState; //0: start, 1: stage, 2: color, 3: clear, 4: over
	int curStage; //0-2;


public:
	GameManager();
	~GameManager();
};