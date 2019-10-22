#pragma once
#include "GameObject.h"

#define BIGHEART 0
#define HEART 1
#define ROPEUPGRADE 2
#define DANGGER 3
#define AXE	4
#define Potion 5
#define MoneyBag	6
#define MoneyMax	7
#define Pork	8
#define JAR	9
#define STOPWATCH	10
#define CROSS	11
#define BOSSBALL	12
#define DOUBLESHOT	13

#define ID_TEX_ITEMS 1500

#define GRAVITY		0.00125f

#define PATH_ITEMS_SPRITE	L"text\\items.txt"
#define PATH_ITEMS_ANI	L"text\\items_ani.txt"

#define BIGHEART_BBOX_WIDTH		24
#define	BIGHEART_BBOX_HEIGHT	20
#define HEART_BBOX_WIDTH		16
#define	HEART_BBOX_HEIGHT		16
#define UPGRADE_BBOX_WIDTH		32
#define	UPGRADE_BBOX_HEIGHT		32
#define DANGGER_BBOX_WIDTH		32
#define	DANGGER_BBOX_HEIGHT		18


class CollectableObject :
	public CGameObject
{
public:
	CollectableObject();
	CollectableObject(int a);
	~CollectableObject();

	virtual void Render();
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects = NULL);
	virtual void GetBoundingBox(float& l, float& t, float& r, float& b);
};

