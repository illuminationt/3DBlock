#ifndef ROOT_H
#define ROOT_H

#include"AppBase.h"
#include"Sequence/Parent.h"

class Root:public AppBase
{
public:
	Root();
	~Root();

	virtual void update();
	virtual bool checkDevCaps();

	virtual void initialize(HWND hWnd);
	
private:
	Sequence::Parent* m_parent;
};

#endif