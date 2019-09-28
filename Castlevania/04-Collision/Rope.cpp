#include "Rope.h"
#include "debug.h"

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
	
	DebugOut(L"Rope x = %f, y = %f \n", this->x, this->y);
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
				top = y + 6;
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
				top = y + 6;
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
	}
	else left = top = right = bottom = 0;
}

void Rope::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
}

void Rope::Render()
{
	int ani = 0;

	if (isLeft) ani = 1;

	animations[ani + 2 * currentRope]->Render(x, y);
}
