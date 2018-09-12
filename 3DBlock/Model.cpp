#include "Model.h"
#include"Root.h"
#include"Graphics.h"
#include"Utility.h"
#include"Vertex.h"
//#include"ColliderTriangle.h"

Model::Model(const char* Xfilename) :
	m_position(0.f, 0.f, 0.f),
	m_angle(0.f, 0.f, 0.f),
	m_scale(1.f, 1.f, 1.f),
	m_mesh(0),
	m_meshMaterials(0),
	m_meshTextures(0),
	m_textureFilename(0),
	m_meshNumber(0),
	m_XFilename(Xfilename) {

	LPD3DXBUFFER mtrlBuffer = 0;
	LPDIRECT3DDEVICE9 d3dDev = Graphics::D3DDev();
	if (FAILED(D3DXLoadMeshFromX(
		Xfilename,
		D3DXMESH_SYSTEMMEM,
		d3dDev,
		NULL,
		&mtrlBuffer,
		NULL,
		&m_meshNumber,
		&m_mesh))) {
		warn("D3DXLoadMeshFromXで失敗");
	}


	//汎用バッファLD3DXBUFFERからマテリアル取得
	D3DXMATERIAL* materials = reinterpret_cast<D3DXMATERIAL*>(mtrlBuffer->GetBufferPointer());
	m_meshMaterials = new D3DMATERIAL9[m_meshNumber];
	m_meshTextures = new LPDIRECT3DTEXTURE9[m_meshNumber];
	m_textureFilename = new char*[m_meshNumber];

	//マテリアルデータ代入
	for (DWORD j = 0; j < m_meshNumber; j++) {
		m_meshMaterials[j] = materials[j].MatD3D;
		m_meshMaterials[j].Ambient = m_meshMaterials[j].Diffuse;

		m_textureFilename[j] = materials[j].pTextureFilename;
		//マテリアル毎のファイルの名前がちゃんと存在為てる
		if (m_textureFilename[j] &&
			lstrlen(m_textureFilename[j]) > 0) {
			//テクスチャ取得
			if (D3DXCreateTextureFromFile(
				Graphics::D3DDev(),
				materials[j].pTextureFilename,
				&m_meshTextures[j])) {
				warn("Mode::Modelでテクスチャ読み込み失敗");
			}
		}
	}
	//一時変数mtrlBufferを解放
	mtrlBuffer->Release();
}


Model::~Model() {
	SAFE_RELEASE(m_mesh);
	SAFE_DELETE_ARRAY(m_meshMaterials);
	SAFE_DELETE_ARRAY(m_meshTextures);
	
	SAFE_DELETE_ARRAY(m_textureFilename);
}


void Model::draw()const {
	D3DXMATRIXA16 wm, tm;
	D3DXMatrixIdentity(&wm);

	//スケーリング
	D3DXMatrixScaling(&wm, m_scale.x, m_scale.y, m_scale.z);

	//回転
	D3DXMatrixRotationYawPitchRoll(&tm, m_angle.y, m_angle.x, m_angle.z);
	D3DXMatrixMultiply(&wm, &wm, &tm);
	//平行移動
	D3DXMatrixTranslation(&tm, m_position.x, m_position.y, m_position.z);
	D3DXMatrixMultiply(&wm, &wm, &tm);

	LPDIRECT3DDEVICE9 d3dDev = Graphics::D3DDev();
	d3dDev->SetTransform(D3DTS_WORLD, &wm);

	//いざレンダリング
	for (DWORD j = 0; j < m_meshNumber; j++) {
		d3dDev->SetMaterial(&m_meshMaterials[j]);
		if (m_textureFilename[j]) {
			d3dDev->SetTexture(0, m_meshTextures[j]);
		}
		else {
			d3dDev->SetTexture(0, NULL);
		}
		m_mesh->DrawSubset(j);
	}
}




void Model::setPosition(const D3DXVECTOR3& a) {
	m_position = a;
}
void Model::setAngle(const D3DXVECTOR3& a) {
	m_angle = D3DXToRadian(a);
}
void Model::setScale(const D3DXVECTOR3& a) {
	m_scale = a;
}

const LPD3DXMESH Model::mesh()const { return m_mesh; };
DWORD Model::meshNumber()const { return m_meshNumber; };

/*
void Model::getVertices(ColliderTriangle* out) {
	//D3DVERTEXELEMENT9 e[MAX_FVF_DECL_SIZE];
	//m_mesh->GetDeclaration(e);

	WORD* indices = 0;
	m_mesh->LockIndexBuffer(0, (void**)&indices);
	m_mesh->UnlockIndexBuffer();

	VertexPNT* vertices = 0;
	m_mesh->LockVertexBuffer(0, (void**)&vertices);
	m_mesh->UnlockVertexBuffer();

	WORD triangleNum = m_mesh->GetNumFaces();
	for (WORD j = 0; j < triangleNum; j++) {
		WORD i0 = indices[j * 3 + 0];
		WORD i1 = indices[j * 3 + 1];
		WORD i2 = indices[j * 3 + 2];

		out[j].setPosition(
			vertices[i0].pos,
			vertices[i1].pos,
			vertices[i2].pos);
	}

	std::ostringstream oss;
	oss << "indices[0]=" << indices[0] << "," << indices[1] << "," << indices[2];
	warn(oss.str().c_str());
}

*/