#ifndef BLOCKSDATA_H
#define BLOCKSDATA_H

#include"Block.h"

/*
各々のクラスですべきこと：
m_sylinder->setMaterial,
m_sylinder->create
*/

//0
class BlockDefault :public Block {
public:
	BlockDefault();
};

//1
class BlockBig :public Block {
public:
	BlockBig();
};
//2
class BlockDRed :public Block{
public:
	BlockDRed();
};
//3
class BlockLong :public Block {
public:
	BlockLong();
};
//4
class BlockBall :public Block {
public:
	BlockBall();
	virtual void executeAfterHit();
private:
};
//5
class BlockMoveRound :public Block {
public:
	BlockMoveRound();
	virtual void executeBeforeHit();


private:
	float m_count;
	D3DXVECTOR3 initPos;
	
	float m_roundRadius;
	float m_angularSpeed;
	
};

//6:
class BlockRainbow :public Block {
public:
	BlockRainbow();
	virtual void executeBeforeHit();
private:
	float m_count;
};

#endif
