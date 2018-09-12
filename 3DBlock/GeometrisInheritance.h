#ifndef GEOMETRIESINHERITANCE_H
#define GEOMETRIESINHERITANCE_H

#include"Geometry.h"
class Model;
class Sylinder :public Geometry {
public:
	void create(float radiusBottom, float radiusTop, float length, UINT slices, UINT stacks);
	float radius()const { return m_radius; }
private:
	float m_radius;
};

class Teapot :public Geometry {
public:
	void create();
};

class Torus :public Geometry {
public:
	void create(float innedRadius, float outerRadius, UINT sides, UINT rings);
};

class Sphere : public Geometry{
public:
	void create(float radius,UINT slices,UINT stacks);
	float radius() { return m_radius; }
private:
	float m_radius;

};


class Poly :public Geometry {
public:
	void create(float length, UINT sides);
};

class Cube :public Geometry {
public:
	void create(float x, float y, float z);

};

#endif