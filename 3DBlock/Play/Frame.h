#ifndef FRAME_H
#define FRAME_H

#include<d3dx9.h>
#include"Model.h"
#include"Vertex.h"

class ColliderTriangle;
class Model;
class Frame
{
public:
	Frame();
	~Frame();

	void draw(const D3DXMATRIX& pvm,
		const D3DXVECTOR3& pointLightPos,
		const D3DXVECTOR3& pointLightPos2,
		const D3DXVECTOR3& pointLightPos3);
	VertexType vertex() { return VertexType::PNT; }
	ID3DXMesh* mesh() { return m_model->mesh(); }

	void onLostDevice()const { m_effect->OnLostDevice(); }
	void onResetDevice()const { m_effect->OnResetDevice(); }

private:
	ID3DXEffect* m_effect;
	Model* m_model;
	IDirect3DTexture9* m_tex;
private:	
private:
	D3DXHANDLE m_hTechnique;
	D3DXHANDLE m_hTime;
	D3DXHANDLE m_hWVP;
	D3DXHANDLE m_hPointLightPosW;
	D3DXHANDLE m_hPointLightPosW2;
	D3DXHANDLE m_hPointLightPosW3;
	D3DXHANDLE m_hPointLightAttenuation;
	D3DXHANDLE m_hSpecularAttenuation;
	D3DXHANDLE m_hTex;
};


#endif