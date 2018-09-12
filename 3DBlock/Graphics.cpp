#include "Graphics.h"
#include"Utility.h"
#include"Const.h"
#include"Text.h"

IDirect3DDevice9* Graphics::m_pD3DDev = 0;
Text* Graphics::m_text = 0;
	Graphics::Graphics() :
		m_pD3D(0),
		m_pSprite(0),
		m_hWnd(0){

		m_text = new Text();

	}


	Graphics::~Graphics()
	{
		SAFE_DELETE(m_text);
	}

	void Graphics::initialize(HWND hWnd){
		m_hWnd = hWnd;
		m_pD3D = Direct3DCreate9(D3D_SDK_VERSION);
		warn(!m_pD3D, "Direct3DCreate9 FAILED");
		

		//HAL(Hardware Abstraction Layer)に対応してるかチェック
		D3DDISPLAYMODE mode;
		m_pD3D->GetAdapterDisplayMode(D3DADAPTER_DEFAULT, &mode);
		//ウインドウモードのチェック
		HR(m_pD3D->CheckDeviceType(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, mode.Format, mode.Format, true));
		//フルスクリーンモードのチェック
		HR(m_pD3D->CheckDeviceType(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, mode.Format, mode.Format, false));

		//ハードウェア頂点処理できるかチェック
		D3DCAPS9 caps;
		HR(m_pD3D->GetDeviceCaps(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, &caps));

		DWORD behaviourFlags = 0;
		if (caps.DevCaps&D3DDEVCAPS_HWTRANSFORMANDLIGHT) {
			behaviourFlags |= D3DCREATE_HARDWARE_VERTEXPROCESSING;
		}
		else {
			behaviourFlags |= D3DCREATE_SOFTWARE_VERTEXPROCESSING;
		}

		//Pure deviveを使えるかチェック
		if (caps.DevCaps&D3DDEVCAPS_PUREDEVICE) {
			if (behaviourFlags&D3DCREATE_HARDWARE_VERTEXPROCESSING) {
				behaviourFlags |= D3DCREATE_PUREDEVICE;
			}
		}

		//D3DPRESENT_PARAMETERS 設定
		ZeroMemory(&m_D3DPP, sizeof(m_D3DPP));
		m_D3DPP.BackBufferWidth = 0;
		m_D3DPP.BackBufferHeight = 0;
		m_D3DPP.BackBufferFormat = mode.Format;
		m_D3DPP.BackBufferCount = 1;
		m_D3DPP.MultiSampleType = D3DMULTISAMPLE_NONE;
		m_D3DPP.MultiSampleQuality = 0;
		m_D3DPP.SwapEffect = D3DSWAPEFFECT_DISCARD;
		m_D3DPP.hDeviceWindow = hWnd;
		m_D3DPP.Windowed = true;
		m_D3DPP.EnableAutoDepthStencil = true;
		m_D3DPP.AutoDepthStencilFormat = D3DFMT_D24S8;
		m_D3DPP.Flags = 0;
		m_D3DPP.FullScreen_RefreshRateInHz = D3DPRESENT_RATE_DEFAULT;
		m_D3DPP.PresentationInterval = D3DPRESENT_INTERVAL_IMMEDIATE;

		//D3DDeviceの作成
		/*
		HR(m_pD3D->CreateDevice(
			D3DADAPTER_DEFAULT,
			D3DDEVTYPE_HAL,
			hWnd,
			behaviourFlags,
			&m_D3DPP,
			&m_pD3DDev));
			*/
		HR(m_pD3D->CreateDevice(D3DADAPTER_DEFAULT,D3DDEVTYPE_HAL,	hWnd,behaviourFlags,&m_D3DPP,&m_pD3DDev));
		
		//Graphicsクラスが抱えるインタフェイスの初期化

		m_text->initialize(m_hWnd, m_pD3DDev, 15, 45);
		
		HR(D3DXCreateSprite(m_pD3DDev, &m_pSprite));
		
	}


	bool Graphics::isDeviceLost() {
		HRESULT hr = m_pD3DDev->TestCooperativeLevel();

		//デバイスがロストかつ、まだ修復できない
		//一旦スリープさせ、次のフレームを待つ
		if (hr == D3DERR_DEVICELOST) {
			Sleep(20);
			return true;
		}

		//内部ドライバがエラーを起こしている。
		//この場合、アプリを終了
		else if (hr == D3DERR_DRIVERINTERNALERROR) {
			warn(true, "internal driver error ...exiting");
			PostQuitMessage(0);
			return true;
		}

		//デバイスはロストしているが、修復できる。
		else if (hr == D3DERR_DEVICENOTRESET) {
			onLostDevice();
			HR(m_pD3DDev->Reset(&m_D3DPP));
			onResetDevice();

			return false;
		}

		//何も消失してない
		return false;

	}

	void Graphics::onLostDevice() {
		HR(m_pSprite->OnLostDevice());
		m_text->onLostDevice();
	}
	void Graphics::onResetDevice() {
		HR(m_pSprite->OnResetDevice());
		m_text->onResetDevice();
	}

	void Graphics::enableFullScreenMode(bool enable) {
		
		//フルスクリーンモード化
		if (enable) {
			//既にフルスクリーンならreturn
			if (!m_D3DPP.Windowed) {
				return;
			}

			//現在のスクリーンの解像度をゲット
			int width = GetSystemMetrics(SM_CXSCREEN);
			int height = GetSystemMetrics(SM_CYSCREEN);

			m_D3DPP.BackBufferFormat = D3DFMT_X8R8G8B8;
			m_D3DPP.BackBufferWidth = width;
			m_D3DPP.BackBufferHeight = height;
			m_D3DPP.Windowed = false;

			//フルスクリーンに適したスタイルに変更
			SetWindowLongPtr(m_hWnd, GWL_STYLE, WS_POPUP);
			SetWindowPos(m_hWnd, HWND_TOP, 0, 0, width, height, SWP_NOZORDER | SWP_SHOWWINDOW);
		
		}
		//ウインドウモード化
		else {
			//既にウインドウモードならreturn
			if (m_D3DPP.Windowed) {
				return;
			}
			RECT rect = { 0,0,WINDOW_WIDTH,WINDOW_HEIGHT };
			AdjustWindowRect(&rect, WS_OVERLAPPEDWINDOW, false);

			m_D3DPP.BackBufferWidth = WINDOW_WIDTH;
			m_D3DPP.BackBufferHeight = WINDOW_HEIGHT;
			m_D3DPP.BackBufferFormat = D3DFMT_UNKNOWN;
			m_D3DPP.Windowed = true;

			//ウインドウモードに適したスタイルに変更
			SetWindowLongPtr(m_hWnd, GWL_STYLE, WS_OVERLAPPEDWINDOW);
			SetWindowPos(m_hWnd, HWND_TOP, CW_USEDEFAULT, CW_USEDEFAULT, rect.right, rect.bottom, SWP_NOZORDER | SWP_SHOWWINDOW);
		}

		//バックバッファのフォーマット変えたのでデバイスのリセットがいる
		onLostDevice();
		/*HR*/(m_pD3DDev->Reset(&m_D3DPP));
		onResetDevice();

	}


	void Graphics::setBackBufferParams(LPARAM lParam) {
		m_D3DPP.BackBufferWidth = LOWORD(lParam);
		m_D3DPP.BackBufferHeight = HIWORD(lParam);
	}
	void Graphics::setBackBufferParams(const RECT& rect) {
		m_D3DPP.BackBufferWidth = rect.right;
		m_D3DPP.BackBufferHeight = rect.bottom;
	}

	const Text* Graphics::text() { return m_text; }

	const LPDIRECT3DDEVICE9 Graphics::D3DDev() {
		return m_pD3DDev;
	}
	const D3DPRESENT_PARAMETERS Graphics::D3DPP()const { return m_D3DPP; }
	D3DPRESENT_PARAMETERS Graphics::D3DPP() { return m_D3DPP; }
	const LPD3DXSPRITE Graphics::sprite()const { return m_pSprite; }
	const HWND Graphics::hWnd()const { return m_hWnd; }
