#pragma once
#include "GameObject.h"
#include "Textures.h"

#define ID_TEX_ROPE				200
#define PATH_ROPE_SPRITE	L"text\\rope.txt"
#define PATH_ROPE_ANI		L"text\\rope_ani.txt"

#define ROPE_NORMAL_ANI_RIGHT	0
#define ROPE_NORMAL_ANI_LEFT	1

#define ROPE_BBOX_HEIGHT		16
#define ROPE_BBOX_WIDTH			46
#define ROPE_MAX_BBOX_HEIGHT	16
#define ROPE_MAX_BBOX_WIDTH		78
#define ROPE_X_BBOX_SIMON	37
#define ROPE_Y_BBOX_SIMON	5

#define ROPE_SIMON_POSITION_X_RIGHT	-16
#define ROPE_SIMON_POSITION_X_LEFT	22

#define ROPE_MAX	2

class Rope: public CGameObject
{
	bool isLastFrame = false;
	bool isLeft = false;
	int currentRope = 0;

public:
	Rope();
	~Rope();

	void SetLastFrame() { isLastFrame = true; }
	void SetSimonPosiiton(float x1, float y1, int nx1);

	void reset();

	void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	void Render();


	void Upgrade() { if (currentRope < 2) currentRope++; }
};

