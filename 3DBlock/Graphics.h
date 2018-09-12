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
		�R���X�g���N�^
		*/
		Graphics();
		virtual ~Graphics();

		/**
		Direct3D app �̏�����
		���Direct3DDevice�̍쐬
		@param hWnd �E�C���h�E�̃n���h��

		*/
		void initialize(HWND hWnd);
	
		/**
		@brief �f�o�C�X���������Ă��邩�`�F�b�N
		@return �f�o�C�X���������Ă�����true
		�f�o�C�X�����X�g���C���ł���Ȃ�IDirect3DDevice9::Reset()���ĂԁB
		*/
		bool isDeviceLost();
	
		/**
		IDirect3DDevice9::Reset()���ĂԑO�ɕK�v�Ȋ֐��B�ڍׂ͖{p.90)
		Reset()��Graphics::isDeviceLost()���ŌĂ΂�Ă���B
		*/
		void onLostDevice();

		/**
		IRirect3DDevice9::Reset()���Ă񂾌�ɕK�v�Ȋ֐��B�ڍׂ͖{p.90
		Reset()��Graphics::isDeviceLost()�ŌĂ΂�Ă���
		*/
		void onResetDevice();
	
		/**
		@brief �E�C���h�E�A�t���X�N���[�����[�h�̐؂�ւ�
		@params enable true : �t���X�N���[���� false : �E�C���h�E���[�h��
		*/
		void enableFullScreenMode(bool enable);

		

		/**
		@brief �⍲�I�Ȋ֐�.�T�C�Y�ς����Ƃ��̏���
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



