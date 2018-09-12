#ifndef OBJECTBASE_H
#define OBJECTBASE_H

#include<d3dx9.h>

class Geometry;

/*
���̊��N���X���S�����ƁF
Geometry.fx�V�F�[�_��p����D3DXCreate�n�̃��b�V��������
�E�V�F�[�_�̃G�t�F�N�g�p�����[�^�ւ̃n���h���擾
�EID3DXEffect�C���^�[�t�F�C�X�����擾�Ȃ�擾
�E�ˉe�s��A���̏����������draw



�h���N���X�����ׂ�����
�E�R���X�g���N�^�̃I�[�o�[���[�h
�E�g�������W�I���g����private�錾
�E�e�X�̃R���X�g���N�^��Geometry��new,initialize,setMaterial,create
�Eupdate�֐���Geometry��Pos,Angle�Ȃǂ�set
*/


class ObjectBase
{
public:
	ObjectBase();
	virtual ~ObjectBase();

	void update();
	
	//pointLightPosW,dirToSunW��0���w�肷��Ɩ���
	void draw(const D3DXMATRIX& pvm,
		const D3DXVECTOR3& eyePosW,
		const D3DXVECTOR3* pointLightPosW=0,
		const D3DXVECTOR3* pointLightPosW2 = 0,
		const D3DXVECTOR3* pointLightPosW3 = 0,
		const D3DXVECTOR3& dirToSunW = D3DXVECTOR3(0.f,1.f,0.f),
		const D3DXVECTOR4& diffuseLight = D3DXVECTOR4(1.f, 1.f, 1.f, 1.f),
		const D3DXVECTOR4& ambientLight = D3DXVECTOR4(1.f, 1.f, 1.f, 1.f),
		const D3DXVECTOR4& specularLight = D3DXVECTOR4(1.f, 1.f, 1.f, 1.f));

	void setPosition(const D3DXVECTOR3&);

	bool isAlive()const { return m_isAlive; }
	virtual void rebirth();
	virtual void destroy();

	void onLostDevice() { m_effect->OnLostDevice(); }
	void onResetDevice() { m_effect->OnResetDevice(); }
private:
	//draw�����Ŏg����
	virtual void drawGeometry(const D3DXMATRIX&) = 0;
protected:
	static ID3DXEffect* m_effect;
protected:
	D3DXVECTOR3 m_position;
	D3DXVECTOR3 m_angle;
	D3DXVECTOR3 m_scale;

protected:
	bool m_isAlive;
private:
	D3DXHANDLE m_hTech;

	D3DXHANDLE m_hDiffuseLight;
	D3DXHANDLE m_hAmbientLight;
	D3DXHANDLE m_hSpecularLight;


	D3DXHANDLE m_hDirToSunW;
	D3DXHANDLE m_hPointLightPosW;
	D3DXHANDLE m_hPointLightPosW2;
	D3DXHANDLE m_hPointLightPosW3;
	D3DXHANDLE m_hPointLightAttenuation;
	D3DXHANDLE m_hSpecularAttenuation;
	D3DXHANDLE m_hEyePosW;

};


#endif