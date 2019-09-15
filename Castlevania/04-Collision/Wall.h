#pragma once
#include "GameObject.h"
class Wall: public CGameObject
{
	int h, w;
public:
	Wall(int h1, int w1);
	virtual void Render();
	virtual void GetBoundingBox(float& l, float& t, float& r, float& b);
	
	void SetHeightWidth(int h1, int w1) { h = h1; w = w1; }
};

