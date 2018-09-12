#include "Play/Frame.h"
#include"Graphics.h"
#include"Utility.h"
#include"Model.h"
//#include"ColliderTriangle.h"
Frame::Frame()
{
	m_model = new Model("BrockKuzushi_Frame_forMultiplePolygonImproved.x");

	ID3DXBuffer* errors;
	D3DXCreateEffectFromFile(Graphics::D3DDev(), "fx/stage_fineShadow.fx", 0, 0,
		D3DXSHADER_DEBUG, 0, &m_effect, &errors);
	if (errors) {
		warn(reinterpret_cast<char*>(errors->GetBufferPointer()));
	}

	m_tex = m_model->texture()[0];

	m_hTechnique = m_effect->GetTechnique(0);
	m_hTime = m_effect->GetParameterByName(0, "g_time");
	m_hWVP = m_effect->GetParameterByName(0, "g_WVP");
	m_hPointLightPosW = m_effect->GetParameterByName(0, "g_pointLightPosW");
	m_hPointLightPosW2 = m_effect->GetParameterByName(0, "g_pointLightPosW2");
	m_hPointLightPosW3 = m_effect->GetParameterByName(0, "g_pointLightPosW3");
	m_hPointLightAttenuation = m_effect->GetParameterByName(0, "g_pointLightAttenuation");
	m_hSpecularAttenuation = m_effect->GetParameterByName(0, "g_specularAttenuation");
	m_hTex = m_effect->GetParameterByName(0, "g_tex");

}


Frame::~Frame()
{
	SAFE_DELETE(m_model);
}

void Frame::draw(const D3DXMATRIX& pvm,
	const D3DXVECTOR3& pointLightPos,
	const D3DXVECTOR3& pointLightPos2,
	const D3DXVECTOR3& pointLightPos3) {
	//moveSun();
	//m_time += 0.01f;
	//m_effect->SetFloat(m_hTime, m_time);
	m_effect->SetMatrix(m_hWVP, &pvm);
	m_effect->SetValue(m_hPointLightPosW, &pointLightPos, sizeof(D3DXVECTOR3));
	{
		m_effect->SetValue(m_hPointLightPosW2, &pointLightPos2, sizeof(D3DXVECTOR3));
		m_effect->SetValue(m_hPointLightPosW3, &pointLightPos3, sizeof(D3DXVECTOR3));
		m_effect->SetValue(m_hPointLightAttenuation, &D3DXVECTOR3(0.3f, 0.3f, 0.3f), sizeof(D3DXVECTOR3));
		m_effect->SetValue(m_hSpecularAttenuation, &D3DXVECTOR3(0.2f, 0.2f, 0.2f), sizeof(D3DXVECTOR3));
	}
	
	m_effect->SetTexture(m_hTex, m_tex);
	m_effect->SetTechnique(m_hTechnique);

	m_effect->CommitChanges();
	//•`‰æŠJŽn
	UINT numPasses = 0;
	m_effect->Begin(&numPasses, 0);
	for (UINT j = 0; j < numPasses; j++) {
		m_effect->BeginPass(j);
		m_model->draw();
		m_effect->EndPass();
	}
	m_effect->End();
}

