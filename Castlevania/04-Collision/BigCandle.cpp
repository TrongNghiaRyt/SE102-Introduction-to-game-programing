#include "BigCandle.h"

BigCandle::BigCandle()
{
	CSprites* sprites = CSprites::GetInstance();
	CAnimations* animations = CAnimations::GetInstance();
	LPDIRECT3DTEXTURE9 texCandle = CTextures::GetInstance()->Get(ID_TEX_BIGCANDLE);

	sprites->AddFromFile(texCandle, PATH_BC_SPRITE);
	animations->AddFromFile(PATH_BC_ANI);

	this->AddAmimationFromFile(PATH_BC_ANI);

	dx = 0;
	dy = 0;

	srand(time(NULL));
	items = 1;

	state = CANDLE_STATE_BIG;
}

BigCandle::BigCandle(int x)
{
	CSprites* sprites = CSprites::GetInstance();
	CAnimations* animations = CAnimations::GetInstance();
	LPDIRECT3DTEXTURE9 texCandle = CTextures::GetInstance()->Get(ID_TEX_BIGCANDLE);

	sprites->AddFromFile(texCandle, PATH_BC_SPRITE);
	animations->AddFromFile(PATH_BC_ANI);

	this->AddAmimationFromFile(PATH_BC_ANI);

	dx = 0;
	dy = 0;

	items = x;
}

void BigCandle::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
}

void BigCandle::GetBoundingBox(float& l, float& t, float& r, float& b)
{
	l = x;
	t = y;
	if (state == CANDLE_STATE_BIG)
	{
		r = x + BIGCANDLE_BBOX_WIDTH;
		b = y + BIGCANDLE_BBOX_HEIGHT;
	}
	else
	{
		r = x + SMALLCANDLE_BBOX_WIDTH;
		b = y + SMALLCANDLE_BBOX_HEIGHT;
	}

}

void BigCandle::Hit()
{
	isHit = true;

	CollectableObject* temp = new CollectableObject(items);
	temp->SetPosition(this->x, this->y);
	obj->push_back(temp);

}


BigCandle::~BigCandle()
{
}
