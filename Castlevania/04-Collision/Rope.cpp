#include "Rope.h"
#include "debug.h"
#include "Brick.h"
#include "Wall.h"
#include "BigCandle.h"

Rope::Rope()
{
	CSprites* sprites = CSprites::GetInstance();
	CAnimations* animations = CAnimations::GetInstance();
	LPDIRECT3DTEXTURE9 texSimon = CTextures::GetInstance()->Get(ID_TEX_ROPE);

	this->dx = 0;
	this->dy = 0;

	sprites->AddFromFile(texSimon, PATH_ROPE_SPRITE);
	animations->AddFromFile(PATH_ROPE_ANI);

	this->AddAmimationFromFile(PATH_ROPE_ANI);

	currentRope = 0;
}

Rope::~Rope()
{
}

void Rope::SetSimonPosiiton(float x1, float y1, int nx1)
{
	if (currentRope != ROPE_MAX)
	{
		if (nx1 > 0)
		{
			this->SetPosition(x1 - 16, y1);
			isLeft = false;
		}
		else
		{
			isLeft = true;
			this->SetPosition(x1 - 46, y1);
		}
	}
	else
	{
		if (nx1 > 0)
		{
			this->SetPosition(x1 - 16, y1);
			isLeft = false;
		}
		else
		{
			isLeft = true;
			this->SetPosition(x1 - 78, y1);
		}
	}
}

void Rope::reset()
{
	isLastFrame = false; isLeft = false;
	for (int i = 0; i < 6; i++)
	{
		animations[i]->Reset();
	}
}

void Rope::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	if (isLastFrame)
	{
		if (currentRope != ROPE_MAX)
		{
			if (isLeft)
			{
				left = x;
				top = y + 17;
				right = x + ROPE_BBOX_WIDTH;
				bottom = y + 6 + ROPE_BBOX_HEIGHT;
			}
			else
			{
				left = x + ROPE_X_BBOX_SIMON;
				top = y + ROPE_Y_BBOX_SIMON;
				right = x + ROPE_X_BBOX_SIMON + ROPE_BBOX_WIDTH;
				bottom = y + ROPE_Y_BBOX_SIMON + ROPE_BBOX_HEIGHT;
			}
		}
		else
		{
			if (isLeft)
			{
				left = x;
				top = y + 17;
				right = x + ROPE_MAX_BBOX_WIDTH;
				bottom = y + 6 + ROPE_MAX_BBOX_HEIGHT;
			}
			else
			{
				left = x + ROPE_X_BBOX_SIMON;
				top = y + ROPE_Y_BBOX_SIMON;
				right = x + ROPE_X_BBOX_SIMON + ROPE_MAX_BBOX_WIDTH;
				bottom = y + ROPE_Y_BBOX_SIMON + ROPE_MAX_BBOX_HEIGHT;
			}
		}
		//DebugOut(L"left = %d, top = %d, right = %d, bottom = %d\n");
	}
	else left = top = right = bottom = 0;


}

void Rope::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	/*DebugOut(L"rope y = %f\n", this->y);*/

	vector<LPCOLLISIONEVENT> coEvents;
	vector<LPCOLLISIONEVENT> coEventsResult;

	coEvents.clear();

	vector<LPGAMEOBJECT> temp;
	if (isLastFrame)
	{
		for (int i = 0; i < coObjects->size(); i++)
		{
			if (!dynamic_cast<CBrick*>((*coObjects)[i]) ||
				!dynamic_cast<Wall*>((*coObjects)[i])	||
				!dynamic_cast<CollectableObject*>((*coObjects)[i]))
				temp.push_back((*coObjects)[i]);
		}

		CalcPotentialStaticCollisions(&temp, coEvents);
	}
	if (coEvents.size() != 0)
	{
		float min_tx, min_ty, nx = 0, ny;

		FilterCollision(coEvents, coEventsResult, min_tx, min_ty, nx, ny);

		// Collision logic with BigCandle

		for (UINT i = 0; i < coEventsResult.size(); i = i +2)
		{
			LPCOLLISIONEVENT e = coEventsResult[i];

			if (dynamic_cast<BigCandle*>(e->obj))
			{
				BigCandle* bigCandle = dynamic_cast<BigCandle*>(e->obj);


				bigCandle->getObject(obj);
				bigCandle->Hit();
				DeleteObjects(bigCandle, obj);
				DebugOut(L"HIT!\n");

				DeleteObjects(bigCandle, obj);
			}

		}

	}
	for (UINT i = 0; i < coEvents.size(); i++) delete coEvents[i];
}

void Rope::Render()
{
	int ani = 0;

	if (isLeft) ani = 1;

	animations[ani + 2 * currentRope]->Render(x, y);
	if (isLeft) RenderBoundingBox(0, 17);
	else RenderBoundingBox(75,17);
}

void Rope::DeleteObjects(LPGAMEOBJECT a, vector<LPGAMEOBJECT>* objects)
{
	for (int i = 0; i < objects->size(); i++)
	{
		if (objects->at(i) == a)
		{
			objects->erase(objects->begin() + i);
			return;
		}
	}
}
