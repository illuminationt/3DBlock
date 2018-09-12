#include "Block.h"
#include"Const.h"
#include"Graphics.h"
#include"Const.h"

Block::Block() :ObjectBase(),
m_score(1),
m_ID(-1),
m_sylinder(0),
m_CSVposition(-1)
{
	m_sylinder = new Sylinder();
	m_sylinder->initialize(m_effect);
	m_sylinder->setMaterial(D3DXCOLOR(0.f, 0.8f, 0.8f, 1.f), D3DXCOLOR(0.f, 0.05f, 0.05f, 1.f), D3DXCOLOR(1.f, 1.f, 1.f, 1.f), 10.f);
	m_sylinder->create(BLOCK_RADIUS, BLOCK_RADIUS, BLOCK_HEIGHT, 100, 10);
	m_sylinder->setAngle(D3DXVECTOR3(90.f, 0.f, 90.f));
}


Block::~Block()
{
	SAFE_DELETE(m_sylinder);
}

void Block::drawGeometry(const D3DXMATRIX& pvm) {
	if (!m_isAlive) {
		return;
	}
	executeBeforeHit();
	m_sylinder->draw(pvm);
	m_sylinder->setPosition(m_position);
}

void Block::destroy() {
	ObjectBase::destroy();
	m_position = D3DXVECTOR3(FLT_MAX, FLT_MAX, FLT_MAX);
}