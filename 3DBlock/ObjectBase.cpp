#include "ObjectBase.h"
#include"Geometry.h"
#include"Graphics.h"

ID3DXEffect* ObjectBase::m_effect = 0;

ObjectBase::ObjectBase() :
	m_position(0.f, 0.f, 0.f),
	m_angle(0.f, 0.f, 0.f),
	m_scale(1.f, 1.f, 1.f),
	m_isAlive(true)
{
	//Å‰‚Éì‚ç‚ê‚éBlock‚ªm_effect‚Ì‰Šú‰»‚ğ¿‚¯•‰‚¤
	if (!m_effect) {
		ID3DXBuffer* errors = 0;
		D3DXCreateEffectFromFile(Graphics::D3DDev(), "fx/geometry_fineShadow.fx", 0, 0, D3DXSHADER_DEBUG, 0, &m_effect, &errors);
		if (errors) {
			warn(reinterpret_cast<char*>(errors->GetBufferPointer()));
		}
	}

	m_hTech = m_effect->GetTechniqueByName("GeometryTech");

	m_hDiffuseLight = m_effect->GetParameterByName(0, "g_diffuseLight");
	m_hAmbientLight = m_effect->GetParameterByName(0, "g_ambientLight");
	m_hSpecularLight = m_effect->GetParameterByName(0, "g_specularLight");

	m_hDirToSunW = m_effect->GetParameterByName(0, "g_dirToSunW");
	m_hPointLightPosW = m_effect->GetParameterByName(0, "g_pointLightPosW");
	m_hPointLightPosW2 = m_effect->GetParameterByName(0, "g_pointLightPosW2");
	m_hPointLightPosW3 = m_effect->GetParameterByName(0, "g_pointLightPosW3");
	m_hPointLightAttenuation = m_effect->GetParameterByName(0, "g_pointLightAttenuation");
	m_hSpecularAttenuation = m_effect->GetParameterByName(0, "g_specularAttenuation");
	m_hEyePosW = m_effect->GetParameterByName(0, "g_eyePosW");

}


ObjectBase::~ObjectBase()
{
}

void ObjectBase::update() {

}


void ObjectBase::draw(const D3DXMATRIX& pvm,
	const D3DXVECTOR3& eyePosW,
	const D3DXVECTOR3* pointLightPosW,
	const D3DXVECTOR3* pointLightPosW2,
	const D3DXVECTOR3* pointLightPosW3,
	const D3DXVECTOR3& dirToSunW,
	const D3DXVECTOR4& diffuseLight,
	const D3DXVECTOR4& ambientLight,
	const D3DXVECTOR4& specularLight) {
	if (!m_isAlive) {
		return;
	}

	m_effect->SetVector(m_hDiffuseLight, &diffuseLight);
	m_effect->SetVector(m_hAmbientLight, &ambientLight);
	m_effect->SetVector(m_hSpecularLight, &specularLight);

	if (dirToSunW) {
		m_effect->SetValue(m_hDirToSunW, &dirToSunW, sizeof(D3DXVECTOR3));
	}
	if (pointLightPosW) {
		m_effect->SetValue(m_hPointLightPosW, pointLightPosW, sizeof(D3DXVECTOR3));
		if (!pointLightPosW2) {
			warn("you dont put pointLightPosW in draw()");
		}
		m_effect->SetValue(m_hPointLightPosW2, pointLightPosW2, sizeof(D3DXVECTOR3));
		m_effect->SetValue(m_hPointLightPosW3, pointLightPosW3, sizeof(D3DXVECTOR3));
		m_effect->SetValue(m_hPointLightAttenuation, &D3DXVECTOR3(0.6f, 0.6f, 0.6f), sizeof(D3DXVECTOR3));
		m_effect->SetValue(m_hSpecularAttenuation, &D3DXVECTOR3(0.1f, 0.1f, 0.1f), sizeof(D3DXVECTOR3));
	}
	
	m_effect->SetValue(m_hEyePosW, &eyePosW, sizeof(D3DXVECTOR3));
	
	
	m_effect->CommitChanges();

	UINT numPasses = 0;
	m_effect->Begin(&numPasses, 0);
	for (UINT j = 0; j < numPasses; j++) {
		m_effect->BeginPass(j);

		drawGeometry(pvm);
		
		m_effect->EndPass();
	}

	m_effect->End();
}

void ObjectBase::rebirth() { m_isAlive = true; }
void ObjectBase::destroy() {
	m_isAlive = false;
	m_position = D3DXVECTOR3(10000.f, 10000.f, 10000.f);
}

void ObjectBase::setPosition(const D3DXVECTOR3& a) { m_position = a; }