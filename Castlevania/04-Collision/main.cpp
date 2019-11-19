/* =============================================================
	INTRODUCTION TO GAME PROGRAMMING SE102
	
	SAMPLE 04 - COLLISION

	This sample illustrates how to:

		1/ Implement SweptAABB algorithm between moving objects
		2/ Implement a simple (yet effective) collision frame work

	Key functions: 
		CGame::SweptAABB
		CGameObject::SweptAABBEx
		CGameObject::CalcPotentialCollisions
		CGameObject::FilterCollision
		CGameObject::GetBoundingBox

	Add functions:
		Viewport and draw GameObjects to Viewport
		
================================================================ */

#include <windows.h>
#include <d3d9.h>
#include <d3dx9.h>

#include "debug.h"
#include "Game.h"
#include "GameObject.h"
#include "Textures.h"

#include "Mario.h"
#include "Brick.h"
#include "Goomba.h"
#include "Wall.h"
#include "Background.h"
#include "HUD.h"
#include "Simon.h"
#include "BigCandle.h"
#include "CollectableObject.h"
#include "Weapon.h"
#include "HiddenObject.h"

#define WINDOW_CLASS_NAME L"SampleWindow"
#define MAIN_WINDOW_TITLE L"04 - Collision"

#define BACKGROUND_COLOR D3DCOLOR_XRGB(0, 0, 0)
#define SCREEN_WIDTH 512//263
#define SCREEN_HEIGHT 480//255

#define MAX_FRAME_RATE 120

#define ID_TEX_HP	1000
#define ID_TEX_BOX	1001

#define ID_TEX_MARIO 10
#define ID_TEX_ENEMY 15
#define ID_TEX_MISC 20

#define ID_TEX_SIMON 100


#define ID_TEX_ROPE	2000
#define ID_TEX_WEAPON 2500
#define ID_TEX_BIGCANDLE 1005
#define ID_TEX_CANDLE	1010
#define ID_TEX_ITEMS	1500
#define ID_TEX_DOOR		1505

CGame *game;

CMario *mario;
CGoomba *goomba;
Wall* wall;
Background bg;
Background bg2;
Simon* simon;
BigCandle* bigCandle;
CollectableObject* collect;
Weapon* wea;
HUD hud;

HiddenObject* hd;

LPD3DXFONT fontNES = NULL;

float viewportX = 0.0, viewportY = 0.0;

vector<LPGAMEOBJECT> objects;

class CSampleKeyHander: public CKeyEventHandler
{
	virtual void KeyState(BYTE *states);
	virtual void OnKeyDown(int KeyCode);
	virtual void OnKeyUp(int KeyCode);
};

CSampleKeyHander * keyHandler; 

void CSampleKeyHander::OnKeyDown(int KeyCode)
{
	DebugOut(L"[INFO] KeyDown: %d\n", KeyCode);
	if (simon->isAttacking() ||
		simon->GetState() == SIMON_STATE_FLASHING ||
		simon->GetState() == SIMON_STATE_AUTOWALK)
		return;

	switch (KeyCode)
	{
	case DIK_X:
		if (simon->IsJumping() == false && simon->isSitting()==false)
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
	case DIK_R:
		simon->SetPosition(1000, 64 + 16 + 3 * 64 + 31);
		simon->SetState(SIMON_STATE_IDLE);
		HiddenObject* hd = new HiddenObject();
		hd->SetState(OBJECT_STATE_AUTOWALK);
		hd->SetPosition(16 + 32 * 5 + 4 * 64 * 4 + 64 + 64 + 32 + 16, 64 + 16 + 3 * 64 + 32 * 2);
		objects.push_back(hd);

	}
}

void CSampleKeyHander::OnKeyUp(int KeyCode)
{
	DebugOut(L"[INFO] KeyUp: %d\n", KeyCode);
}

