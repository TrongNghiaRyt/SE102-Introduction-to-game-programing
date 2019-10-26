#include "Simon.h"
#include "debug.h"
#include "BigCandle.h"
#include "Weapon.h"
#include "Wall.h"

Simon::Simon() : CGameObject()
{
	CSprites* sprites = CSprites::GetInstance();
	CAnimations* animations = CAnimations::GetInstance();
	LPDIRECT3DTEXTURE9 texSimon = CTextures::GetInstance()->Get(ID_TEX_SIMON);

	sprites->AddFromFile(texSimon, PATH_SIMON_SPRITE);
	animations->AddFromFile(PATH_SIMON_ANI);
	this->AddAmimationFromFile(PATH_SIMON_ANI);

	untouchable = 0;

	rope = new Rope();
}

Simon::~Simon()
{
	delete rope;
}

void Simon::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	// Calculate dx, dy 
	CGameObject::Update(dt);

	// Simple fall down
	vy += SIMON_GRAVITY * dt;
	float jumpTemp = vy;

	vector<LPCOLLISIONEVENT> coEvents;
	vector<LPCOLLISIONEVENT> coEventsResult;

	coEvents.clear();


	if (state == SIMON_STATE_FLASHING)
	{
		if (startFlashing == false)
		{
			startFlashing = true;
			FlashingTime += GetTickCount();
		}
		else
		{
			float remain = FlashingTime - GetTickCount();

			if (remain <= 0 || remain > SIMON_FLASHING_TIME + 10)
			{
				state = SIMON_STATE_IDLE;
				startFlashing = false;
				remain = 0;
				FlashingTime = SIMON_FLASHING_TIME;
			}
		}
	}

	if (this->isAttacking())
	{
		if (startAttack == false)
		{
			startAttack = true;
			attackTime += GetTickCount();
		}
		else
		{
			float remain = attackTime - GetTickCount();
			//DebugOut(L"[INFO] Remain Time: %f\n", remain);
			//DebugOut(L"[INFO] get tick: %X\n", GetTickCount());

			if (remain <= SIMON_ATTACK_TIME / 3 - SIMON_ATTACK_TIME / 6) rope->SetLastFrame();
			if (remain <= 0 || remain > SIMON_ATTACK_TIME + 10)
			{
				rope->reset();
				Attacking = false;
				startAttack = false;
				remain = 0;
				attackTime = SIMON_ATTACK_TIME;
				animations[SIMON_ANI_ATTACK_LEFT]->Reset();
				animations[SIMON_ANI_ATTACK_RIGHT]->Reset();

				if (state == SIMON_STATE_SITDOWN_ATTACK)
					if (nx > 0) this->SetState(SIMON_STATE_SITDOWN_RIGHT);
					else this->SetState(SIMON_STATE_SITDOWN_LEFT);
				else this->SetState(SIMON_STATE_IDLE);
				
				//Weapon ereas
				if (isWeapon) {
					Weapon* weapon = new Weapon();
					weapon->SetState(weaponType);
					weapon->SetPosition(this->x, this->y + 10);
					weapon->getObjects(obj);
					weapon->setNx(this->nx);
					obj->push_back(weapon);
				}

				isWeapon = false;
			}
		}
		//Update rope position to simon gravity.
		//int tempx = x + dx, tempy = y + vy;
		if (!isWeapon)
		{
			rope->SetSimonPosiiton(x + dx, y + dy, nx);
			if (state == SIMON_STATE_SITDOWN_ATTACK)
				rope->SetSimonPosiiton(x + dx, y + dy + SIMON_SITDOWN_HEIGHT_CHANGE, nx);
			rope->getObjects(obj);
			rope->Update(dt, coObjects);
		}
	}

	if (this->vy >= /*0.075*/ 0.1 && isJumping == true)
		JumpFall = true;

	// reset untouchable timer if untouchable time has passed
	if (GetTickCount() - untouchable_start > SIMON_UNTOUCHABLE_TIME)
	{
		untouchable_start = 0;
		untouchable = 0;
	}


	if (state != SIMON_STATE_DIE)
	{
		//Add objects that can be conllision with simon.
		vector<LPGAMEOBJECT> temp;
		for (int i = 0; i < coObjects->size(); i++)
			if (!dynamic_cast<BigCandle*>((*coObjects)[i]))
				temp.push_back((*coObjects)[i]);

		CalcPotentialCollisions(&temp, coEvents);
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
		x += min_tx * dx + nx * 0.2f;		// nx*0.4f : need to push out a bit to avoid overlapping next frame
		y += min_ty * dy + ny * 0.2f;

		if (nx != 0) vx = 0;
		if (ny != 0) vy = 0;

		//to avoid multi-jump
		if (JumpFall == true && jumpTemp > vy)
		{
			this->SetState(SIMON_STATE_IDLE);
			isJumping = false;
			JumpFall = false;
		}
		//if (isAttacking())
		//{
		//	//int tempx = x + dx, tempy = y + vy;

		//	//rope->SetSimonPosiiton(x, y, nx);
		//	//if (state == SIMON_STATE_SITDOWN_ATTACK)
		//	//	rope->SetSimonPosiiton(x, y + SIMON_SITDOWN_HEIGHT_CHANGE, nx);
		//}
		// Collision logic with Goombas
		
		for (UINT i = 0; i < coEventsResult.size(); i++)
		{
			LPCOLLISIONEVENT e = coEventsResult[i];

			if (dynamic_cast<CollectableObject*>(e->obj))
			{
				CollectableObject* cObj = dynamic_cast<CollectableObject*>(e->obj);
				int cObjState = cObj->GetState();
				
				switch (cObjState) {
				case BIGHEART:
					heart + 5;
					//DeleteObjects(cObj);
					break;
				case ROPEUPGRADE:
					rope->Upgrade();
					//DeleteObjects(cObj);
					state = SIMON_STATE_FLASHING;
					break;
				case DANGGER:
					weaponType = WEAPON_DANGGER;
					break;
				}
				DeleteObjects(cObj);
			}
		}
	}

	// clean up collision events
	for (UINT i = 0; i < coEvents.size(); i++) delete coEvents[i];

	for (UINT i = 0; i < coObjects->size(); i++)
	{
		if (isCollision(coObjects->at(i)))
		{
			if (dynamic_cast<CollectableObject*>((*coObjects)[i]))
			{
				CollectableObject* cObj = dynamic_cast<CollectableObject*>(coObjects->at(i));
				int cObjState = cObj->GetState();

				switch (cObjState) {
				case BIGHEART:
					heart + 5;
					break;
				case ROPEUPGRADE:
					rope->Upgrade();
					this->SetState(SIMON_STATE_FLASHING);
					state = SIMON_STATE_FLASHING;
					break;
				case DANGGER:
					weaponType = WEAPON_DANGGER;
					break;
				}
				DeleteObjects(cObj);
			}
		}
	}
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
	else if (this->isAttacking() && state != SIMON_STATE_FLASHING)
	{
		if (state == SIMON_STATE_SITDOWN_ATTACK)
			if (nx > 0) ani = SIMON_ANI_SIT_ATTACK_RIGHT;
			else ani = SIMON_ANI_SIT_ATTACK_LEFT;
		else
			if (nx > 0) ani = SIMON_ANI_ATTACK_RIGHT;
			else ani = SIMON_ANI_ATTACK_LEFT;
	}
	else if (state == SIMON_STATE_FLASHING)
	{
		if (nx > 0) ani = SIMON_ANI_FLASHING_RIGHT;
		else ani = SIMON_ANI_FLASHING_LEFT;
	}
	else if (isJumping && this->isAttacking()==false)
	{
		if (JumpFall)
			if (nx > 0) ani = SIMON_ANI_IDLE_RIGHT;
			else ani = SIMON_ANI_IDLE_LEFT;
		else
			//if (nx > 0) ani = SIMON_ANI_SITDOWN_RIGHT;
			//else ani = SIMON_ANI_SITDOWN_LEFT;
			if (nx > 0) ani = SIMON_ANI_JUMP_RIGHT;
			else ani = SIMON_ANI_JUMP_LEFT;
	}
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
	animations[ani]->Render(x, y, alpha);
	//animations[testAni]->Render(x, y, alpha);

	if (nx > 0) RenderBoundingBox(SIMON_RIGHT_BBOX, 0);
	else RenderBoundingBox(SIMON_LEFT_BBOX, 0);
	//RenderBoundingBox();

	if (isAttacking() && isWeapon == false) rope->Render();
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
	case SIMON_STATE_ATTACK:
		if (nx > 0)
			SIMON_ANI_ATTACK_RIGHT;
		else
			SIMON_ANI_ATTACK_LEFT;
		break;
	case SIMON_STATE_JUMP:
		vy = -SIMON_JUMP_SPEED_Y;
		isJumping = true;
		break;
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
	case SIMON_STATE_FLASHING:
		Attacking = false;
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
		left = x + SIMON_RIGHT_BBOX;
		top = y;
		right = x + SIMON_RIGHT_BBOX + SIMON_BBOX_WIDTH;
		bottom = y + SIMON_BBOX_HEIGHT;
	}
}

