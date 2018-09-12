#include"Vertex.h"
#include"Graphics.h"
#include"Utility.h"

IDirect3DVertexDeclaration9* VertexPos::decl = 0;
IDirect3DVertexDeclaration9* VertexPN::decl = 0;
IDirect3DVertexDeclaration9* VertexPNT::decl = 0;
IDirect3DVertexDeclaration9* VertexPT::decl = 0;
IDirect3DVertexDeclaration9* VertexPTC::decl = 0;

void initAllVertexDeclaration() {

	D3DVERTEXELEMENT9 vertexPosElements[] = {
		{ 0, 0,  D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_POSITION, 0 },
		D3DDECL_END()
	};

	HR(Graphics::D3DDev()->CreateVertexDeclaration(vertexPosElements, &VertexPos::decl));

	D3DVERTEXELEMENT9 vertexPNElements[] = {
		{ 0, 0,  D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_POSITION, 0 },
		{ 0,12,D3DDECLTYPE_FLOAT3,D3DDECLMETHOD_DEFAULT,D3DDECLUSAGE_NORMAL,0 },
		D3DDECL_END()
	};

	HR(Graphics::D3DDev()->CreateVertexDeclaration(vertexPNElements, &VertexPN::decl));

	D3DVERTEXELEMENT9 vertexPNTElements[] = {
		{ 0, 0,  D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_POSITION, 0 },
		{ 0,12,D3DDECLTYPE_FLOAT3,D3DDECLMETHOD_DEFAULT,D3DDECLUSAGE_NORMAL,0 },
		{ 0,24,D3DDECLTYPE_FLOAT2,D3DDECLMETHOD_DEFAULT,D3DDECLUSAGE_TEXCOORD,0 },
		D3DDECL_END()
	};

	HR(Graphics::D3DDev()->CreateVertexDeclaration(vertexPNTElements, &VertexPNT::decl));

	D3DVERTEXELEMENT9 vertexPTElements[] = {
		{ 0, 0,  D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_POSITION, 0 },
		{ 0,12,D3DDECLTYPE_FLOAT2,D3DDECLMETHOD_DEFAULT,D3DDECLUSAGE_TEXCOORD,0 },
		D3DDECL_END()
	};

	HR(Graphics::D3DDev()->CreateVertexDeclaration(vertexPTElements, &VertexPT::decl));


	D3DVERTEXELEMENT9 vertexPTCElements[] = {
		{ 0, 0,  D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_POSITION, 0 },
		{ 0,12,D3DDECLTYPE_FLOAT2,D3DDECLMETHOD_DEFAULT,D3DDECLUSAGE_TEXCOORD,0 },
		{0,20,D3DDECLTYPE_FLOAT4,D3DDECLMETHOD_DEFAULT,D3DDECLUSAGE_COLOR,0},
		D3DDECL_END()
	};

	HR(Graphics::D3DDev()->CreateVertexDeclaration(vertexPTCElements, &VertexPTC::decl));

}

void destroyAllVertexDeclarations() {
	SAFE_RELEASE(VertexPos::decl);
}