void CSampleKeyHander::KeyState(BYTE* states)
{
	// disable control key when Mario die 
	//if (mario->GetState() == MARIO_STATE_DIE) return;
	//if (game->IsKeyDown(DIK_RIGHT))
	//	mario->SetState(MARIO_STATE_WALKING_RIGHT);
	//else if (game->IsKeyDown(DIK_LEFT))
	//	mario->SetState(MARIO_STATE_WALKING_LEFT);
	//else
	//	mario->SetState(MARIO_STATE_IDLE);
	if (simon->isAttacking() ||
		simon->GetState() == SIMON_STATE_FLASHING ||
		simon->GetState() == SIMON_STATE_AUTOWALK)
		return;

	if (game->IsKeyDown(DIK_RIGHT))
		if (simon->GetState() == SIMON_STATE_SITDOWN_LEFT ||
			simon->GetState() == SIMON_STATE_SITDOWN_RIGHT)
			simon->SetState(SIMON_STATE_SITDOWN_RIGHT);
		else
			simon->SetState(SIMON_STATE_WALKING_RIGHT);
	else if (game->IsKeyDown(DIK_LEFT))
		if (simon->GetState() == SIMON_STATE_SITDOWN_RIGHT ||
			simon->GetState() == SIMON_STATE_SITDOWN_LEFT)
			simon->SetState(SIMON_STATE_SITDOWN_LEFT);
		else
			simon->SetState(SIMON_STATE_WALKING_LEFT);
	else if (game->IsKeyDown(DIK_DOWN))
		if (simon->getNx() > 0)
			simon->SetState(SIMON_STATE_SITDOWN_RIGHT);
		else
			simon->SetState(SIMON_STATE_SITDOWN_LEFT);
	else if (simon->GetState() != SIMON_STATE_JUMP && simon->GetState() != SIMON_STATE_ATTACK)
		simon->SetState(SIMON_STATE_IDLE);
}

LRESULT CALLBACK WinProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message) {
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}

	return 0;
}

