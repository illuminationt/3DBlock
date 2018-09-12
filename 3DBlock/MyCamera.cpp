#include "MyCamera.h"
#include"Const.h"
#include"Input.h"

MyCamera::MyCamera() :
	m_position(0.f, 0.f, -10.f),
	m_target(0.f, 0.f, 1.f),
	m_fovy(D3DX_PI*0.25f),
	m_nearClip(1.f),
	m_farClip(1000.f)
{
}


MyCamera::~MyCamera()
{
}

void MyCamera::setPosition(const D3DXVECTOR3& a) {
	m_position = a;
}
void MyCamera::setTarget(const D3DXVECTOR3& a) {
	m_target = a;
}

void MyCamera::setLens(float fovy, float nearZ, float farZ) {
	if (fovy != 0) {
		m_fovy = fovy;
	}
	if (nearZ != 0) {
		m_nearClip = nearZ;
	}
	if (farZ != 0) {
		m_farClip = farZ;
	}
}

void MyCamera::testMove() {
	Input* i = Input::instance();

	if (i->isKeyDown('I')) {
		m_position.z++;
	}
	if (i->isKeyDown('J')) {
		m_position.x--;
	}
	if (i->isKeyDown('K')) {
		m_position.z--;
	}
	if (i->isKeyDown('L')) {
		m_position.x++;
	}
	if (i->isKeyDown('U')) {
		m_position.y++;
	}
	if (i->isKeyDown('N')) {
		m_position.y--;
	}
}

const D3DXVECTOR3* MyCamera::position() { return &m_position; }

void MyCamera::createProjMatrix(D3DXMATRIX* out) {
	D3DXMATRIX v, p;
	float aspect = (float)WINDOW_WIDTH / (float)WINDOW_HEIGHT;
	D3DXMatrixLookAtLH(&v, &m_position, &m_target, &D3DXVECTOR3(0.f, 1.f, 0.f));
	D3DXMatrixPerspectiveFovLH(&p, m_fovy, aspect, m_nearClip, m_farClip);

	(*out) = v*p;
}