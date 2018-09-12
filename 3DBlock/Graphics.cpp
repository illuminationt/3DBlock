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
		

		//HAL(Hardware Abstraction Layer)�ɑΉ����Ă邩�`�F�b�N
		D3DDISPLAYMODE mode;
		m_pD3D->GetAdapterDisplayMode(D3DADAPTER_DEFAULT, &mode);
		//�E�C���h�E���[�h�̃`�F�b�N
		HR(m_pD3D->CheckDeviceType(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, mode.Format, mode.Format, true));
		//�t���X�N���[�����[�h�̃`�F�b�N
		HR(m_pD3D->CheckDeviceType(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, mode.Format, mode.Format, false));

		//�n�[�h�E�F�A���_�����ł��邩�`�F�b�N
		D3DCAPS9 caps;
		HR(m_pD3D->GetDeviceCaps(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, &caps));

		DWORD behaviourFlags = 0;
		if (caps.DevCaps&D3DDEVCAPS_HWTRANSFORMANDLIGHT) {
			behaviourFlags |= D3DCREATE_HARDWARE_VERTEXPROCESSING;
		}
		else {
			behaviourFlags |= D3DCREATE_SOFTWARE_VERTEXPROCESSING;
		}

		//Pure devive���g���邩�`�F�b�N
		if (caps.DevCaps&D3DDEVCAPS_PUREDEVICE) {
			if (behaviourFlags&D3DCREATE_HARDWARE_VERTEXPROCESSING) {
				behaviourFlags |= D3DCREATE_PUREDEVICE;
			}
		}

		//D3DPRESENT_PARAMETERS �ݒ�
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

		//D3DDevice�̍쐬
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
		
		//Graphics�N���X��������C���^�t�F�C�X�̏�����

		m_text->initialize(m_hWnd, m_pD3DDev, 15, 45);
		
		HR(D3DXCreateSprite(m_pD3DDev, &m_pSprite));
		
	}


	bool Graphics::isDeviceLost() {
		HRESULT hr = m_pD3DDev->TestCooperativeLevel();

		//�f�o�C�X�����X�g���A�܂��C���ł��Ȃ�
		//��U�X���[�v�����A���̃t���[����҂�
		if (hr == D3DERR_DEVICELOST) {
			Sleep(20);
			return true;
		}

		//�����h���C�o���G���[���N�����Ă���B
		//���̏ꍇ�A�A�v�����I��
		else if (hr == D3DERR_DRIVERINTERNALERROR) {
			warn(true, "internal driver error ...exiting");
			PostQuitMessage(0);
			return true;
		}

		//�f�o�C�X�̓��X�g���Ă��邪�A�C���ł���B
		else if (hr == D3DERR_DEVICENOTRESET) {
			onLostDevice();
			HR(m_pD3DDev->Reset(&m_D3DPP));
			onResetDevice();

			return false;
		}

		//�����������ĂȂ�
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
		
		//�t���X�N���[�����[�h��
		if (enable) {
			//���Ƀt���X�N���[���Ȃ�return
			if (!m_D3DPP.Windowed) {
				return;
			}

			//���݂̃X�N���[���̉𑜓x���Q�b�g
			int width = GetSystemMetrics(SM_CXSCREEN);
			int height = GetSystemMetrics(SM_CYSCREEN);

			m_D3DPP.BackBufferFormat = D3DFMT_X8R8G8B8;
			m_D3DPP.BackBufferWidth = width;
			m_D3DPP.BackBufferHeight = height;
			m_D3DPP.Windowed = false;

			//�t���X�N���[���ɓK�����X�^�C���ɕύX
			SetWindowLongPtr(m_hWnd, GWL_STYLE, WS_POPUP);
			SetWindowPos(m_hWnd, HWND_TOP, 0, 0, width, height, SWP_NOZORDER | SWP_SHOWWINDOW);
		
		}
		//�E�C���h�E���[�h��
		else {
			//���ɃE�C���h�E���[�h�Ȃ�return
			if (m_D3DPP.Windowed) {
				return;
			}
			RECT rect = { 0,0,WINDOW_WIDTH,WINDOW_HEIGHT };
			AdjustWindowRect(&rect, WS_OVERLAPPEDWINDOW, false);

			m_D3DPP.BackBufferWidth = WINDOW_WIDTH;
			m_D3DPP.BackBufferHeight = WINDOW_HEIGHT;
			m_D3DPP.BackBufferFormat = D3DFMT_UNKNOWN;
			m_D3DPP.Windowed = true;

			//�E�C���h�E���[�h�ɓK�����X�^�C���ɕύX
			SetWindowLongPtr(m_hWnd, GWL_STYLE, WS_OVERLAPPEDWINDOW);
			SetWindowPos(m_hWnd, HWND_TOP, CW_USEDEFAULT, CW_USEDEFAULT, rect.right, rect.bottom, SWP_NOZORDER | SWP_SHOWWINDOW);
		}

		//�o�b�N�o�b�t�@�̃t�H�[�}�b�g�ς����̂Ńf�o�C�X�̃��Z�b�g������
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