/*
	Load all game resources 
	In this example: load textures, sprites, animations and mario object

	TO-DO: Improve this function by loading texture,sprite,animation,object from file
*/
void LoadResources()
{
	CTextures * textures = CTextures::GetInstance();

	textures->Add(ID_TEX_MARIO, L"textures\\mario.png",D3DCOLOR_XRGB(255, 255, 255));
	textures->Add(ID_TEX_MISC, L"textures\\misc.png", D3DCOLOR_XRGB(176, 224, 248));
	textures->Add(ID_TEX_ENEMY, L"textures\\enemies.png", D3DCOLOR_XRGB(3, 26, 110));
	textures->Add(ID_TEX_BBOX, L"textures\\bbox.png", D3DCOLOR_XRGB(255, 255, 255));
	textures->Add(ID_TEX_HP, L"textures\\hp.png", D3DCOLOR_XRGB(0, 0, 0));
	textures->Add(ID_TEX_BOX, L"textures\\weaponBox2.png", D3DCOLOR_XRGB(0, 0, 0));
	textures->Add(ID_TEX_SIMON, L"textures\\simon.png", D3DCOLOR_XRGB(255, 255, 255));
	textures->Add(ID_TEX_ROPE, L"textures\\rope.png", D3DCOLOR_XRGB(255, 255, 255));
	textures->Add(ID_TEX_BIGCANDLE, L"textures\\items\\BigCandle.png", D3DCOLOR_XRGB(255, 255, 255));
	textures->Add(ID_TEX_ITEMS, L"textures\\items\\Collectable.png", D3DCOLOR_XRGB(255, 255, 255));
	textures->Add(ID_TEX_WEAPON, L"textures\\weapon.png", D3DCOLOR_XRGB(255, 255, 255));
	textures->Add(ID_TEX_DOOR, L"textures\\door.png", D3DCOLOR_XRGB(255, 0, 255));

	bg.getMap(L"textures\\map1_2.png");
	bg.getMapData(L"text\\map11_2.txt");
	bg.getTiles(L"text\\map12_512.txt");
	bg2.getMap(L"textures\\map2.png");
	bg2.getMapData(L"text\\map21.txt");
	bg2.getTiles(L"text\\map22.txt");

	hud.Create();

	CSprites* sprites = CSprites::GetInstance();
	CAnimations* animations = CAnimations::GetInstance();

	//LPDIRECT3DTEXTURE9 texMario = textures->Get(ID_TEX_MARIO);

	// big
	//sprites->Add(10001, 246, 154, 260, 181, texMario);		// idle right

	//sprites->Add(10002, 275, 154, 290, 181, texMario);		// walk
	//sprites->Add(10003, 304, 154, 321, 181, texMario);

	//sprites->Add(10011, 186, 154, 200, 181, texMario);		// idle left
	//sprites->Add(10012, 155, 154, 170, 181, texMario);		// walk
	//sprites->Add(10013, 125, 154, 140, 181, texMario);

	//sprites->Add(10099, 215, 120, 231, 135, texMario);		// die 

	//// small
	//sprites->Add(10021, 247, 0, 259, 15, texMario);			// idle small right
	//sprites->Add(10022, 275, 0, 291, 15, texMario);			// walk 
	//sprites->Add(10023, 306, 0, 320, 15, texMario);			// 

	//sprites->Add(10031, 187, 0, 198, 15, texMario);			// idle small left

	//sprites->Add(10032, 155, 0, 170, 15, texMario);			// walk
	//sprites->Add(10033, 125, 0, 139, 15, texMario);			// 


	LPDIRECT3DTEXTURE9 texMisc = textures->Get(ID_TEX_MISC);
	sprites->Add(20001, 408, 225, 424, 241, texMisc);

	LPDIRECT3DTEXTURE9 texEnemy = textures->Get(ID_TEX_ENEMY);
	sprites->Add(30001, 5, 14, 21, 29, texEnemy);
	sprites->Add(30002, 25, 14, 41, 29, texEnemy);

	sprites->Add(30003, 45, 21, 61, 29, texEnemy); // die sprite

	LPANIMATION ani;

	//ani = new CAnimation(100);	// idle big right
	//ani->Add(10001);
	//animations->Add(400, ani);

	//ani = new CAnimation(100);	// idle big left
	//ani->Add(10011);
	//animations->Add(401, ani);

	//ani = new CAnimation(100);	// idle small right
	//ani->Add(10021);
	//animations->Add(402, ani);

	//ani = new CAnimation(100);	// idle small left
	//ani->Add(10031);
	//animations->Add(403, ani);

	//ani = new CAnimation(100);	// walk right big
	//ani->Add(10001);
	//ani->Add(10002);
	//ani->Add(10003);
	//animations->Add(500, ani);

	//ani = new CAnimation(100);	// // walk left big
	//ani->Add(10011);
	//ani->Add(10012);
	//ani->Add(10013);
	//animations->Add(501, ani);

	//ani = new CAnimation(100);	// walk right small
	//ani->Add(10021);
	//ani->Add(10022);
	//ani->Add(10023);
	//animations->Add(502, ani);

	//ani = new CAnimation(100);	// walk left small
	//ani->Add(10031);
	//ani->Add(10032);
	//ani->Add(10033);
	//animations->Add(503, ani);


	//ani = new CAnimation(100);		// Mario die
	//ani->Add(10099);
	//animations->Add(599, ani);



	//ani = new CAnimation(100);		// brick
	//ani->Add(20001);
	//animations->Add(601, ani);

	//ani = new CAnimation(300);		// Goomba walk
	//ani->Add(30001);
	//ani->Add(30002);
	//animations->Add(701, ani);

	//ani = new CAnimation(1000);		// Goomba dead
	//ani->Add(30003);
	//animations->Add(702, ani);

	//mario = new CMario();
	//mario->AddAnimation(400);		// idle right big
	//mario->AddAnimation(401);		// idle left big
	//mario->AddAnimation(402);		// idle right small
	//mario->AddAnimation(403);		// idle left small

	//mario->AddAnimation(500);		// walk right big
	//mario->AddAnimation(501);		// walk left big
	//mario->AddAnimation(502);		// walk right small
	//mario->AddAnimation(503);		// walk left big

	//mario->AddAnimation(599);		// die

	//mario->SetPosition(50.0f, 0);
	//objects.push_back(mario);
	simon = new Simon();
	simon->SetPosition(1000, 100);
	simon->getObjects(&objects);
	objects.push_back(simon);

	Wall* wall = new Wall(32, bg.GetMapWidth());
	//Wall* wall = new Wall();
	wall->SetPosition(0, 400 - 32);
	//wall->SetHeightWidth(32, bg.GetMapWidth());
	objects.push_back(wall);
	
	Wall* wall2 = new Wall(bg.GetMapHeight(), 32);
	wall2->SetPosition(-32 - 12, 0);
	objects.push_back(wall2);

	Wall* wall3 = new Wall(bg.GetMapHeight(), 32);
	//Wall* wall = new Wall();
	wall3->SetPosition(bg.GetMapWidth(), 0);
	//wall->SetHeightWidth(32, bg.GetMapWidth());
	objects.push_back(wall3);

	for (int i = 0; i < 4; i=i+2)
	{
		BigCandle* bigCandle = new BigCandle(BIGHEART);
		bigCandle->SetPosition(16 + 32 * 5 + i * 64 * 4, 64 + 16 + 3 * 64 + 32);
		objects.push_back(bigCandle);
	}
	for (int i = 1; i < 4; i = i + 2)
	{
		BigCandle* bigCandle = new BigCandle(ROPEUPGRADE);
		bigCandle->SetPosition(16 + 32 * 5 + i * 64 * 4, 64 + 16 + 3 * 64 + 32);
		objects.push_back(bigCandle);
	}
	BigCandle* bigCandle = new BigCandle(DANGGER);
	bigCandle->SetPosition(16 + 32 * 5 + 4 * 64 * 4, 64 + 16 + 3 * 64 + 32);
	objects.push_back(bigCandle);

	HiddenObject* hd = new HiddenObject();
	hd->SetState(OBJECT_STATE_AUTOWALK);
	hd->SetPosition(16 + 32 * 5 + 4 * 64 * 4 + 64 + 64 +32 +16, 64 + 16 + 3 * 64 + 32*2);
	objects.push_back(hd);

}