void Simon::attack()
{
	Attacking = true;
	vx = 0;
	//this->SetState(SIMON_STATE_ATTACK);
	////state = SIMON_STATE_ATTACK;
	//vx = 0;
	//this->SetState(SIMON_STATE_ATTACK);
	//rope->SetSimonPosiiton(x, y, nx);

	if (state == SIMON_STATE_SITDOWN_LEFT  || 
		state == SIMON_STATE_SITDOWN_RIGHT || 
		state == SIMON_STATE_SITDOWN_ATTACK)
	{
		this->SetState(SIMON_STATE_SITDOWN_ATTACK);
		rope->SetSimonPosiiton(x, y + SIMON_SITDOWN_HEIGHT_CHANGE, nx);
	}
	else
	{
		this->SetState(SIMON_STATE_ATTACK);
		rope->SetSimonPosiiton(x, y, nx);
	}
}

bool Simon::isSitting()
{
	if (state == SIMON_STATE_SITDOWN_LEFT ||
		state == SIMON_STATE_SITDOWN_RIGHT ||
		state == SIMON_STATE_SITDOWN_ATTACK)
		return true;
	return false;
}

void Simon::setWeapon()
{
	isWeapon = true;
	if (weaponType > -1) this->attack();
	else isWeapon = false;
}



void Simon::DeleteObjects(LPGAMEOBJECT a)
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
