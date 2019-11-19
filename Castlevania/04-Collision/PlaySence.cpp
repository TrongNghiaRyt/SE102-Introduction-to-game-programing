#include "PlaySence.h"

PlaySence::PlaySence()
{
	simon = new Simon();
	LoadResources();
}

void PlaySence::Render()
{
	hud.Draw();
	bg.DrawMap();
	grid.Render();
	simon->Render();
}

void PlaySence::Update(DWORD dt)
{
	hud.Update(dt);
	grid.Update(dt);
	simon->Update(dt, grid.getCoObjects());

	//update camera
	if (camY < 0) camY = 0;
	float cx, cy;
	simon->GetPosition(cx, cy);

	//if (cx < SCREEN_WIDTH / 2) cx = 0;
	//else cx -= SCREEN_WIDTH / 2;
	//if (cx > bg.GetMapWidth() - SCREEN_WIDTH) cx = bg.GetMapWidth() - SCREEN_WIDTH;

	cx -= SCREEN_WIDTH / 2;
	if (cx < borderLeft) cx = borderLeft;
	if (cx > borderRight - SCREEN_WIDTH) cx = borderRight - SCREEN_WIDTH;
	cy = 0;

	bg.GetCamPos(cx, 0.0);
}

void PlaySence::LoadResources()
{
	switch (gameStage)
	{
	case GAME_STAGE_11:
	{
		bg.ClearMap();
		bg.getMap(L"textures\\map1_2.png");
		bg.getMapData(L"text\\map11_2.txt");
		bg.getTiles(L"text\\map12_512.txt");

		borderLeft = 0;
		borderRight = bg.GetMapWidth();

		grid.LoadGridData(GAME_STAGE_11);

		simon->SetPosition(80, 224 + HUD_HEIGHT - 1);

		break;
	}
	case GAME_STAGE_12:
	{
		bg.ClearMap();
		bg.getMap(L"textures\\map2.png");
		bg.getMapData(L"text\\map21.txt");
		bg.getTiles(L"text\\map22.txt");

		//grid.LoadGridData(GAME_STAGE_12);

		simon->SetPosition(80, 224 + 32 + HUD_HEIGHT - 1);

		break;
	}
	}
}

void PlaySence::KeyState(BYTE* states)
{
	if (simon->isAttacking() ||
		simon->GetState() == SIMON_STATE_FLASHING)
		return;

	if (CGame::GetInstance()->IsKeyDown(DIK_RIGHT))
		if (simon->GetState() == SIMON_STATE_SITDOWN_LEFT ||
			simon->GetState() == SIMON_STATE_SITDOWN_RIGHT)
			simon->SetState(SIMON_STATE_SITDOWN_RIGHT);
		else
			simon->SetState(SIMON_STATE_WALKING_RIGHT);
	else if (CGame::GetInstance()->IsKeyDown(DIK_LEFT))
		if (simon->GetState() == SIMON_STATE_SITDOWN_RIGHT ||
			simon->GetState() == SIMON_STATE_SITDOWN_LEFT)
			simon->SetState(SIMON_STATE_SITDOWN_LEFT);
		else
			simon->SetState(SIMON_STATE_WALKING_LEFT);
	else if (CGame::GetInstance()->IsKeyDown(DIK_DOWN))
		if (simon->getNx() > 0)
			simon->SetState(SIMON_STATE_SITDOWN_RIGHT);
		else
			simon->SetState(SIMON_STATE_SITDOWN_LEFT);
	else if (simon->GetState() != SIMON_STATE_JUMP && simon->GetState() != SIMON_STATE_ATTACK)
		simon->SetState(SIMON_STATE_IDLE);
}

void PlaySence::OnKeyDown(int KeyCode)
{
	DebugOut(L"[INFO] KeyDown: %d\n", KeyCode);
	if (simon->isAttacking() ||
		simon->GetState() == SIMON_STATE_FLASHING)
		return;

	switch (KeyCode)
	{
	case DIK_X:
		if (simon->IsJumping() == false && simon->isSitting() == false)
			simon->SetState(SIMON_STATE_JUMP);
		break;
	case DIK_A: // reset
		//mario->SetState(MARIO_STATE_IDLE);
		//mario->SetLevel(MARIO_LEVEL_BIG);
		//mario->SetPosition(50.0f,0.0f);
		//mario->SetSpeed(0, 0);
		break;
	case DIK_C:
		simon->setWeapon();
		break;
	case DIK_Z:
		simon->attack();
		break;
	case DIK_NUMPAD0:
		simon->TestAni();
	}
}

void PlaySence::OnKeyUp(int KeyCode)
{
	DebugOut(L"[INFO] KeyUp: %d\n", KeyCode);
}
