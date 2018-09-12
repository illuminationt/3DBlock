
/*
#ifndef IMAGE_H
#define IMAGE_H
#include<d3dx9.h>
#include<string>

class Image
{
public:
	Image();
	Image(const char* filename);
	Image(const Image&);
	~Image();

	//p:左上の座標（ピクセル単位)
	void draw(float* p, float width, float height);
	void draw(float x, float y, float width, float height);

	void testDraw1();
private://ヘルパー関数
	void helperDraw(float* p1, float* p2, float* p3, float* uv1 = 0, float* uv2 = 0, float* uv3 = 0);
	

private:
	IDirect3DTexture9* m_texture;
	IDirect3DVertexBuffer9* m_vertexBuffer;
	ID3DXEffect* m_effect;
	IDirect3DVertexDeclaration9* m_decl;
private:
	std::string m_filename;

	D3DXVECTOR2 m_size;

	D3DXVECTOR2 m_position;
	float m_angleZ;
	D3DXVECTOR2 m_scale;

};


#endif
*/

#ifndef IMAGE_H
#define IMAGE_H

#include<d3dx9.h>
#include"Vertex.h"

class Image {
public:
	Image(const char* filename);
	~Image();

	void drawWindow();
	void draw(float x, float y, float width, float height);

	void setPosition(const D3DXVECTOR2&);
	void setSize(const D3DXVECTOR2&);
	void setAngleZ(float);

	void setUVsize(const D3DXVECTOR2&);
	void setUVoffset(const D3DXVECTOR2&);
	void setColor(float r,float g,float b);
	void setAlpha(float a);

	void onLostDevice() { m_effect->OnLostDevice(); }
	void onResetDevice() { m_effect->OnResetDevice(); }
public:
	void draw();
	void testDraw();
	void testDraw2();
private:
	ID3DXEffect* m_effect;
	IDirect3DTexture9* m_texture;
private:
	void createWorldMatrix();
private:
	D3DXVECTOR2 m_position;
	D3DXVECTOR2 m_scale;
	float m_angleZ;
	D3DXVECTOR2 m_size;

	D3DXVECTOR2 m_UVsize;
	D3DXVECTOR2 m_UVoffset;
	
	D3DXVECTOR4 m_color;

	
private://一時保存
	D3DXMATRIX m_projFixed;
	D3DXMATRIX m_projChange;
	D3DXMATRIX m_world;
	
	static VertexPT* m_vertexPT;
	static void create() {if (!m_vertexPT) {m_vertexPT = new VertexPT[4];}}
};



#endif