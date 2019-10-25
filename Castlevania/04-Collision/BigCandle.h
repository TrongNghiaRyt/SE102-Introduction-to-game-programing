#pragma once
#include "GameObject.h"
#include <time.h>
#include <stdlib.h>

#include "Sprites.h"
#include "Textures.h"
#include "CollectableObject.h"

//#include "BigHeart.h"
//#include "RopeUpgrade.h"

#define ID_TEX_BIGCANDLE 1005
#define PATH_BC_SPRITE	L"text\\BigCandle.txt"
#define PATH_BC_ANI	L"text\\BigCandle_ani.txt"

#define BIGCANDLE_BBOX_WIDTH 32
#define BIGCANDLE_BBOX_HEIGHT	64

class BigCandle :
	public CGameObject
{
	bool isHit = false;
	int items = 0;
	vector<LPGAMEOBJECT>* obj;

public:
	BigCandle();
	BigCandle(int x);

	~BigCandle();

	virtual void Render() { animations[0]->Render(x, y); RenderBoundingBox(); }
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects = NULL);
	virtual void GetBoundingBox(float& l, float& t, float& r, float& b);

	void Hit();

	void getObject(vector<LPGAMEOBJECT>* a) { obj = a; }
};

