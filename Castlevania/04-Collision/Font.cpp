#include "Font.h"

LPD3DXFONT Font::MakeFont(LPCWSTR name, int size)
{
	LPD3DXFONT f = NULL;
	
	D3DXFONT_DESC desc = {
		size,                   //height
		0,                      //width
		0,                      //weight
		0,                      //miplevels
		false,                  //italic
		DEFAULT_CHARSET,        //charset
		OUT_TT_PRECIS,          //output precision
		CLIP_DEFAULT_PRECIS,    //quality
		DEFAULT_PITCH,          //pitch and family
		L""                     //font name
	};

	//strcpy(desc.FaceName, name);
	wcsncpy_s(desc.FaceName, name, 32);

	D3DXCreateFontIndirect(d3ddv, &desc, &f);

	return f;
}

void Font::FontPrint(int x, int y, LPCWSTR text, D3DCOLOR color)
{
	LPD3DXSPRITE spriteHandler = CGame::GetInstance()->GetSpriteHandler();
	LPD3DXFONT font = MakeFont(L"Nintendo NES Font", 24);

	//figure out the text boundary
	RECT rect = { x, y, 0, 0 };
	font->DrawText(NULL, text, lstrlen(text), &rect, DT_CALCRECT, color);

	//print the text
	font->DrawText(spriteHandler, text, lstrlen(text), &rect, DT_LEFT, color);
}
