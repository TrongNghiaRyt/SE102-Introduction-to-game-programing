#include "Weapon.h"
#include "BigCandle.h"
#include "debug.h"

Weapon::Weapon()
{
	CSprites* sprites = CSprites::GetInstance();
	CAnimations* animations = CAnimations::GetInstance();
	LPDIRECT3DTEXTURE9 texWeapon = CTextures::GetInstance()->Get(ID_TEX_WEAPON);

	sprites->AddFromFile(texWeapon, PATH_WEAPON_SPRITE);
	animations->AddFromFile(PATH_WEAPON_ANI);
	this->AddAmimationFromFile(PATH_WEAPON_ANI);
	
	state = 0;
}

void Weapon::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = x;
	top = y;

	switch (state)
	{
	case WEAPON_DANGGER:
		right = x + 32;
		bottom = y + 18;
	}
}

void Weapon::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	CGameObject::Update(dt, coObjects);

	switch (state)
	{
	case WEAPON_DANGGER:
		if (nx > 0) 
			vx = DANGGER_SPEED;
		else 
			vx = DANGGER_SPEED;
		vx = DANGGER_SPEED;
		vy += 0;
		x += vx * dt * nx;
		y += dy;
		DebugOut(L"Nx = %d\n", this->nx);
		DebugOut(L"vx = %f\n", vx * dt * nx);
		break;
		
	}
	vector<LPCOLLISIONEVENT> coEvents;
	vector<LPCOLLISIONEVENT> coEventsResult;

	coEvents.clear();

	vector<LPGAMEOBJECT> temp;
	for (int i = 0; i < coObjects->size(); i++)
		if (dynamic_cast<BigCandle*>((*coObjects)[i]))
			temp.push_back((*coObjects)[i]);

	CalcPotentialCollisions(&temp, coEvents);

	if (coEvents.size() == 0)
	{
		//x += dx;
		//y += dy;
	}
	else
	{
		float min_tx, min_ty, nx = 0, ny;

		FilterCollision(coEvents, coEventsResult, min_tx, min_ty, nx, ny);

		for (UINT i = 0; i < coEventsResult.size(); i++)
		{
			LPCOLLISIONEVENT e = coEventsResult[i];

			if (dynamic_cast<BigCandle*>(e->obj))
			{
				BigCandle* bigCandle = dynamic_cast<BigCandle*>(e->obj);

				bigCandle->getObject(obj);
				bigCandle->Hit();
				DeleteObjects(bigCandle);
				SetPosition(-100, -100);
			}
		}

		for (UINT i = 0; i < coEvents.size(); i++) delete coEvents[i];
	}
}

void Weapon::Render()
{
	int ani = 0;

	switch (state)
	{
	case WEAPON_DANGGER:
		if (nx > 0) ani = WEAPON_ANI_DANGGER_R;
		else ani = WEAPON_ANI_DANGGER_L;
	}

	animations[ani]->Render(x, y);
}

void Weapon::DeleteObjects(LPGAMEOBJECT a)
{
	for (int i = 0; i < obj->size(); i++)
	{
		if (obj->at(i) == a)
		{
			obj->erase(obj->begin() + i);
			return;
		}
	}
}
