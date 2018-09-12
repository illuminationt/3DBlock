#include "Geometry.h"
#include"Graphics.h"


Geometry::Geometry() :
	m_pMesh(0),
	m_pEffect(0),
	m_hTransformMatrix(0),
	m_position(0.f, 0.f, 0.f),
	m_angle(0.f, 0.f, 0.f),
	m_scale(1.f, 1.f, 1.f)
{
}


Geometry::~Geometry()
{
}

void Geometry::initialize(ID3DXEffect* pEffect,const char* texFilename) {
	m_pEffect = pEffect;
	/*
	ID3DXBuffer* errors = 0;
	D3DXCreateEffectFromFile(Graphics::D3DDev(), "
	", 0, 0, D3DXSHADER_DEBUG, 0, &m_pEffect, &errors);
	if (errors) {
		warn((char*)errors->GetBufferPointer());
	}
	*/
	if (texFilename) {
		D3DXCreateTextureFromFile(Graphics::D3DDev(), texFilename, &m_texture);
	}
	

	m_hDiffuse = m_pEffect->GetParameterByName(0, "g_diffuseMtrl");
	m_hAmbient = m_pEffect->GetParameterByName(0, "g_ambientMtrl");
	m_hSpecular = m_pEffect->GetParameterByName(0, "g_specularMtrl");
	m_hSpecularPower = m_pEffect->GetParameterByName(0, "g_specularPower");
	m_hTex = m_pEffect->GetParameterByName(0, "g_tex");

	m_hWorld = m_pEffect->GetParameterByName(0, "g_world");
	m_hTransformMatrix = m_pEffect->GetParameterByName(0, "g_WVP");
	m_hWorldInvTranspose = m_pEffect->GetParameterByName(0, "g_worldInvTranspose");

}



void Geometry::draw(const D3DXMATRIX& pvm) {

	m_pEffect->SetTexture(m_hTex, m_texture);
	setMatrix(pvm);
	registerMaterial();

	m_pMesh->DrawSubset(0);
}
void Geometry::draw() {
	m_pMesh->DrawSubset(0);
}
void Geometry::drawShadow(const D3DXMATRIX& pvm) {
	
	Graphics::D3DDev()->SetRenderState(D3DRS_STENCILENABLE, true);
	Graphics::D3DDev()->SetRenderState(D3DRS_STENCILFUNC, D3DCMP_EQUAL);
	Graphics::D3DDev()->SetRenderState(D3DRS_STENCILREF, 0x0);
	Graphics::D3DDev()->SetRenderState(D3DRS_STENCILMASK, 0xffffffff);
	Graphics::D3DDev()->SetRenderState(D3DRS_STENCILWRITEMASK, 0xffffffff);
	Graphics::D3DDev()->SetRenderState(D3DRS_STENCILZFAIL, D3DSTENCILOP_KEEP);
	Graphics::D3DDev()->SetRenderState(D3DRS_STENCILFAIL, D3DSTENCILOP_KEEP);
	Graphics::D3DDev()->SetRenderState(D3DRS_STENCILPASS, D3DSTENCILOP_INCR);
	
	D3DXVECTOR4 light(0.577f, -0.577f, 0.577f, 0.0f);

	D3DXPLANE plane(0.f, -1.f, 0.f, 0.f);

	D3DXMATRIX S;
	D3DXMatrixShadow(&S, &light, &plane);

	D3DXMATRIX eps;
	D3DXMatrixTranslation(&eps, 0.f, 0.1f, 0.f);

	Graphics::D3DDev()->SetRenderState(D3DRS_ALPHABLENDENABLE, true);
	Graphics::D3DDev()->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	Graphics::D3DDev()->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

	setMatrix(S*eps*pvm);
	m_pMesh->DrawSubset(0);

	Graphics::D3DDev()->SetRenderState(D3DRS_ALPHABLENDENABLE, false);
	Graphics::D3DDev()->SetRenderState(D3DRS_STENCILENABLE, false);
}

void Geometry::createWorldMatrix(D3DXMATRIX* out) {
	D3DXMATRIX tmp;
	D3DXMatrixIdentity(out);

	D3DXMatrixScaling(out, m_scale.x, m_scale.y, m_scale.z);
	D3DXMatrixRotationYawPitchRoll(&tmp, D3DXToRadian(m_angle.y), D3DXToRadian(m_angle.x), D3DXToRadian(m_angle.z));
	*out *= tmp;

	D3DXMatrixTranslation(&tmp, m_position.x, m_position.y, m_position.z);
	*out *= tmp;

}
void Geometry::registerMaterial() {
	m_pEffect->SetValue(m_hDiffuse, &m_material.diffuse, sizeof(D3DXCOLOR));
	m_pEffect->SetValue(m_hAmbient, &m_material.ambient, sizeof(D3DXCOLOR));
	m_pEffect->SetValue(m_hSpecular, &m_material.specular, sizeof(D3DXCOLOR));
	m_pEffect->SetFloat(m_hSpecularPower, m_material.specularPower);
	m_pEffect->CommitChanges();
}
void Geometry::setMatrix(const D3DXMATRIX& pvm) {

	createWorldMatrix(&m_worldMatrix);
	m_pEffect->SetMatrix(m_hWorld, &m_worldMatrix);
	m_pEffect->SetMatrix(m_hTransformMatrix, &(m_worldMatrix*pvm));

	D3DXMATRIX worldInvTranspose;
	D3DXMatrixInverse(&worldInvTranspose, 0, &m_worldMatrix);
	D3DXMatrixTranspose(&worldInvTranspose, &worldInvTranspose);
	m_pEffect->SetMatrix(m_hWorldInvTranspose, &worldInvTranspose);

	m_pEffect->CommitChanges();
}

void Geometry::setPosition(const D3DXVECTOR3& a) { m_position = a; }
void Geometry::setPosition(float x, float y, float z) {
	m_position.x = x;
	m_position.y = y;
	m_position.z = z;
}
void Geometry::setAngle(const D3DXVECTOR3& a) { m_angle = a; }
void Geometry::setAngle(float x, float y, float z) {
	m_angle.x = x;
	m_angle.y = y;
	m_angle.z = z;
}

void Geometry::setScale(float ratio) {
	m_scale.x = m_scale.y = m_scale.z = ratio;
}

void Geometry::setScale(float x, float y, float z) {
	m_scale.x = x;
	m_scale.y = y;
	m_scale.z = z;
}

void Geometry::setMaterial(const D3DXCOLOR& d, const D3DXCOLOR& a, const D3DXCOLOR& s, float power) {
	m_material.diffuse = d;
	m_material.ambient = a;
	m_material.specular = s;
	m_material.specularPower = power;
}