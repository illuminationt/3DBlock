#pragma once
#define WIN32_LEAN_AND_MEAN

#include<d3d9.h>
#include<d3dx9.h>
class Text;

	class Graphics
	{

	private:
		IDirect3D9* m_pD3D;
		static IDirect3DDevice9* m_pD3DDev;
		ID3DXSprite* m_pSprite;
		D3DPRESENT_PARAMETERS m_D3DPP;
		D3DDISPLAYMODE m_displayMode;

		static Text* m_text;
		HWND m_hWnd;

	public:
		/**
		コンストラクタ
		*/
		Graphics();
		virtual ~Graphics();

		/**
		Direct3D app の初期化
		主にDirect3DDeviceの作成
		@param hWnd ウインドウのハンドル

		*/
		void initialize(HWND hWnd);
	
		/**
		@brief デバイスが消失しているかチェック
		@return デバイスが消失していたらtrue
		デバイスがロストかつ修復できるならIDirect3DDevice9::Reset()を呼ぶ。
		*/
		bool isDeviceLost();
	
		/**
		IDirect3DDevice9::Reset()を呼ぶ前に必要な関数。詳細は本p.90)
		Reset()はGraphics::isDeviceLost()内で呼ばれている。
		*/
		void onLostDevice();

		/**
		IRirect3DDevice9::Reset()を呼んだ後に必要な関数。詳細は本p.90
		Reset()はGraphics::isDeviceLost()で呼ばれている
		*/
		void onResetDevice();
	
		/**
		@brief ウインドウ、フルスクリーンモードの切り替え
		@params enable true : フルスクリーン化 false : ウインドウモード化
		*/
		void enableFullScreenMode(bool enable);

		

		/**
		@brief 補佐的な関数.サイズ変えたときの処理
		*/
		void setBackBufferParams(LPARAM);
		void setBackBufferParams(const RECT&);


		static const Text* text();
		static const LPDIRECT3DDEVICE9 D3DDev();
		const D3DPRESENT_PARAMETERS D3DPP()const;
		D3DPRESENT_PARAMETERS D3DPP();
		const LPD3DXSPRITE sprite()const;
		const HWND hWnd()const;
	}
	;



