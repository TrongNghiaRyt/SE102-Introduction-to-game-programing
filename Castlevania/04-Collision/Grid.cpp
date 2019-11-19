#include "Grid.h"
#include "tinyxml.h"
#include "debug.h"

#define 

Grid::Grid()
{
	grids = new vector<LPGAMEOBJECT>[GRID_SIZE];
}

Grid::~Grid()
{
	delete[] grids;
}

void Grid::LoadGridData(int level)
{
	switch (level)
	{
	case LEVEL_YARD:
	{
		TiXmlDocument doc("text\\map1.tmx");
		if (!doc.LoadFile())
		{
			DebugOut(L"Load object data failed!\n");
			break;
		}

		TiXmlElement* root = doc.RootElement();
		TiXmlElement* objectGroup = root->FirstChildElement();

		for (TiXmlElement* object = objectGroup->FirstChildElement(); object != NULL; object = object->NextSiblingElement())
		{
			int ObjectType;
			object->QueryIntAttribute("type", &ObjectType);

			switch (ObjectType)
			{
			case OBJECT_TYPE_WALL:
			{
				int x, y, w, h;
				object->QueryIntAttribute("x", &x);
				object->QueryIntAttribute("y", &y);
				object->QueryIntAttribute("width", &w);
				object->QueryIntAttribute("height", &h);

				int gridNum;
				TiXmlElement* grid = object->FirstChildElement()->FirstChildElement();
				grid->QueryIntAttribute("value", &gridNum);

				Wall* wall = new Wall(w, h);
				wall->SetPosition(x, y);
				this->AddObject(gridNum, wall);

				break;
			}
			case OBJECT_TYPE_CANDLE:
			{
				int x, y, w, h;
				object->QueryIntAttribute("x", &x);
				object->QueryIntAttribute("y", &y);
				object->QueryIntAttribute("width", &w);
				object->QueryIntAttribute("height", &h);

				int gridNum, objectContain, type;

				TiXmlElement* grid = object->FirstChildElement()->FirstChildElement();
				grid->QueryIntAttribute("value", &gridNum);

				TiXmlElement* contain = grid->NextSiblingElement();
				contain->QueryIntAttribute("value", &objectContain);

				TiXmlElement* Type = contain->NextSiblingElement();
				contain->QueryIntAttribute("value", &type);

				BigCandle* bigCandle = new BigCandle(objectContain);
				bigCandle->SetState(type);
				bigCandle->SetPosition(x, y);

				this->AddObject(gridNum, bigCandle);

				break;
			}
			case OBJECT_TYPE_HIDDEN:
			{
				int x, y, w, h;
				object->QueryIntAttribute("x", &x);
				object->QueryIntAttribute("y", &y);
				object->QueryIntAttribute("width", &w);
				object->QueryIntAttribute("height", &h);

				int gridNum, type;

				TiXmlElement* grid = object->FirstChildElement()->FirstChildElement();
				grid->QueryIntAttribute("value", &gridNum);

				TiXmlElement* Type = grid->NextSiblingElement();
				Type->QueryIntAttribute("value", &type);

				HiddenObject* hd = new HiddenObject();
				hd->SetState(OBJECT_STATE_AUTOWALK);
				hd->SetPosition(x, y);
				this->AddObject(gridNum, hd);
			}
			}

		}

		break;
	}

	case LEVEL_CASTLE:
	{
		break;
	}
	}
}

void Grid::AddObject(int num, LPGAMEOBJECT a)
{
	if (num < 10)
		grids[num].push_back(a);
	else
	{
		while (num != 0)
		{
			int temp = num % 10;
			num /= 10;
			grids[temp].push_back(a);
		}
	}
}

void Grid::DeleteObject(LPGAMEOBJECT a)
{
	for (int i = 0; i<GRID_SIZE; i++)
		for (int j = 0; j < grids[i].size(); j++)
		{
			if (grids[i][j] == a)
			{
				grids[i].erase(grids[i].begin() + j);
				return;
			}
		}
}

void Grid::Render()
{
	for (auto v : *vObject) v->Render();
}

void Grid::Update(DWORD dt)
{
	for (int i = 0; i < vObject->size(); i++)
	{
		if ((*vObject)[i] == NULL)
		{
			DeleteObject(vObject->at(i));
			vObject->erase(vObject->begin() + i);
		}
		else
			vObject->at(i)->Update(dt, vObject);
	}
}

void Grid::SetCamera(float x, float y)
{
	int start, end;

	start = (int)x / GRID_WIDTH;
	end = ((int)x + SCREEN_WIDTH) / GRID_WIDTH;

	if (oldStart == start && oldEnd == end) return;
	else { oldStart = start; oldEnd = end;}

	for (int j = 0; j < grids[start].size(); j++) sObject.insert(grids[start][j]);

	if (start == end) return;

	for (int j = 0; j < grids[end].size(); j++) sObject.insert(grids[end][j]);

	vector<LPGAMEOBJECT> vec(sObject.begin(), sObject.end());
	vObject = &vec;
}
