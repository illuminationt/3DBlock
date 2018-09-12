#ifndef BALL_H
#define BALL_H

#include"ObjectBase.h"
#include"Vertex.h"

class Sphere;
class Sylinder;
class Block;
class Player;
class Ball:public ObjectBase
{
public:
	Ball();
	virtual ~Ball();

	void update(float dt);

	void reflectWall(ID3DXMesh*,VertexType,const D3DXVECTOR3* recievedVel=0,const D3DXMATRIX* invWorld=0,const D3DXMATRIX* invAngle=0,const Player* player=0);
	//è’ìÀÇµÇΩÇÁtrue
	bool reflectSylinder(Block*,unsigned* scoreOut,int* IDout );
	void reflectPlayer(const Player*);

	const D3DXVECTOR3* position()const { return &m_position; }
	void setVelocity(const D3DXVECTOR3& v) { m_velocity = v; }
	const D3DXVECTOR3* velocity()const { return &m_velocity; }
	float radius()const { return m_radius; }
private:
	virtual void drawGeometry(const D3DXMATRIX&);
	Sphere* m_sphere;
	
	//ãtÉèÅ[ÉãÉhïœä∑(D3DXIntersectÇÃÇΩÇﬂÅj
	//void invWorldTranslation(D3DXVECTOR3* out,const D3DXMATRIX* invWorld);
	const ID3DXMesh* getInvWolrdMesh(ID3DXMesh*);
private:
	D3DXVECTOR3 m_velocity;
	float m_radius;

	static const float m_speed;
	static const float m_getSpeedFromPlayerRate;
	static const float m_correctDirThreshold;
};


#endif

