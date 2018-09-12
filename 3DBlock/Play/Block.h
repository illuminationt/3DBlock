#ifndef BLOCK_H
#define BLOCK_H

#include<d3dx9.h>
#include"GeometrisInheritance.h"
#include"ObjectBase.h"

class Block:public ObjectBase
{
public:
	Block();
	virtual ~Block();

	virtual void executeBeforeHit() {};
	virtual void executeAfterHit() {};

	virtual void destroy();

	const Sylinder* sylinder()const { return m_sylinder; }
	void setCSVposition(int p) { m_CSVposition = p; }
	int CSVposition()const { return m_CSVposition; }
	bool isAlive()const { return m_isAlive; }
	unsigned score()const { return m_score; }
	int ID()const { return m_ID; }
protected:
	unsigned m_score;
	int m_ID;

protected:
	virtual void drawGeometry(const D3DXMATRIX&);
	Sylinder* m_sylinder;

	//CSVファイル内で何番目だったか？
	int m_CSVposition;
};


#endif