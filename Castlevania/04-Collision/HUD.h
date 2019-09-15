#pragma once

#include <windows.h>
#include <d3d9.h>
#include <d3dx9.h>
#include "Game.h"
#include "Debug.h"
#include "Textures.h"
#include "Sprites.h"

#define ID_TEX_HP	1000
#define ID_TEX_BOX	1001

class HUD
{

	LPSPRITE simonHP;
	LPSPRITE enemyHP;
	LPSPRITE HP;
	LPSPRITE box;

	DWORD timeCountSec = 1000;
	int time;
	int stage;
	int score;

	float remain = 0;
public:
	HUD();
	void Create();
	void Draw();
	void Update(DWORD dt);

	void FillNumber(int x, int y, int number, int size);
};

