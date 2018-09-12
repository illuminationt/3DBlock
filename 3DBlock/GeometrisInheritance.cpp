#include "GeometrisInheritance.h"
#include"Graphics.h"
#include"Utility.h"
#include"Model.h"

void Sylinder::create(float radiusBottom, float radiusTop, float length, UINT slices, UINT stacks) {
	D3DXCreateCylinder(
		Graphics::D3DDev(),
		radiusBottom, radiusTop,
		length, slices, stacks,
		&m_pMesh, 0);
	m_radius = radiusBottom;
}

void Teapot::create() {
	D3DXCreateTeapot(Graphics::D3DDev(), &m_pMesh, 0);
}

void Torus::create(float innerRadius, float outerRadius, UINT sides, UINT rings) {
	HR(D3DXCreateTorus(
		Graphics::D3DDev(),
		innerRadius, outerRadius, sides, rings,
		&m_pMesh, 0));
}

void Sphere::create(float radius,UINT slices,UINT stacks) {
	D3DXCreateSphere(
		Graphics::D3DDev(),
		radius,
		slices,
		stacks,
		&m_pMesh, 0);
	m_radius = radius;
}

void Poly::create(float length,UINT sides) {
	HR(D3DXCreatePolygon(Graphics::D3DDev(), length, sides, &m_pMesh, 0));
}

void Cube::create(float x, float y, float z) {
	D3DXCreateBox(Graphics::D3DDev(), x, y, z, &m_pMesh, 0);
}