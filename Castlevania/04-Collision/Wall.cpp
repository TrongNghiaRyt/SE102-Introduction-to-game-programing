#include "Wall.h"

Wall::Wall(int h1, int w1)
{
	h = h1;
	w = w1;
}

void Wall::Render()
{
	RenderBoundingBox();
}

void Wall::GetBoundingBox(float& l, float& t, float& r, float& b)
{
	l = x;
	t = y;
	r = x + w;
	b = y + h;
}
