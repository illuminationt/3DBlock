#include"AppBase.h"
#include"Input.h"
#include"Graphics.h"
#include"Text.h"
#include"Utility.h"
#include"Const.h"
#include<tchar.h>
#include<string>

//テスト
//#include"Test.h"
Graphics* AppBase::m_graphics = 0;

AppBase::AppBase() :
	m_paused(false),
	m_minOrMaxed(false),
	m_initialized(false)
{
	
	if (!checkDevCaps()) {
		warn("Your PC doesn't have enough spec for running this application");
		PostQuitMessage(0);
	}
	
	m_graphics = new Graphics();

	
}
AppBase::~AppBase() {
	SAFE_DELETE(m_graphics);
}

void AppBase::run() {
	{
		// calculate elapsed time of last frame, save in frameTime
		QueryPerformanceCounter(&m_timeEnd);
		m_frameTime = (float)(m_timeEnd.QuadPart - m_timeStart.QuadPart) /
			(float)m_timerFreq.QuadPart;

		// Power saving code, requires winmm.lib
		// if not enough time has elapsed for desired frame rate
		if (m_frameTime < MIN_FRAME_TIME)
		{
			m_sleepTime = (DWORD)((MIN_FRAME_TIME - m_frameTime) * 1000);
			timeBeginPeriod(1);         // Request 1mS resolution for windows timer
			Sleep(m_sleepTime);           // release cpu for sleepTime
			timeEndPeriod(1);           // End 1mS timer resolution
			//return;
		}

		m_fps = 1.f / m_frameTime;

		if (m_frameTime > MAX_FRAME_TIME) // if frame rate is very slow
			//m_frameTime = MAX_FRAME_TIME; // limit maximum frameTime

		m_timeStart = m_timeEnd;
		
		
	}
	std::ostringstream oss("");
	oss << "FPS : " << m_fps << "\n Time in a frame (ms) : " << m_frameTime*1000;

	//描画メイン==================================
	m_graphics->D3DDev()->Clear(0, 0, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, D3DCOLOR_XRGB(0,0,0), 1.f, 0);
	m_graphics->D3DDev()->BeginScene();
	
	//m_graphics->sprite()->Begin(0);

	update();

	//m_graphics->sprite()->End();

	m_graphics->text()->draw(oss.str().c_str());
	m_graphics->D3DDev()->EndScene();
	m_graphics->D3DDev()->Present(0, 0, 0, 0);
	//=============================================

	{//CopyRight
		Input::instance()->readControllers();       // read state of controllers			
		Input::instance()->clear(inputNS::KEYS_PRESSED);
	}

	
}



void AppBase::initialize(HWND hWnd) {
	m_graphics->initialize(hWnd);
	Input::create(hWnd);
	
	//パフォーマンスカウンタの周波数を取得	
	if (QueryPerformanceFrequency(&m_timerFreq) == false) {

	}
	//パフォーマンスカウンタの開始時刻を取得
	QueryPerformanceCounter(&m_timeStart);


	//m_test = new Test(m_graphics->D3DDev(), m_graphics->sprite());

	m_initialized = true;
}

void AppBase::update() {
	if (Input::instance()->isKeyDown('A')) {
		//m_graphics->enableFullScreenMode(true);
	}
	if (Input::instance()->isKeyDown('B')) {
		//m_graphics->enableFullScreenMode(false);
	}
	//TODO: とりあえず今はキャンセルしとけ
	//m_test->move();
	//m_test->draw();


}
bool AppBase::checkDevCaps() { return true; }
void AppBase::onLostDevice() {
	m_graphics->onLostDevice();
	Graphics::text()->onLostDevice();
}

void AppBase::onResetDevice() {
	m_graphics->onResetDevice();
	Graphics::text()->onResetDevice();
}





