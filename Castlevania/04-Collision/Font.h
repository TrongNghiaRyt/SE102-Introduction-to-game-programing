#pragma once

#include <windows.h>
#include <d3d9.h>
#include <d3dx9.h>
#include "Game.h"
#include "Debug.h"

class Font
{
	LPDIRECT3DDEVICE9 d3ddv = CGame::GetInstance()->GetDirect3DDevice();
	LPD3DXSPRITE spriteHandler = CGame::GetInstance()->GetSpriteHandler();
public:
	LPD3DXFONT MakeFont(LPCWSTR name, int size);
	void FontPrint(int x, int y, LPCWSTR text, D3DCOLOR color = D3DCOLOR_XRGB(255, 255, 255));
};

