#pragma once
#include "Game.h"

class Sence
{
public:
	virtual void Render() = 0;
	virtual void Update() = 0;
	virtual void LoadResources() = 0;

	virtual void KeyState(BYTE* states) = 0;
	virtual void OnKeyDown(int KeyCode) = 0;
	virtual void OnKeyUp(int KeyCode) = 0;
};

