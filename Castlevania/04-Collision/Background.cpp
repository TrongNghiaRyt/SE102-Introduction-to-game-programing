#include "Background.h"

Background::Background()
{
}

Background::~Background()
{
}

void Background::getMapData(LPCWSTR filename)
{
	ifstream file(filename);
	string line;
	int width, height;
	if (!file)
	{
		DebugOut(L"[ERROR] GetInfoFromFile failed: %s\n", filename);
	}

	file >> width >> height;
	tileWidthNumber = width;
	tileHeightNumber = height;

	//Clear mapdata.
	mapData.clear();

	for (int i = 0; i < width * height; ++i)
	{
		int temp;
		file >> temp;
		mapData.push_back(temp);
	}

	file.close();
}

void Background::getMap(LPCWSTR filename)
{
	CTextures::GetInstance()->Add(level, filename, D3DCOLOR_XRGB(0, 0, 0));
}

void Background::getTiles(LPCWSTR filename)
{
	ifstream file(filename);
	string line;
	if (!file)
	{
		DebugOut(L"[ERROR] Get Tiles from file failed: %s\n", filename);
	}

	while (getline(file, line))	//Passing the first line - information line.
	{
		string name;						//name of the frame.
		int id, left, top, right, bottom;	//Id, posittion of the frame
		file >> name >> id >> left >> top >> right >> bottom;

		LPDIRECT3DTEXTURE9 texMap = CTextures::GetInstance()->Get(level);

		LPSPRITE s = new CSprite(id, left, top, right, bottom, texMap);
		tiles.push_back(s);
	}

	file.close();
}

void Background::DrawMap()
{
	int tilenum = 0;
	int count = -1;
	int columns, rows;

	//calculate starting tile position
	int tilex = cam_x / TILEWIDTH;
	int tiley = cam_y / TILEHEIGHT;

	//calculate the number of columns and rows
	columns = SCREEN_WIDTH / TILEWIDTH;
	rows = SCREEN_HEIGHT / TILEHEIGHT;

	for (int i = 0; i <= rows-2; i++)
	{
		for (int j = 0; j <= columns; j++)
		{
			//int temp = (i*tiley)*tileWidthNumber + tilex + j;
			int temp = ((tiley + i) * tileWidthNumber + (tilex + j));

			if (temp < tileWidthNumber * tileHeightNumber && temp >= 0)
				tilenum = mapData[temp];
			else
				tilenum = mapData[0];

			int x = j * TILEWIDTH + tilex * TILEWIDTH;
			int y = i * TILEHEIGHT + tiley * TILEHEIGHT;

			//draw the tile onto the scroll buffer
			//tiles[tilenum]->Draw(tempx, tempy+64+16);
			tiles[tilenum]->Draw(x, y + 64 + 16);
		}
	}
}
