#pragma once
#include "Sence.h"
#include "Game.h"

//--Game Objects--
#include "Simon.h"
#include "Wall.h"
#include "CollectableObject.h"
#include "BigCandle.h"

#include "HUD.h"
#include "Background.h"
#include "Grid.h"

#define GAME_STAGE_11 1
#define GAME_STAGE_12 2
#define GAME_STATE_13 3
#define GAME_STAGE_14 4

#define HUD_HEIGHT 96

class PlaySence :
	public Sence
{
	int gameStage;

	float camX, camY;
	float borderLeft, borderRight;
	Simon* simon;

	HUD hud;
	Background bg;
	Grid grid;
	vector<LPGAMEOBJECT> objects;

public:
	PlaySence();

	void Render();
	void Update(DWORD dt);
	void LoadResources();

	void KeyState(BYTE* states);
	void OnKeyDown(int KeyCode);
	void OnKeyUp(int KeyCode);

	void StageUp() { gameStage++; }
};

