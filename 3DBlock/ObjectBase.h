#ifndef OBJECTBASE_H
#define OBJECTBASE_H

#include<d3dx9.h>

class Geometry;

/*
この基底クラスが担うこと：
Geometry.fxシェーダを用いるD3DXCreate系のメッシュを扱う
・シェーダのエフェクトパラメータへのハンドル取得
・ID3DXEffectインターフェイスが未取得なら取得
・射影行列、光の情報をもらってdraw



派生クラスがすべきこと
・コンストラクタのオーバーロード
・使いたいジオメトリをprivate宣言
・各々のコンストラクタでGeometryのnew,initialize,setMaterial,create
・update関数でGeometryのPos,Angleなどをset
*/


class ObjectBase
{
public:
	ObjectBase();
	virtual ~ObjectBase();

	void update();
	
	//pointLightPosW,dirToSunWは0を指定すると無視
	void draw(const D3DXMATRIX& pvm,
		const D3DXVECTOR3& eyePosW,
		const D3DXVECTOR3* pointLightPosW=0,
		const D3DXVECTOR3* pointLightPosW2 = 0,
		const D3DXVECTOR3* pointLightPosW3 = 0,
		const D3DXVECTOR3& dirToSunW = D3DXVECTOR3(0.f,1.f,0.f),
		const D3DXVECTOR4& diffuseLight = D3DXVECTOR4(1.f, 1.f, 1.f, 1.f),
		const D3DXVECTOR4& ambientLight = D3DXVECTOR4(1.f, 1.f, 1.f, 1.f),
		const D3DXVECTOR4& specularLight = D3DXVECTOR4(1.f, 1.f, 1.f, 1.f));

	void setPosition(const D3DXVECTOR3&);

	bool isAlive()const { return m_isAlive; }
	virtual void rebirth();
	virtual void destroy();

	void onLostDevice() { m_effect->OnLostDevice(); }
	void onResetDevice() { m_effect->OnResetDevice(); }
private:
	//draw内部で使われる
	virtual void drawGeometry(const D3DXMATRIX&) = 0;
protected:
	static ID3DXEffect* m_effect;
protected:
	D3DXVECTOR3 m_position;
	D3DXVECTOR3 m_angle;
	D3DXVECTOR3 m_scale;

protected:
	bool m_isAlive;
private:
	D3DXHANDLE m_hTech;

	D3DXHANDLE m_hDiffuseLight;
	D3DXHANDLE m_hAmbientLight;
	D3DXHANDLE m_hSpecularLight;


	D3DXHANDLE m_hDirToSunW;
	D3DXHANDLE m_hPointLightPosW;
	D3DXHANDLE m_hPointLightPosW2;
	D3DXHANDLE m_hPointLightPosW3;
	D3DXHANDLE m_hPointLightAttenuation;
	D3DXHANDLE m_hSpecularAttenuation;
	D3DXHANDLE m_hEyePosW;

};


#endif