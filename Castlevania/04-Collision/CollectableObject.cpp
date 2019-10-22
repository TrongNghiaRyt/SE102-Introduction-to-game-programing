#include "CollectableObject.h"
#include "Textures.h"

CollectableObject::CollectableObject()
{
	CSprites* sprites = CSprites::GetInstance();
	CAnimations* animations = CAnimations::GetInstance();
	LPDIRECT3DTEXTURE9 itemsTex = CTextures::GetInstance()->Get(ID_TEX_ITEMS);

	sprites->AddFromFile(itemsTex, PATH_ITEMS_SPRITE);
	animations->AddFromFile(PATH_ITEMS_ANI);
	this->AddAmimationFromFile(PATH_ITEMS_ANI);

	state = 1;
}

CollectableObject::CollectableObject(int a)
{
	CSprites* sprites = CSprites::GetInstance();
	CAnimations* animations = CAnimations::GetInstance();
	LPDIRECT3DTEXTURE9 itemsTex = CTextures::GetInstance()->Get(ID_TEX_ITEMS);

	sprites->AddFromFile(itemsTex, PATH_ITEMS_SPRITE);
	animations->AddFromFile(PATH_ITEMS_ANI);
	this->AddAmimationFromFile(PATH_ITEMS_ANI);

	state = a;
}

CollectableObject::~CollectableObject()
{
}

void CollectableObject::Render()
{
	animations[state]->Render(x, y);
}

void CollectableObject::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	CGameObject::Update(dt);

	vy += GRAVITY * dt;

	vector<LPCOLLISIONEVENT> coEvents;
	vector<LPCOLLISIONEVENT> coEventsResult;
	coEvents.clear();

	CalcPotentialCollisions(coObjects, coEvents);

	if (coEvents.size() == 0)
	{
		x += dx;
		y += dy;
	}
	else
	{
		float min_tx, min_ty, nx = 0, ny;

		FilterCollision(coEvents, coEventsResult, min_tx, min_ty, nx, ny);

		// block 
		//x += min_tx * dx + nx * 0.1f;		// nx*0.4f : need to push out a bit to avoid overlapping next frame
		//y += min_ty * dy + ny * 0.1f;

		if (nx != 0) vx = 0;
		if (ny != 0) vy = 0;
	}
	for (UINT i = 0; i < coEvents.size(); i++) delete coEvents[i];
}

void CollectableObject::GetBoundingBox(float& l, float& t, float& r, float& b)
{
	l = x;
	t = y;

	switch (state)
	{
	case BIGHEART:
		r = x + BIGHEART_BBOX_WIDTH;
		b = y + BIGHEART_BBOX_HEIGHT;
		break;
	case HEART:
		r = x + HEART_BBOX_WIDTH;
		b = y + HEART_BBOX_HEIGHT;
		break;
	case ROPEUPGRADE:
		r = x + UPGRADE_BBOX_WIDTH;
		b = y + UPGRADE_BBOX_HEIGHT;
		break;
	case DANGGER:
		r = x + DANGGER_BBOX_WIDTH;
		b = y + DANGGER_BBOX_HEIGHT;
		break;
	}
}
