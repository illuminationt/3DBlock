#ifndef MYCAMERA_H
#define MYCAMERA_H

#include<d3dx9.h>

class MyCamera
{
public:
	MyCamera();
	~MyCamera();

	void setPosition(const D3DXVECTOR3&);
	void setTarget(const D3DXVECTOR3&);

	//�e�p�����[�^�ύX�B�ύX�������Ȃ��p�����^�ɂ�0���w�肵�Ă�������
	void setLens(float fovy, float nearZ, float farZ);

	//�e�X�g�p�J�����ړ��֐�
	void testMove();

	const D3DXVECTOR3* position();
	void createProjMatrix(D3DXMATRIX* out);
private:
	D3DXVECTOR3 m_position;
	D3DXVECTOR3 m_target;

	float m_fovy;
	float m_nearClip;
	float m_farClip;

};


#endif