#ifndef PLAYER_H
#define PLAYER_H
#include<d3dx9.h>
#include"Geometry.h"
#include"GeometrisInheritance.h"
#include"ObjectBase.h"
#include"Vertex.h"
#include"Play/Ball.h"

class Cube;
class Player:public ObjectBase
{
public:
	Player();
	virtual ~Player();

	void update(float dt,Ball** out,bool isStarted);
	bool isIntersect(const Ball* ball,D3DXVECTOR3* minSub=0)const;
	const D3DXMATRIX* invWorld();
	const D3DXMATRIX* invAngle();
	const D3DXVECTOR3* position() { return &m_position; }
	VertexType vertex() { return VertexType::PN; }
	const D3DXVECTOR3* velocity() { return &m_velocity; }
	ID3DXMesh* mesh() { return m_cube->mesh(); }
private:
	virtual void drawGeometry(const D3DXMATRIX&);
	
	void refrectBall(D3DXVECTOR3* outVel);
	Cube* m_cube;
	D3DXMATRIX m_invWorld;
	D3DXMATRIX m_invAngle;

	D3DXVECTOR3 m_prevPos;
	D3DXVECTOR3 m_velocity;

	D3DXVECTOR3 size;

	unsigned m_time;


	static const float m_speed;
};


#endif