#ifndef VERTEX_H
#define VERTEX_H
#include<d3dx9.h>

void initAllVertexDeclaration();
void destroyAllVertexDeclarations();

enum VertexType {
	Pos,
	PN,
	PNT,
	PT,
	PTC,
	UNKNOWN,
};


struct VertexPos{
	VertexPos() :pos(0.f, 0.f, 0.f) {}
	VertexPos(float x, float y, float z) :pos(x, y, z) {}
	VertexPos(const D3DXVECTOR3& v) :pos(v) {}

	D3DXVECTOR3 pos;
	static IDirect3DVertexDeclaration9* decl;
};

struct VertexPN {
	VertexPN() :pos(0.f, 0.f, 0.f), normal(0.f, 0.f, 0.f) {}
	VertexPN(float x, float y, float z, float nx, float ny, float nz) :pos(x, y, z), normal(nx, ny, nz) {}
	VertexPN(const D3DXVECTOR3& v, const D3DXVECTOR3& n) :pos(v), normal(n) {}

	D3DXVECTOR3 pos;
	D3DXVECTOR3 normal;
	static IDirect3DVertexDeclaration9* decl;
};


struct VertexPNT {
	VertexPNT() :pos(0.f, 0.f, 0.f), normal(0.f, 0.f, 0.f), uv(0.f, 0.f) {}
	VertexPNT(float x, float y, float z, float nx, float ny, float nz, float u, float v) :pos(x, y, z), normal(nx, ny, nz), uv(u, v) {}
	VertexPNT(const D3DXVECTOR3& v, const D3DXVECTOR3& n, const D3DXVECTOR2& UV) :pos(v), normal(n), uv(UV) {}

	D3DXVECTOR3 pos;
	D3DXVECTOR3 normal;
	D3DXVECTOR2 uv;
	static IDirect3DVertexDeclaration9* decl;
};

struct VertexPT {
	VertexPT() :pos(0.f, 0.f, 0.f), uv(0.f, 0.f) {}
	VertexPT(float x, float y, float z, float u, float v) :pos(x, y, z), uv(u, v){}
	VertexPT(const D3DXVECTOR3& v, const D3DXVECTOR2& UV) :pos(v), uv(UV){}

	D3DXVECTOR3 pos;
	D3DXVECTOR2 uv;

	static IDirect3DVertexDeclaration9* decl;
};

//2ŽŸŒ³‰æ‘œ—pPositionTexColor
struct VertexPTC {
	VertexPTC() :pos(0.f, 0.f, 0.f), uv(0.f, 0.f), color(1.f,1.f,1.f,1.f) {}
	VertexPTC(float x, float y, float z, float u, float v, float r, float g, float b,float a) :pos(x, y, z),  uv(u, v),color(r,g,b,a) {}
	VertexPTC(const D3DXVECTOR3& v,const D3DXVECTOR2& UV,const D3DXVECTOR4& color) :pos(v), uv(UV),color(color) {}

	D3DXVECTOR3 pos;
	D3DXVECTOR2 uv;
	D3DXVECTOR4 color;
	
	static IDirect3DVertexDeclaration9* decl;
};



#endif