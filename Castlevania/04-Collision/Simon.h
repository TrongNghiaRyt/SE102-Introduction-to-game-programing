#pragma once
#include "GameObject.h"
#include "Textures.h"
#include "Brick.h"

#define PATH_SIMON_ANI		L"text\\simon_ani.txt"
#define PATH_SIMON_SPRITE	L"text\\simon.txt"
#define ID_TEX_SIMON			100

#define SIMON_WALKING_SPEED		0.1f 
//0.1f
#define SIMON_JUMP_SPEED_Y			0.5f
#define SIMON_JUMP_DEFLECT_SPEED	0.2f
#define SIMON_GRAVITY				0.002f
#define SIMON_DIE_DEFLECT_SPEED		0.5f

#define SIMON_STATE_IDLE			0
#define SIMON_STATE_WALKING_RIGHT	10
#define SIMON_STATE_WALKING_LEFT	11
#define SIMON_STATE_SITDOWN_RIGHT	20
#define SIMON_STATE_SITDOWN_LEFT	21
#define SIMON_STATE_ATTACK			30
#define SIMON_STATE_JUMP			40
#define SIMON_STATE_DIE				-1

#define SIMON_ANI_STAND					0
#define SIMON_ANI_IDLE_RIGHT			1
#define SIMON_ANI_IDLE_LEFT				2
#define SIMON_ANI_SITDOWN_RIGHT			3
#define SIMON_ANI_SITDOWN_LEFT			4
#define SIMON_ANI_WALKING_RIGHT			5
#define SIMON_ANI_WALKING_LEFT			6
#define SIMON_ANI_ATTACK_RIGHT			7
#define SIMON_ANI_ATTACK_LEFT			8
#define SIMON_ANI_SIT_ATTACK_RIGHT		9
#define SIMON_ANI_SIT_ATTACK_LEFT		10


#define SIMON_BBOX_WIDTH  32
#define SIMON_BBOX_HEIGHT 64
#define SIMON_RIGHT_BBOX	18
#define SIMON_LEFT_BBOX	12

#define SIMON_UNTOUCHABLE_TIME 3000

class Simon : public CGameObject
{
	int untouchable;
	DWORD untouchable_start;
	
	int testAni = 0;

public:
	Simon();
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* colliable_objects = NULL);
	virtual void Render();
	void SetState(int state);
	void StartUntouchable() { untouchable = 1; untouchable_start = GetTickCount(); }

	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	void TestAni() { 
		testAni++; 
		if (testAni == animations.size())
			testAni = 0;
	}
};

