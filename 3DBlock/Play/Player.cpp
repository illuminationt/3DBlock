#include "Player.h"
#include"Graphics.h"
#include"Input.h"
#include"Text.h"
#include"Const.h"

const float Player::m_speed = 18.f;

Player::Player():
	m_time(0),
	m_cube(0),
	size(4.f,1.f,0.5f)
{
	m_cube = new Cube();
	m_cube->initialize(m_effect);
	m_cube->setMaterial(D3DXCOLOR(0.1f, 0.4f, 0.4f, 1.f), D3DXCOLOR(0.f, 0.0f, 0.0f, 1.f), D3DXCOLOR(1.f, 1.f, 1.f, 1.f), 10.f);
	
	m_cube->create(size.x,size.y,size.z);
	
	m_position = D3DXVECTOR3(0.f, 0.f, 0.f);
}

Player::~Player() {
	SAFE_DELETE(m_cube);
}

const D3DXMATRIX* Player::invWorld() {
	
	D3DXMATRIX trans, rot, scale;
	D3DXMatrixTranslation(&trans, m_position.x, m_position.y, m_position.z);
	D3DXMatrixRotationYawPitchRoll(&rot, D3DXToRadian(m_angle.y), D3DXToRadian(m_angle.x), D3DXToRadian(m_angle.z));
	D3DXMatrixScaling(&scale, m_scale.x, m_scale.y, m_scale.z);

	m_invWorld = scale*rot*trans;
	D3DXMatrixInverse(&m_invWorld, 0, &m_invWorld);
	{
		D3DXMatrixInverse(&rot, 0, &rot);
		m_invAngle = rot;
	}
	return &m_invWorld;
}

const D3DXMATRIX* Player::invAngle() {
	return &m_invAngle;
}

void Player::update(float dt,Ball** out,bool isStarted) {
	Input* i = Input::instance();
	D3DXVECTOR3 move(0.f, 0.f, 0.f);
	if (i->isKeyDown('W')) {
		move.z++;
	}
	if (i->isKeyDown('A')) {
		move.x--;
	}
	if (i->isKeyDown('S')) {
		move.z--;
	}
	if (i->isKeyDown('D')) {
		move.x++;
	}
	if (!isStarted) {
		move.z = 0.f;
	}
	/*
	if (i->isKeyDown('E')) {
		move.y++;
	}
	if (i->isKeyDown('X')) {
		move.y--;
	}
	*/
	m_prevPos = m_position;
	m_position += move*m_speed*dt;

	if (m_position.x > 8.5f) {
		m_position.x = 8.5f;
	}
	else if (m_position.x < -8.5f) {
		m_position.x = -8.5f;
	}
	m_scale.x = 1.4f;

	m_velocity = m_position - m_prevPos;

	std::ostringstream oss;
	oss << "pos.x = " << m_position.x << ",pos.y = " << m_position.y << ", pos.z = " << m_position.z;
	Graphics::text()->draw(oss.str().c_str(), 0, 80);


	for(int j = 0; j < 3; j++) {
		D3DXVECTOR3 sub;
		if (isIntersect(out[j],&sub)) {
			/*
			D3DXVECTOR3 v = *out[j]->velocity();
			if (v.x>0||v.z>0) {
				out[j]->setPosition(*out[j]->position() + sub);
			}
			if (v.x < 0 || v.z < 0) {
				out[j]->setPosition(*out[j]->position() - sub);
			}
			*/
			D3DXVECTOR3 p = *out[j]->position();
			D3DXVECTOR3 v = m_velocity;
			out[j]->setPosition(D3DXVECTOR3(p.x,p.y,p.z+v.z*1.1f));
			//if (isIntersect(out[j])) {
				//out[j]->setPosition(*out[j]->position() - m_velocity*1.1f);
			//}
			D3DXVECTOR3 vel = *out[j]->velocity();
			vel += m_velocity;
			out[j]->setVelocity(vel);
			
		}
	}
}

void Player::drawGeometry(const D3DXMATRIX& pvm) {
	m_cube->draw(pvm);
	m_cube->setPosition(m_position);
	m_cube->setAngle(m_angle);
	m_cube->setScale(m_scale.x, 1.f, 1.f);
}

bool Player::isIntersect(const Ball* ball,D3DXVECTOR3* minSub)const {
	float xMin = m_position.x - (size.x / 2.f)*m_scale.x;
	float xMax = m_position.x + (size.x / 2.f)*m_scale.x;
	float zMin = m_position.z - (size.z / 2.f)*m_scale.z;
	float zMax = m_position.z + (size.z / 2.f)*m_scale.z;

	D3DXVECTOR3 ballPos = *ball->position();
	float ballXMin = ballPos.x - ball->radius();
	float ballXMax = ballPos.x + ball->radius();
	float ballZMin = ballPos.z - ball->radius();
	float ballZMax = ballPos.z + ball->radius();

	//‚Ü‚¸x•ûŒü
	if (ballXMin+EPSILON < xMax&&xMin+EPSILON < ballXMax) {
		if (ballZMin+EPSILON < zMax&&zMin +EPSILON< ballZMax) {
			
			float xSub = min(fabs(ballXMin - xMax), fabs(xMin - ballXMax));
			float zSub = min(fabs(ballZMin - zMax), fabs(zMin - ballZMax));

			if (xSub < zSub) {
				
				*minSub = D3DXVECTOR3(xSub, 0.f, 0.f);
			}
			else {
				*minSub = D3DXVECTOR3(0.f, 0.f, zSub);
			}
			return true;
		}
	}
	return false;
}

void Player::refrectBall(D3DXVECTOR3* out) {
	D3DXVECTOR3 dir;
	D3DXVec3Normalize(&dir, &m_velocity);

	D3DXVECTOR3 t = (*out + m_velocity) / 2.f;
	out->z = t.z;
}