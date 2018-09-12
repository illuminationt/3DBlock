#include "Text.h"
#include<tchar.h>
#include"Utility.h"
#include"Const.h"

Text::Text():
	m_font(0),
	m_pD3DDev(0){
}


Text::~Text()
{
}

void Text::initialize(HWND hWnd, LPDIRECT3DDEVICE9 dev,int width, int height ) {
	m_hWnd = hWnd;
	m_pD3DDev = dev;
	
	D3DXFONT_DESC fontDesc;
	fontDesc.Height = height;
	fontDesc.Width = width;
	fontDesc.Weight = FW_BOLD;
	fontDesc.MipLevels = 0;
	fontDesc.Italic = false;
	fontDesc.CharSet = DEFAULT_CHARSET;
	fontDesc.OutputPrecision = OUT_DEFAULT_PRECIS;
	fontDesc.Quality = DEFAULT_QUALITY;
	fontDesc.PitchAndFamily = DEFAULT_PITCH | FF_DONTCARE;
	//_tcscpy(fontDesc.FaceName, _T("Times New Roman"));
	strcpy_s(fontDesc.FaceName, _T("メイリオ"));

	HR(D3DXCreateFontIndirect(m_pD3DDev, &fontDesc, &m_font));
	initializeTrue();
}
void Text::initializeTrue() {

	D3DXFONT_DESC fontDesc;
	fontDesc.Height = WINDOW_HEIGHT / 12;
	fontDesc.Width = WINDOW_WIDTH / 50;
	fontDesc.Weight = FW_BOLD;
	fontDesc.MipLevels = 0;
	fontDesc.Italic = false;
	fontDesc.CharSet = DEFAULT_CHARSET;
	fontDesc.OutputPrecision = OUT_DEFAULT_PRECIS;
	fontDesc.Quality = DEFAULT_QUALITY;
	fontDesc.PitchAndFamily = DEFAULT_PITCH | FF_DONTCARE;
	//_tcscpy(fontDesc.FaceName, _T("Times New Roman"));
	strcpy_s(fontDesc.FaceName, _T("メイリオ"));

	HR(D3DXCreateFontIndirect(m_pD3DDev, &fontDesc, &m_fontTrue));
}

void Text::draw(const char* text,int x,int y)const {
	RECT rect;
	rect.top = y;
	rect.left = x;
	rect.bottom = y+WINDOW_HEIGHT;
	rect.right = x + WINDOW_WIDTH;

	m_font->DrawTextA(
		NULL,
		text,
		-1,
		&rect,
		DT_LEFT,
		D3DCOLOR_XRGB(128, 128, 128));
}

void Text::drawTrue(const char* text, int x, int y)const {
	RECT rect;
	rect.top = y;
	rect.left = x;
	rect.bottom = y + WINDOW_HEIGHT;
	rect.right = x + WINDOW_WIDTH;

	m_fontTrue->DrawTextA(
		NULL,
		text,
		-1,
		&rect,
		DT_LEFT,
		D3DCOLOR_XRGB(255, 255, 255));
}

void Text::onLostDevice()const {
	HR(m_font->OnLostDevice());
	HR(m_fontTrue->OnLostDevice());
}

void Text::onResetDevice() const{
	HR(m_font->OnResetDevice());
	HR(m_fontTrue->OnResetDevice());
}