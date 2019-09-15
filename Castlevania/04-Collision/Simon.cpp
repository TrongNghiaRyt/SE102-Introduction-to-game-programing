#include "Simon.h"
#include "debug.h"

Simon::Simon() : CGameObject()
{
	CSprites* sprites = CSprites::GetInstance();
	CAnimations* animations = CAnimations::GetInstance();
	LPDIRECT3DTEXTURE9 texSimon = CTextures::GetInstance()->Get(ID_TEX_SIMON);

	sprites->AddFromFile(texSimon, PATH_SIMON_SPRITE);
	animations->AddFromFile(PATH_SIMON_ANI);
	this->AddAmimationFromFile(PATH_SIMON_ANI);

	untouchable = 0;
}

void Simon::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	// Calculate dx, dy 
	CGameObject::Update(dt);

	// Simple fall down
	vy += SIMON_GRAVITY * dt;

	vector<LPCOLLISIONEVENT> coEvents;
	vector<LPCOLLISIONEVENT> coEventsResult;

	coEvents.clear();

	// turn off collision when die 
	if (state != SIMON_STATE_DIE)
		CalcPotentialCollisions(coObjects, coEvents);

	// reset untouchable timer if untouchable time has passed
	if (GetTickCount() - untouchable_start > SIMON_UNTOUCHABLE_TIME)
	{
		untouchable_start = 0;
		untouchable = 0;
	}

	// No collision occured, proceed normally
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
		x += min_tx * dx + nx * 0.4f;		// nx*0.4f : need to push out a bit to avoid overlapping next frame
		y += min_ty * dy + ny * 0.4f;

		if (nx != 0) vx = 0;
		if (ny != 0) vy = 0;

		// Collision logic with Goombas
		for (UINT i = 0; i < coEventsResult.size(); i++)
		{
			LPCOLLISIONEVENT e = coEventsResult[i];

			//if (dynamic_cast<CGoomba*>(e->obj)) // if e->obj is Goomba 
			//{
			//	CGoomba* goomba = dynamic_cast<CGoomba*>(e->obj);

			//	// jump on top >> kill Goomba and deflect a bit 
			//	if (e->ny < 0)
			//	{
			//		if (goomba->GetState() != GOOMBA_STATE_DIE)
			//		{
			//			goomba->SetState(GOOMBA_STATE_DIE);
			//			vy = -SIMON_JUMP_DEFLECT_SPEED;
			//		}
			//	}
			//	else if (e->nx != 0)
			//	{
			//		if (untouchable == 0)
			//		{
			//			if (goomba->GetState() != GOOMBA_STATE_DIE)
			//			{
			//				if (level > SIMON_LEVEL_SMALL)
			//				{
			//					level = SIMON_LEVEL_SMALL;
			//					StartUntouchable();
			//				}
			//				else
			//					SetState(SIMON_STATE_DIE);
			//			}
			//		}
			//	}
			//}
		}
	}

	// clean up collision events
	for (UINT i = 0; i < coEvents.size(); i++) delete coEvents[i];
	//DebugOut(L"x = %f, y = %f\n", x, y);
	//DebugOut(L"xv = %f, yv = %f\n", xcam, ycam);
}

void Simon::Render()
{
	int ani;
	if (state == SIMON_STATE_DIE)
		ani = SIMON_ANI_IDLE_RIGHT;
	//else if (this->isAttacking())
	//{
	//	if (state == SIMON_STATE_SIT_ATTACK)
	//		if (nx > 0) ani = SIMON_ANI_SIT_ATTACK_RIGHT;
	//		else ani = SIMON_ANI_SIT_ATTACK_LEFT;
	//	else
	//		if (nx > 0) ani = SIMON_ANI_ATTACK_RIGHT;
	//		else ani = SIMON_ANI_ATTACK_LEFT;

	//}
	//else if (state == SIMON_STATE_FLASHING)
	//{
	//	if (nx > 0) ani = SIMON_ANI_FLASHING_RIGHT;
	//	else ani = SIMON_ANI_FLASHING_LEFT;
	//}
	else if (state == SIMON_STATE_SITDOWN_RIGHT)
		ani = SIMON_ANI_SITDOWN_RIGHT;
	else if (state == SIMON_STATE_SITDOWN_LEFT)
		ani = SIMON_ANI_SITDOWN_LEFT;
	//else if (isJumping)
	//{
	//	if (JumpFall)
	//		if (nx > 0) ani = SIMON_ANI_IDLE_RIGHT;
	//		else ani = SIMON_ANI_IDLE_LEFT;
	//	else
	//		if (nx > 0) ani = SIMON_ANI_SIT_RIGHT;
	//		else ani = SIMON_ANI_SIT_LEFT;
	//}
	else if (vx == 0)
	{
		if (nx > 0) ani = SIMON_ANI_IDLE_RIGHT;
		else ani = SIMON_ANI_IDLE_LEFT;
	}
	else if (vx > 0)
		ani = SIMON_ANI_WALKING_RIGHT;
	else ani = SIMON_ANI_WALKING_LEFT;


	int alpha = 255;
	if (untouchable) alpha = 128;
	animations[ani]->Render(xcam, ycam, alpha);
	//animations[testAni]->Render(xcam, ycam, alpha);

	DebugOut(L"nx = %d\n", this->nx);

	if (nx > 0) RenderBoundingBox(SIMON_RIGHT_BBOX, 0);
	else RenderBoundingBox(SIMON_LEFT_BBOX, 0);
	//RenderBoundingBox();
}

void Simon::SetState(int state)
{
	CGameObject::SetState(state);

	switch (state)
	{
	case SIMON_STATE_WALKING_RIGHT:
		vx = SIMON_WALKING_SPEED;
		nx = 1;
		break;
	case SIMON_STATE_WALKING_LEFT:
		vx = -SIMON_WALKING_SPEED;
		nx = -1;
		break;
	case SIMON_STATE_JUMP:
		vy = -SIMON_JUMP_SPEED_Y;
	case SIMON_STATE_IDLE:
		vx = 0;
		break;
	case SIMON_STATE_SITDOWN_RIGHT:
		vx = 0;
		nx = 1;
		break;
	case SIMON_STATE_SITDOWN_LEFT:
		vx = 0;
		nx = -1;
		break;
	case SIMON_STATE_DIE:
		vy = -SIMON_DIE_DEFLECT_SPEED;
		break;
	}
}

void Simon::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	if (nx > 0)
	{
		left = x + SIMON_RIGHT_BBOX;
		top = y;
		right = x + SIMON_RIGHT_BBOX + SIMON_BBOX_WIDTH;
		bottom = y + SIMON_BBOX_HEIGHT;
	}
	else
	{
		left = x;
		top = y;
		right = x + SIMON_BBOX_WIDTH;
		bottom = y + SIMON_BBOX_HEIGHT;
	}
}