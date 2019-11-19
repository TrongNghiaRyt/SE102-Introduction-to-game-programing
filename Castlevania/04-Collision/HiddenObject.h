#pragma once
#include "GameObject.h"

#define BBOX_HEIGHT 32
#define BBOX_WIDTH	32

#define OBJECT_STATE_AUTOWALK	0

class HiddenObject :
	public CGameObject
{
public:
	HiddenObject();
	
	void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	void Update(DWORD dt);
	void Render();
};

