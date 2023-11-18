#pragma once

class Camera
{
public:
	double theta = 0, phi = 0;
	double camPos[3] = { 15,1,0 };
	double camDirection[3] = { 0,0,0 };
	double camUp[3] = { 0,1,0 };

public:
	Camera();
	~Camera();
};