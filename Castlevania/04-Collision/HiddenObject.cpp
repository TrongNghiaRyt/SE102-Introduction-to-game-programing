#include "HiddenObject.h"
#include "debug.h"

HiddenObject::HiddenObject()
{
	state = OBJECT_STATE_AUTOWALK;
}

void HiddenObject::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = x;
	top = y;
	right = x + BBOX_WIDTH;
	bottom = y + BBOX_HEIGHT;
}

void HiddenObject::Update(DWORD dt)
{
}

void HiddenObject::Render()
{
	RenderBoundingBox();
}
