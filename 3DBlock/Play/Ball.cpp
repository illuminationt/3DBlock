#include "Ball.h"
#include"GeometrisInheritance.h"
#include"Const.h"
#include"Vertex.h"
#include"Block.h"
#include"Graphics.h"
#include"Play/Player.h"

const float Ball::m_speed = 18.f;
const float Ball::m_getSpeedFromPlayerRate = 30.f;
const float Ball::m_correctDirThreshold = 0.1f;


Ball::Ball():ObjectBase(),
m_velocity(15.f,0.f,10.f)
{
	m_sphere = new Sphere();
	m_sphere->initialize(m_effect);
	m_sphere->setMaterial(D3DXCOLOR(0.8f, 0.6f, 0.2f, 1.f), D3DXCOLOR(0.3f, 0.2f, 0.f, 1.f), D3DXCOLOR(1.f, 1.f, 1.f, 1.f), 10.f);
	m_sphere->create(BALL_RADIUS, 50, 50);
	m_radius = BALL_RADIUS;

	destroy();

}


Ball::~Ball()
{
	SAFE_DELETE(m_sphere);
}

void Ball::update(float dt) {
	m_position += m_velocity*dt;

	D3DXVECTOR3 dir;
	D3DXVec3Normalize(&dir, &m_velocity);
	m_velocity = dir*m_speed;

	//壁に垂直、水平になったときの補正
	if (abs(dir.x) < m_correctDirThreshold) {
		m_velocity.x *= 2.f;
	}
	if (abs(dir.z) < m_correctDirThreshold) {
		m_velocity.z *= 2.f;
	}

	//落ちたら死ぬ
	if (m_position.z < -18.f) {
		destroy();
	}
}

void Ball::reflectWall(ID3DXMesh* mesh,VertexType vertex,const D3DXVECTOR3* recieveVel,const D3DXMATRIX* invWorld,const D3DXMATRIX* invAngle,const Player* player) {
	if (!m_isAlive) {
		return;
	}
	D3DXVECTOR3 dir;
	D3DXVec3Normalize(&dir, &m_velocity);

	BOOL isHit = false;
	DWORD faceIndex;
	float dist = FLT_MAX;
	if (invWorld) {
		D3DXVECTOR3 invPosition = m_position, invDir = dir;
		D3DXVec3TransformCoord(&invPosition, &m_position, invWorld);
		D3DXVec3TransformCoord(&invDir, &dir, invAngle);
		D3DXIntersect(mesh, &invPosition, &invDir, &isHit, &faceIndex, 0, 0, &dist, 0, 0);

		//if (player->isIntersect(this)) {
			//isHit = true;
			//dist = 0.f;
		//}
		//else {
			//isHit = false;
		//}
	}
	else {
		D3DXIntersect(mesh, &m_position, &dir, &isHit, &faceIndex, 0, 0, &dist, 0, 0);
	}
	
	if (isHit) {
		if (dist < m_radius) {
			WORD* i = 0;
			mesh->LockIndexBuffer(D3DLOCK_READONLY, (void**)&i);
			WORD i0, i1, i2;
			i0 = i[faceIndex * 3 + 0];
			i1 = i[faceIndex * 3 + 1];
			i2 = i[faceIndex * 3 + 2];
			mesh->UnlockIndexBuffer();
			//この頂点がフレームとプレイヤーで違うのが諸悪の根源
			D3DXVECTOR3 p0, p1, p2;
			VertexPN* vPN = 0;
			VertexPNT* vPNT = 0;
			switch (vertex) {
				
			case VertexType::PN: {

				mesh->LockVertexBuffer(D3DLOCK_READONLY, (void**)&vPN);
				p0 = vPN[i0].pos;
				p1 = vPN[i1].pos;
				p2 = vPN[i2].pos;
				mesh->UnlockVertexBuffer();
			}
				break;
			
			case VertexType::PNT: {

				mesh->LockVertexBuffer(D3DLOCK_READONLY, (void**)&vPNT);
				p0 = vPNT[i0].pos;
				p1 = vPNT[i1].pos;
				p2 = vPNT[i2].pos;
				mesh->UnlockVertexBuffer();
			}
				break;
			
			}
			

			D3DXVECTOR3 n, p01, p02;
			p01 = p1 - p0;
			p02 = p2 - p0;
			D3DXVec3Cross(&n, &p01, &p02);
			D3DXVec3Normalize(&n,&n);

			float dot = D3DXVec3Dot(&m_velocity, &n);
			m_velocity = m_velocity - 2.f*dot*n;

			if (recieveVel) {
				m_velocity += (*recieveVel)*m_getSpeedFromPlayerRate;
			}
		}
	}
}

bool Ball::reflectSylinder(Block* block,unsigned* scoreOut,int* IDout) {
	if (!block->isAlive()) {
		return false;
	}
	if (!m_isAlive) {
		return false;
	}
	const Sylinder* syl = block->sylinder();
	float radiusSumSq = syl->radius() + m_radius;
	radiusSumSq *= radiusSumSq;
	
	D3DXVECTOR3 d = m_position - *syl->position();
	float distSq = d.x*d.x + d.z*d.z;

	if (radiusSumSq > distSq) {
		D3DXVECTOR3 dir;
		D3DXVec3Normalize(&dir,&dir);
		D3DXVECTOR3 collidePoint = m_position + dir*m_radius;
		D3DXVECTOR3 n = collidePoint - *syl->position();
		D3DXVec3Normalize(&n, &n);

		float dot = D3DXVec3Dot(&m_velocity, &n);
		m_velocity = m_velocity - 2.f*dot*n;
		m_velocity.y = 0.f;
		block->destroy();
		*scoreOut = block->score();
		*IDout = block->ID();
		return true;
	}
	return false;
}

void Ball::reflectPlayer(const Player* player) {
	//if (player->isIntersect(this)) {

	//}
}

void Ball::drawGeometry(const D3DXMATRIX& pvm) {
	m_sphere->draw(pvm);
	m_sphere->setPosition(m_position);
}






