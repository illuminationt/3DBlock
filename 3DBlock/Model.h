#ifndef MODEL_H
#define MODEL_H

#include<d3d9.h>
#include<d3dx9.h>
#include<iostream>
class ColliderTriangle;

class Model
{
public:
	explicit Model(const char* Xfilename);
	~Model();

	//�����Ń��[���h�s������
	void draw()const;

	void setPosition(const D3DXVECTOR3&);
	void setAngle(const D3DXVECTOR3&);
	void setScale(const D3DXVECTOR3&);

	const LPD3DXMESH mesh()const;
	DWORD meshNumber()const;

	//void getVertices(ColliderTriangle* out);
	const LPDIRECT3DTEXTURE9* texture()const { return m_meshTextures; }
private:
	D3DXVECTOR3 m_position;
	D3DXVECTOR3 m_angle;
	D3DXVECTOR3 m_scale;

	LPD3DXMESH m_mesh;
	D3DMATERIAL9* m_meshMaterials;//�}�e���A���̔z��
	LPDIRECT3DTEXTURE9* m_meshTextures;//�e�N�X�`���̔z��
	char** m_textureFilename;
	DWORD m_meshNumber;

	const char* m_XFilename;
};



#endif