/*
	Update world status for this frame
	dt: time period between beginning of last frame and beginning of this frame
*/
void Update(DWORD dt)
{
	// We know that Mario is the first object in the list hence we won't add him into the colliable object list
	// TO-DO: This is a "dirty" way, need a more organized way 
	vector<LPGAMEOBJECT> coObjects;
	for (int i = 1; i < objects.size(); i++)
	{
		coObjects.push_back(objects[i]);
	}

	for (int i = 0; i < objects.size(); i++)
	{
		objects[i]->Update(dt,&coObjects);
	}

	//Check scooling windows
	//float x_temp, y_temp;
	//simon->GetPosition(x_temp, y_temp);
	//float nx = simon->getNx();
	//float midPoint = (SCREEN_WIDTH / 2) + 32 / 2;

	//float tempx = x_temp - viewportX;
	//float tempy = y_temp - viewportY;

	//if (tempx != midPoint)
	//	if (nx > 0)
	//		viewportX += tempx - midPoint + 18;
	//	else
	//		viewportX += tempx - midPoint + 12;
	//
	////if (tempy != 120)
	////	viewportY += tempy - 120.0;


	////update Map crolling position condition 
	//if (viewportX < 0) viewportX = 0;
	//if (viewportX + SCREEN_WIDTH > bg.GetMapWidth())
	//	viewportX = bg.GetMapWidth() - SCREEN_WIDTH;

	if (viewportY < 0) viewportY = 0;
	//DebugOut(L"temp x = %f\n", tempx);
	//DebugOut(L"x = %f\n", viewportX);

	//--------------
	//float midPoint = SCREEN_WIDTH / 2;

	//float xs, ys;
	//simon->GetPosition(xs, ys);
	//int nx = simon->getNx();

	//float tempx = xs - viewportX;
	//float tempy = ys - viewportY;

	//if (tempx != midPoint)
	//	if (nx > 0)
	//		viewportX += tempx - midPoint;
	//	else
	//		viewportX += tempx - midPoint;

	////update Map crolling position condition 
	//if (viewportX < 0) viewportX = 0;
	//if (viewportY < 0) viewportY = 0;

	//DebugOut(L"x = %f, y = %f\n", viewportX, viewportY);
	//DebugOut(L"xs = %f, ys = %f\n", xs, ys);
	//--------------
	//float sx, sy;
	//simon->GetPosition(sx, sy);
	//viewportX = sx - 512 / 2;
	//
	//if (sx < 512 / 2)
	//	viewportX = 0;

	// Update camera to follow mario
	float cx, cy;
	simon->GetPosition(cx, cy);

	if (cx < SCREEN_WIDTH / 2) cx = 0;
	else cx -= SCREEN_WIDTH / 2;

	if (cx > bg.GetMapWidth() - SCREEN_WIDTH) cx = bg.GetMapWidth() - SCREEN_WIDTH;

	cy -= SCREEN_HEIGHT / 2;
	//if (cx < 512 / 2) cx = 0;
	bg.GetCamPos(cx, 0.0);
	CGame::GetInstance()->SetCamPos(cx, 0.0f /*cy*/);

	//for (int i = 0; i < objects.size(); i++)
	//{
	//	objects[i]->SetDrawPoint(viewportX, viewportY);
	//}
	hud.Update(dt);
}

