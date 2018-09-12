#ifndef TEXT_H
#define TEXT_H

#include<d3dx9.h>

class Text
{
public:
	Text();
	~Text();

	void initialize(HWND hWnd, LPDIRECT3DDEVICE9 dev,int width, int height );
	void initializeTrue();
	void draw(const char* text,int x=0,int y=0)const;
	void drawTrue(const char* text, int x = 0, int y = 0)const;


	void onLostDevice()const;
	void onResetDevice()const;

private :
	ID3DXFont* m_font;
	ID3DXFont* m_fontTrue;
	LPDIRECT3DDEVICE9 m_pD3DDev;
	HWND m_hWnd;
};

#endif
