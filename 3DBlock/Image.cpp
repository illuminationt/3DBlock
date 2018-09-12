#include"Vertex.h"
#include"Image.h"
#include"Const.h"
#include"Graphics.h"
#include"Utility.h"


VertexPT* Image::m_vertexPT = 0;

Image::Image(const char* filename):
m_effect(0),
m_texture(0),
m_position(0.f,0.f),
m_scale(1.f,1.f),
m_angleZ(0.f),
m_size(1.f,1.f),
m_UVsize(1.f,1.f),
m_UVoffset(0.f,0.f),
m_color(1.f,1.f,1.f,1.f){
	ID3DXBuffer* error = 0;
	D3DXCreateEffectFromFile(Graphics::D3DDev(), "fx/image.fx", 0, 0, 0, 0, &m_effect, &error);
	if (error) {
		warn(reinterpret_cast<char*>(error->GetBufferPointer()));
	}

	if (FAILED(D3DXCreateTextureFromFile(Graphics::D3DDev(), filename, &m_texture))) {
		warn("テクスチャ読み込み失敗");
	}
	
	

	{
		float asp = (float)WINDOW_HEIGHT / (float)WINDOW_WIDTH;
		m_projFixed = D3DXMATRIX(asp, 0.f, 0.f, 0.f,
			0.f, -1.f, 0.f, 0.f,
			0.f, 0.f, 1.f, 0.f,
			-1.f, 1.f, 0.f, 1.f);
		m_projChange = D3DXMATRIX(2.f / (float)WINDOW_WIDTH, 0.f, 0.f, 0.f,
			0.f, -2.f / (float)WINDOW_HEIGHT, 0.f, 0.f,
			0.f, 0.f, 1.f, 0.f,
			-1.f, 1.f, 0.f, 1.f);

		create();
		m_vertexPT[0] = VertexPT(0.f, 0.f, 0.f, 0.f, 0.f);
		m_vertexPT[1] = VertexPT(1.f, 0.f, 0.f, 1.f, 0.f);
		m_vertexPT[2] = VertexPT(0.f, 1.f, 0.f, 0.f, 1.f);
		m_vertexPT[3] = VertexPT(1.f, 1.f, 0.f, 1.f, 1.f);
	}
};

Image::~Image() {
	
}

void Image::testDraw() {
	float vtx[9] = {
		0.f,0.f,0.f,
		1.f,0.f,0.f,
		0.f,1.f,0.f
	};
	float asp = (float)WINDOW_HEIGHT / (float)WINDOW_WIDTH;
	D3DXMATRIX proj(asp, 0.f, 0.f, 0.f,
		0.f, -1.f, 0.f, 0.f,
		0.f, 0.f, 1.f, 0.f,
		0.f, 0.f, 0.f, 1.f);
	D3DXMATRIX trans(1.f, 0.f, 0.f, 0.f,
		0.f, 1.f, 0.f, 0.f,
		0.f, 0.f, 1.f, 0.f,
		-1.f, 1.f, 0.f, 1.f);
	Graphics::D3DDev()->SetTransform(D3DTS_PROJECTION, &(proj*trans));
	Graphics::D3DDev()->SetFVF(D3DFVF_XYZ);
	Graphics::D3DDev()->DrawPrimitiveUP(D3DPT_TRIANGLELIST, 1, vtx, sizeof(float) * 3);

}

void Image::testDraw2() {



	m_effect->SetTechnique("tech");
	//m_effect->SetMatrix("g_proj", &m_proj);
	m_effect->SetMatrix("g_world", &m_world);
	m_effect->SetTexture("g_tex", m_texture);
	
	m_effect->SetValue("g_UVsize", &D3DXVECTOR2(0.55f, 0.55f), sizeof(D3DXVECTOR2));
	m_effect->SetValue("g_UVoffset", &D3DXVECTOR2(0.2f, 0.2f), sizeof(D3DXVECTOR2));
	m_effect->SetValue("g_pictureSize", &D3DXVECTOR2(1.f, 1.f), sizeof(D3DXVECTOR2));

	UINT numPasses = 0;
	m_effect->Begin(&numPasses, 0);
	for (UINT j = 0; j < numPasses; j++) {
		m_effect->BeginPass(j);
		//Graphics::D3DDev()->SetVertexDeclaration(m_decl);
		m_effect->CommitChanges();
		Graphics::D3DDev()->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, 2, m_vertexPT, sizeof(VertexPTC));

		m_effect->EndPass();
	}

	m_effect->End();
}

void Image::draw() {
	createWorldMatrix();

	m_effect->SetTechnique("tech");
	m_effect->SetMatrix("g_proj", &m_projChange);
	m_effect->SetMatrix("g_world", &m_world);
	m_effect->SetTexture("g_tex", m_texture);

	m_effect->SetValue("g_UVsize",&m_UVsize, sizeof(D3DXVECTOR2));
	m_effect->SetValue("g_UVoffset", &m_UVoffset, sizeof(D3DXVECTOR2));
	m_effect->SetValue("g_pictureSize", &m_size, sizeof(D3DXVECTOR2));
	m_effect->SetVector("g_color", &m_color);


	UINT numPasses = 0;
	m_effect->Begin(&numPasses, 0);
	for (UINT j = 0; j < numPasses; j++) {
		m_effect->BeginPass(j);
		Graphics::D3DDev()->SetVertexDeclaration(VertexPT::decl);
		m_effect->CommitChanges();
		Graphics::D3DDev()->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, 2, m_vertexPT, sizeof(VertexPT));
		
		m_effect->EndPass();
	}

	m_effect->End();
}
void Image::draw(float x, float y, float width, float height) {
	m_position.x = x;
	m_position.y = y;
	//まずスケーリング
	m_vertexPT[1].pos.x = m_vertexPT[3].pos.x = width;
	m_vertexPT[2].pos.y = m_vertexPT[3].pos.y = height;
	draw();
}
void Image::drawWindow() {
	draw(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);
}
void Image::createWorldMatrix() {
	D3DXMATRIX trans, rot, scale;
	D3DXMatrixTranslation(&trans, m_position.x, m_position.y, 0.f);
	D3DXMatrixRotationZ(&rot, m_angleZ);
	D3DXMatrixScaling(&scale, m_scale.x, m_scale.y, 1.f);

	m_world = scale*rot*trans;
}


void Image::setPosition(const D3DXVECTOR2& a) { m_position = a; }
void Image::setSize(const D3DXVECTOR2& a) { m_size = a; }
void Image::setAngleZ(float a) { m_angleZ = a; }

void Image::setUVsize(const D3DXVECTOR2& a) { m_UVsize = a; }
void Image::setUVoffset(const D3DXVECTOR2& a) { m_UVoffset = a; }
void Image::setColor(float r, float g, float b) { m_color.x = r; m_color.y = g; m_color.z = b; }
void Image::setAlpha(float a) { m_color.w = a; }



