/*
	Render a frame 
*/
void Render()
{
	LPDIRECT3DDEVICE9 d3ddv = game->GetDirect3DDevice();
	LPDIRECT3DSURFACE9 bb = game->GetBackBuffer();
	LPD3DXSPRITE spriteHandler = game->GetSpriteHandler();

	if (d3ddv->BeginScene())
	{
		// Clear back buffer with a color
		d3ddv->ColorFill(bb, NULL, BACKGROUND_COLOR);

		spriteHandler->Begin(D3DXSPRITE_ALPHABLEND);

		//bg.DrawMap(viewportX, viewportY);
		bg.DrawMap();

		for (int i = 1; i < objects.size(); i++)
			objects[i]->Render();
		objects[0]->Render();
		//objects[objects.size() - 1]->Render();
		hud.Draw(); 

		spriteHandler->End();
		d3ddv->EndScene();
	}


	d3ddv->Present(NULL, NULL, NULL, NULL);
}

HWND CreateGameWindow(HINSTANCE hInstance, int nCmdShow, int ScreenWidth, int ScreenHeight)
{
	WNDCLASSEX wc;
	wc.cbSize = sizeof(WNDCLASSEX);

	wc.style = CS_HREDRAW | CS_VREDRAW;
	wc.hInstance = hInstance;

	wc.lpfnWndProc = (WNDPROC)WinProc;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hIcon = NULL;
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	wc.lpszMenuName = NULL;
	wc.lpszClassName = WINDOW_CLASS_NAME;
	wc.hIconSm = NULL;

	RegisterClassEx(&wc);

	HWND hWnd =
		CreateWindow(
			WINDOW_CLASS_NAME,
			MAIN_WINDOW_TITLE,
			WS_OVERLAPPEDWINDOW, // WS_EX_TOPMOST | WS_VISIBLE | WS_POPUP,
			CW_USEDEFAULT,
			CW_USEDEFAULT,
			ScreenWidth,
			ScreenHeight,
			NULL,
			NULL,
			hInstance,
			NULL);

	if (!hWnd) 
	{
		OutputDebugString(L"[ERROR] CreateWindow failed");
		DWORD ErrCode = GetLastError();
		return FALSE;
	}

	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);

	return hWnd;
}

int Run()
{
	MSG msg;
	int done = 0;
	DWORD frameStart = GetTickCount();
	DWORD tickPerFrame = 1000 / MAX_FRAME_RATE;

	while (!done)
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			if (msg.message == WM_QUIT) done = 1;

			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}

		DWORD now = GetTickCount();

		// dt: the time between (beginning of last frame) and now
		// this frame: the frame we are about to render
		DWORD dt = now - frameStart;

		if (dt >= tickPerFrame)
		{
			frameStart = now;

			game->ProcessKeyboard();
			
			Update(dt);
			Render();
		}
		else
			Sleep(tickPerFrame - dt);	
	}

	return 1;
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	HWND hWnd = CreateGameWindow(hInstance, nCmdShow, SCREEN_WIDTH, SCREEN_HEIGHT);

	game = CGame::GetInstance();
	game->Init(hWnd);

	keyHandler = new CSampleKeyHander();
	game->InitKeyboard(keyHandler);


	LoadResources();

	SetWindowPos(hWnd, 0, 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, SWP_NOMOVE | SWP_NOOWNERZORDER | SWP_NOZORDER);

	Run();

	return 0;
}