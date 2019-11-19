#include "Door.h"
#include "Game.h"
#include "Textures.h"
Door::Door()
{
	CSprites* sprites = CSprites::GetInstance();
	CAnimations* animations = CAnimations::GetInstance();
	LPDIRECT3DTEXTURE9 texSimon = CTextures::GetInstance()->Get(ID_TEX_DOOR);

	this->dx = 0;
	this->dy = 0;

	sprites->AddFromFile(texSimon, PATH_DOOR_SPRITE);
	animations->AddFromFile(PATH_DOOR_ANI);

	this->AddAmimationFromFile(PATH_DOOR_ANI);

	visible = true;

}

void Door::Render()
{
	if (!visible) return;
	animations[state]->Render(x, y);
	RenderBoundingBox();
}

void Door::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	CGameObject::Update(dt, coObjects);
}

void Door::GetBoundingBox(float& l, float& t, float& r, float& b)
{
	if (!visible) return;
	if (state == DOOR_STATE_CASTLE)
	{
		l = x + 64;
		t = y + 192;
		r = x + 64 + DOOR_BBOX_CASTLE_WIDTH;
		b = y + 192 + DOOR_BBOX_CASTLE_HEIGHT;
	}
	else
	{
		l = x;
		t = y;
		r = x + DOOR_BBOX_NORMAL_WIDTH;
		b = y + DOOR_BBOX_NORMAL_HEIGHT;
	}
}
