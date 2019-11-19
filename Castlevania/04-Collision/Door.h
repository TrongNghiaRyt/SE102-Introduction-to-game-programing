#pragma once
#include "GameObject.h"

#define CASTLE_BBOX_HEIGHT	288
#define CASTLE_BBOX_WIDTH	96

#define DOOR_STATE_CASTLE	0
#define DOOR_STATE_NORMAL	1
#define DOOR_STATE_NORM_OPEN	2
#define DOOR_STATE_NORM_STILL	3
#define DOOR_STATE_NORM_CLOSE	4

#define DOOR_BBOX_CASTLE_WIDTH	32//96
#define DOOR_BBOX_CASTLE_HEIGHT	96//288
#define DOOR_BBOX_NORMAL_WIDTH	48
#define DOOR_BBOX_NORMAL_HEIGHT	96

#define ID_TEX_DOOR		1505

#define PATH_DOOR_ANI	L"text\\door_ani.txt"
#define PATH_DOOR_SPRITE L"text\\door.txt"

class Door : public CGameObject
{
	bool visible = false;
public:
	Door();
	~Door();

	virtual void Render();
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects = NULL);
	virtual void GetBoundingBox(float& l, float& t, float& r, float& b);

	void Activate() { visible = true; }
	void Deactivate() { visible = false; }
};