/*
const LPDIRECT3DDEVICE9 AppBase::D3DDev()const {
	return m_graphics->D3DDev();
}
*/
LRESULT AppBase::handleMessage(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) {

	switch (msg) {
	case WM_DESTROY:
		PostQuitMessage(0);        //tell Windows to kill this program
		return 0;
	case WM_KEYDOWN: case WM_SYSKEYDOWN:    // key down
		Input::instance()->keyDown(wParam);
		return 0;
	case WM_KEYUP: case WM_SYSKEYUP:        // key up
		Input::instance()->keyUp(wParam);
		return 0;
	case WM_CHAR:                           // character entered
		Input::instance()->keyIn(wParam);
		return 0;
	case WM_MOUSEMOVE:                      // mouse moved
		Input::instance()->mouseIn(lParam);
		return 0;
	case WM_INPUT:                          // raw mouse data in
		Input::instance()->mouseRawIn(lParam);
		return 0;
	case WM_LBUTTONDOWN:                    // left mouse button down
		Input::instance()->setMouseLButton(true);
		Input::instance()->mouseIn(lParam);             // mouse position
		return 0;
	case WM_LBUTTONUP:                      // left mouse button up
		Input::instance()->setMouseLButton(false);
		Input::instance()->mouseIn(lParam);             // mouse position
		return 0;
	case WM_MBUTTONDOWN:                    // middle mouse button down
		Input::instance()->setMouseMButton(true);
		Input::instance()->mouseIn(lParam);             // mouse position
		return 0;
	case WM_MBUTTONUP:                      // middle mouse button up
		Input::instance()->setMouseMButton(false);
		Input::instance()->mouseIn(lParam);             // mouse position
		return 0;
	case WM_RBUTTONDOWN:                    // right mouse button down
		Input::instance()->setMouseRButton(true);
		Input::instance()->mouseIn(lParam);             // mouse position
		return 0;
	case WM_RBUTTONUP:                      // right mouse button up
		Input::instance()->setMouseRButton(false);
		Input::instance()->mouseIn(lParam);             // mouse position
		return 0;
	case WM_XBUTTONDOWN: case WM_XBUTTONUP: // mouse X button down/up
		Input::instance()->setMouseXButton(wParam);
		Input::instance()->mouseIn(lParam);             // mouse position
		return 0;
	case WM_MOUSEWHEEL:                     // mouse wheel move
		Input::instance()->mouseWheelIn(wParam);
		return 0;
	case WM_DEVICECHANGE:                   // check for controller insert
		Input::instance()->checkControllers();
		return 0;

		{
	case WM_SIZE:
		if (!m_initialized)return 0;
		if (m_graphics->D3DDev())
		{
			
			m_graphics->setBackBufferParams(lParam);
			if (wParam == SIZE_MINIMIZED)
			{
				m_paused = true;
				m_minOrMaxed = true;
			}
			else if (wParam == SIZE_MAXIMIZED)
			{
				m_paused = false;
				m_minOrMaxed = true;
				onLostDevice();
				HR(m_graphics->D3DDev()->Reset(&m_graphics->D3DPP()));
				onResetDevice();
			}
			// Restored is any resize that is not a minimize or maximize.
			// For example, restoring the window to its default size
			// after a minimize or maximize, or from dragging the resize
			// bars.
			else if (wParam == SIZE_RESTORED)
			{
				m_paused = false;

				// Are we restoring from a mimimized or maximized state, 
				// and are in windowed mode?  Do not execute this code if 
				// we are restoring to full screen mode.
				if (m_minOrMaxed && m_graphics->D3DPP().Windowed)
				{
					onLostDevice();
					HR(m_graphics->D3DDev()->Reset(&m_graphics->D3DPP()));
					onResetDevice();
				}
				else
				{

				}
				m_minOrMaxed = false;
			}
		}
		return 0;


		// WM_EXITSIZEMOVE is sent when the user releases the resize bars.
		// Here we reset everything based on the new window dimensions.
	case WM_EXITSIZEMOVE:
		RECT clientRect;
		GetClientRect(hWnd, &clientRect);
		m_graphics->setBackBufferParams(clientRect);
		onLostDevice();
		HR(m_graphics->D3DDev()->Reset(&m_graphics->D3DPP()));
		onResetDevice();

		return 0;

		// WM_CLOSE is sent when the user presses the 'X' button in the
		// caption bar menu.
	case WM_CLOSE:
		DestroyWindow(hWnd);
		return 0;



		}

	default:
		return DefWindowProc(hWnd, msg, wParam, lParam);
	}
}