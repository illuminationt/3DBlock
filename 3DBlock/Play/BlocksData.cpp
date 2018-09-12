#include"BlocksData.h"
#include"Const.h"
#include"Utility.h"


BlockDefault::BlockDefault():Block() {
	m_sylinder->setMaterial(D3DXCOLOR(0.f, 0.8f, 0.8f, 1.f), D3DXCOLOR(0.f, 0.05f, 0.05f, 1.f), D3DXCOLOR(1.f, 1.f, 1.f, 1.f), 10.f);
	m_sylinder->create(BLOCK_RADIUS, BLOCK_RADIUS, BLOCK_HEIGHT, 100, 10);

	m_ID = 0;
}

BlockBig::BlockBig() : Block() {
	m_sylinder->setMaterial(D3DXCOLOR(0.f, 0.8f, 0.8f, 1.f), D3DXCOLOR(0.f, 0.05f, 0.05f, 1.f), D3DXCOLOR(1.f, 1.f, 1.f, 1.f), 10.f);
	m_sylinder->create(BLOCK_RADIUS*2.f, BLOCK_RADIUS*2.f, BLOCK_HEIGHT, 100, 10);

	m_ID = 1;
	m_score = 2;
}

BlockDRed::BlockDRed() : Block() {
	m_sylinder->setMaterial(D3DXCOLOR(1.f, 0.2f, 0.2f, 1.f), D3DXCOLOR(0.02f, 0.f, 0.f, 1.f), D3DXCOLOR(1.f, 1.f, 1.f, 1.f), 10.f);
	m_sylinder->create(BLOCK_RADIUS, BLOCK_RADIUS, BLOCK_HEIGHT, 100, 10);

	m_ID = 2;
}

BlockLong::BlockLong() : Block() {
	m_sylinder->setMaterial(D3DXCOLOR(0.0f, 0.8f, 0.8f, 1.f), D3DXCOLOR(0.02f, 0.f, 0.f, 1.f), D3DXCOLOR(1.f, 1.f, 1.f, 1.f), 10.f);
	m_sylinder->create(BLOCK_RADIUS, BLOCK_RADIUS, BLOCK_HEIGHT*4, 100, 10);

	m_ID = 3;
	m_score = 2;
}

BlockBall::BlockBall() :Block() {
	m_sylinder->setMaterial(D3DXCOLOR(1.f, 1.f, 0.f, 1.f), D3DXCOLOR(0.02f, 0.f, 0.f, 1.f), D3DXCOLOR(1.f, 1.f, 0.f, 1.f), 10.f);
	m_sylinder->create(BLOCK_RADIUS, BLOCK_RADIUS, BLOCK_HEIGHT, 100, 10);

	m_ID = 4;
}
void BlockBall::executeAfterHit() {
	
}




BlockMoveRound::BlockMoveRound():Block(),
m_count(0), 
initPos(0.f,0.f,0.f){
	m_sylinder->setMaterial(D3DXCOLOR(1.f, 0.2f, 0.4f, 1.f), D3DXCOLOR(0.02f, 0.f, 0.f, 1.f), D3DXCOLOR(1.f, 1.f, 1.f, 1.f), 10.f);
	m_sylinder->create(BLOCK_RADIUS, BLOCK_RADIUS, BLOCK_HEIGHT, 100, 10);

	m_ID = 5;
	m_score = 3;

	float r = random(1, 100);
	m_roundRadius = r / 33.f;
	m_angularSpeed = (r - 50.f) / 50.f;
}
void BlockMoveRound::executeBeforeHit() {
	m_count += 0.1f;

	if (initPos == D3DXVECTOR3(0.f, 0.f, 0.f)) {
		initPos = m_position;
	}

	float t = m_count*m_angularSpeed;
	m_position.x = initPos.x+m_roundRadius*cos(t);
	m_position.z = initPos.z+m_roundRadius*sin(t);
}

BlockRainbow::BlockRainbow():Block(),m_count(0.f) {
	m_sylinder->setMaterial(D3DXCOLOR(1.f, 0.2f, 0.4f, 1.f), D3DXCOLOR(0.02f, 0.f, 0.f, 1.f), D3DXCOLOR(1.f, 1.f, 1.f, 1.f), 10.f);
	m_sylinder->create(BLOCK_RADIUS*2.f, BLOCK_RADIUS*2.f, BLOCK_HEIGHT*4.f, 100, 10);

	m_ID = 6;
	m_score = 5;
}

void BlockRainbow::executeBeforeHit() {
	m_count += 0.02f;
	float r = cos(m_count)*sin(m_count);
	float g = cos(m_count)*cos(m_count);
	float b = sin(m_count);
	m_sylinder->setMaterial(D3DXCOLOR(r, g,b, 1.f), D3DXCOLOR(0.02f, 0.f, 0.f, 1.f), D3DXCOLOR(r*2.f, g*2.f, b*2.f, 1.f), 10.f);

}