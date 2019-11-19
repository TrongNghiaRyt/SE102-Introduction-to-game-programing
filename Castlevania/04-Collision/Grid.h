#pragma once
#include "Game.h"
#include "GameObject.h"
#include "Wall.h"
#include "Weapon.h"
#include "BigCandle.h"
#include "CollectableObject.h"
#include "HiddenObject.h"
#include "Door.h"

#include <vector>
#include <string>
#include <string.h>
#include <unordered_map>
#include <set>

#define LEVEL_CASTLE	2
#define LEVEL_YARD		1

#define GRID_WIDTH 704
#define GRID_SIZE 10 //2 grid for basement

#define OBJECT_TYPE_WALL		0
#define OBJECT_TYPE_CANDLE		1
#define OBJECT_TYPE_HIDDEN		2
#define OBJECT_TYPE_DOOR		3
#define OBJECT_TYPE_ENEMIES		4
#define OBJECT_TYPE_STAIR		5

#define SCREEN_WIDTH 512
#define GRID_DATA_YARD		"text\\map1.tmx"
#define GRID_DATA_CASTLE	"text\\map2.tmx"
using namespace std;

class Grid
{
	vector<LPGAMEOBJECT> *grids;
	//unordered_map<int, LPGAMEOBJECT> coObject;

	//set cotain unique element.
	set<LPGAMEOBJECT> sObject;
	vector<LPGAMEOBJECT>* vObject;

	int oldStart = -1;
	int oldEnd = -1;

public:
	Grid();
	~Grid();

	void LoadGridData(int level);
	void AddObject(int num, LPGAMEOBJECT a);
	void DeleteObject(LPGAMEOBJECT a);

	void Render();
	void Update(DWORD dt);

	void SetCamera(float x, float y);
	vector<LPGAMEOBJECT>* getCoObjects() { return vObject; }
};

