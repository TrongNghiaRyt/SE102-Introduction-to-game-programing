#pragma once
#include <Windows.h>
#include <d3d9.h>
#include <d3dx9.h>
#include <vector>
#include <iostream>
#include <fstream>
#include <sstream> 
#include <math.h>
#include "Game.h"
#include "Debug.h"
#include "Sprites.h"
#include "Textures.h"

#define SCREEN_WIDTH 512
#define SCREEN_HEIGHT 480

class Background
{
	int level = 0;

	int gameWorldWidth;
	int gameWorldHeight;

	const int TILEWIDTH = 64;
	const int TILEHEIGHT = 64;

	int tileWidthNumber;
	int tileHeightNumber;

	vector<int> mapData;
	vector<LPSPRITE> tiles;

	float cam_x, cam_y;

public:
	Background();
	~Background();

	void getMapData(LPCWSTR filename);	//Matrix data of the map
	void getMap(LPCWSTR filename);		//Map tile on image file to textures
	void getTiles(LPCWSTR filename);	//Add tiles from textures to tiles vector

	void DrawMap();
	void ClearMap();

	int GetMapHeight() { return tileHeightNumber * TILEHEIGHT; }
	int GetMapWidth() { return tileWidthNumber * TILEWIDTH; }

	void GetCamPos(float xv, float yv) { cam_x = xv, cam_y = yv; }
};

