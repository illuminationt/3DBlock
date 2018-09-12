#include "Root.h"
#include"Vertex.h"
#include"Graphics.h"
#include"Sequence/Parent.h"
Root::Root()
{
	Sequence::Parent::create();
	m_parent = Sequence::Parent::instance();
}


Root::~Root()
{
	m_parent->destroy();
}


void Root::initialize(HWND hWnd) {
	AppBase::initialize(hWnd);

	initAllVertexDeclaration();
}

bool Root::checkDevCaps() {
	D3DCAPS9 caps;
	Graphics::D3DDev()->GetDeviceCaps(&caps);

	//頂点シェーダ2.0以上かチェック
	if (caps.VertexShaderVersion < D3DVS_VERSION(2, 0)) {
		return false;
	}
	//ピクセルシェーダ2.0以上かチェック
	if (caps.PixelShaderVersion < D3DPS_VERSION(2, 0)) {
		return false;
	}


	return true;

}

void Root::update() {
	AppBase::update();
	m_parent->update(m_frameTime);
}