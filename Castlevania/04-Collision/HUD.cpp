#include "HUD.h"
#include <string>
#include <string.h>

HUD::HUD()
{
}

void HUD::Create()
{
	CTextures* textures = CTextures::GetInstance();
	LPDIRECT3DTEXTURE9 texHP = textures->Get(ID_TEX_HP);
	LPDIRECT3DTEXTURE9 texBox = textures->Get(ID_TEX_BOX);

	simonHP = new CSprite(1, 1, 0, 9, 16, texHP);
	HP = new CSprite(2, 10, 0, 18, 16, texHP);
	enemyHP = new CSprite(3, 19, 0, 27, 16, texHP);
	box = new CSprite(0,0,0,95,44, texBox);

	time = 110;
	score = 456;
	stage = 01;
}

void HUD::Draw()
{
	CGame::GetInstance()->FontPrint(1, 16, L"SCORE-       TIME      STAGE ");

	//CGame::GetInstance()->FontPrint(1 + 96, 16, FillNumber(111, 6));

	FillNumber(1+ 96, 16, 111, 6);

	//CGame::GetInstance()->FontPrint(1 + 96 + 96 + 32 + 64, 16, FillNumber(time, 4));

	FillNumber(1 + 96 + 96 + 32 + 64, 16, time, 4);

	//CGame::GetInstance()->FontPrint(1 + 96 + 96 + 96 + 96 + 80, 16, FillNumber(stage, 2));

	FillNumber(1 + 96 + 96 + 96 + 96 + 80, 16, stage, 2);

	CGame::GetInstance()->FontPrint(1, 16 + 17, L"PLAYER");
	CGame::GetInstance()->FontPrint(1, 16 + 17 + 17, L"ENEMY");

	int n = 16;
	for (int i = 0; i < n; i++)
	{
		//Player HP
		HP->Draw(110 + i * 9, 16 + 17);
		simonHP->Draw(110 + i * 9, 16 + 17);
		CGame::GetInstance()->FontPrint(272 + 95, 16 + 17, L"-05");

		//Enemy HP
		HP->Draw(110 + i * 9, 16 + 17 + 17);
		enemyHP->Draw(110 + i * 9, 16 + 17 + 17);
		CGame::GetInstance()->FontPrint(272 + 95 - 15, 16 + 17 + 17, L"P-05");
	}

	box->Draw(272, 16 + 17);

}

void HUD::Update(DWORD dt)
{

	//Counting playing time by minus 1 sec = 1000 tickCount
	if (remain <= 0 || remain > 1000 + 10)
	{
		timeCountSec += GetTickCount64();
		remain = timeCountSec - GetTickCount64();
	}
	else
	{
		remain = timeCountSec - GetTickCount64();

		if (remain <= 0 || remain > 1000 + 10)
		{
			remain = 0;
			timeCountSec = 1000;
			if (time > 0)
				time -= 1;
			else
				time = 0;
		}
	}
}

void HUD::FillNumber(int x, int y, int number, int size)
{

	int numFill = size - std::to_string(number).length();

	string zeros = {"0"};
	for (int i = 0; i < numFill - 1; i++)
		zeros += "0";
	string text = zeros + std::to_string(number);

	//String to LPCWSTR
	std::wstring stemp = std::wstring(text.begin(), text.end());
	LPCWSTR sw = stemp.c_str();

	CGame::GetInstance()->FontPrint(x, y, sw);
}
