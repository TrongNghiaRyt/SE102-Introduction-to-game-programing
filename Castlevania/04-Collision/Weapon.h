#pragma once
#include "GameObject.h"

#define DANGGER_SPEED	0.15f


#define WEAPON_DANGGER 0
#define WEAPON_AXE 1
#define WEAPON_POTION 2

#define PATH_WEAPON_SPRITE L"text\\weapon.txt"
#define PATH_WEAPON_ANI	L"text\\weapon_ani.txt"
#define ID_TEX_WEAPON 2500

#define WEAPON_ANI_DANGGER_R 0
#define WEAPON_ANI_DANGGER_L 1

class Weapon :
	public CGameObject
{
	vector<LPGAMEOBJECT>* obj;
public:

	Weapon();

	void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	void Render();

	void getObjects(vector<LPGAMEOBJECT>* a) { obj = a; }
	void DeleteObjects(LPGAMEOBJECT a);
	void setNx(int x) { 
		this->nx = x; }
};

