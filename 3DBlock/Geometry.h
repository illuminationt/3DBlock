#ifndef GEOMETRY_H
#define GEOMETRY_H

#include<d3dx9.h>
#include"Utility.h"
struct Material {
	Material() :diffuse(WHITE), ambient(WHITE), specular(WHITE), specularPower(10.f) {}
	Material(const D3DXVECTOR3& d, const D3DXVECTOR3& a,
		const D3DXVECTOR3& s, float power) :
		diffuse(d), ambient(a), specular(s), specularPower(power) {}

	D3DXCOLOR diffuse;
	D3DXCOLOR ambient;
	D3DXCOLOR specular;
	float specularPower;
};

class Geometry
{
public:
	Geometry();
	virtual ~Geometry();

	void initialize(ID3DXEffect* pEffect,const char* texFilename=0);

	void draw(const D3DXMATRIX& pvm);
	void draw();
	void drawShadow(const D3DXMATRIX& pvm);


	void setPosition(const D3DXVECTOR3&);
	void setPosition(float x, float y, float z);
	void setAngle(const D3DXVECTOR3&);
	void setAngle(float x, float y, float z);
	void setScale(float ratio);
	void setScale(float x, float y, float z);

	inline const D3DXVECTOR3* position()const { return &m_position; }

	void setMaterial(const D3DXCOLOR& diffuse, const D3DXCOLOR& ambient, const D3DXCOLOR& specular, float specularPower);

	inline ID3DXMesh* mesh()const { return m_pMesh; }
	inline void setMesh(ID3DXMesh* m) { m_pMesh = m; }

	void onLostDevice()const { m_pEffect->OnLostDevice(); }
	void onResetDevice()const { m_pEffect->OnResetDevice(); }
private:
	void createWorldMatrix(D3DXMATRIX* out);
	D3DXMATRIX m_worldMatrix;

protected:
	ID3DXMesh* m_pMesh;
	ID3DXEffect* m_pEffect;
	IDirect3DTexture9* m_texture;

	D3DXHANDLE m_hTransformMatrix;

	D3DXHANDLE m_hDiffuse;
	D3DXHANDLE m_hAmbient;
	D3DXHANDLE m_hSpecular;
	D3DXHANDLE m_hSpecularPower;
	D3DXHANDLE m_hTex;

	D3DXHANDLE m_hWorld;
	//D3DXHANDLE m_hTransformMatrix;
	D3DXHANDLE m_hWorldInvTranspose;

	Material m_material;

	D3DXVECTOR3 m_position;
	D3DXVECTOR3 m_angle;
	D3DXVECTOR3 m_scale;

	void setMatrix(const D3DXMATRIX& pvm);
	void registerMaterial();
};


#endif