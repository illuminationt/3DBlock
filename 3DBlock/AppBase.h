#ifndef D3DAPP_H
#define D3DAPP_H

#include<Windows.h>
#include<d3dx9.h>
class Input;

class Graphics;
class Text;
class Test;
class AppBase {

public:
	static Graphics* m_graphics;

	//GfxStats* m_status;
public:
	AppBase();
	virtual ~AppBase();

	//派生先で継承
	virtual void update() = 0;
	virtual bool checkDevCaps() = 0;


	void run();
	virtual void initialize(HWND hWnd);

	//デバイス消失用(オーバーロードを期待します)
	virtual void onLostDevice();
	virtual void onResetDevice();

	//const LPDIRECT3DDEVICE9 D3DDev()const;

	LRESULT handleMessage(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

protected:
	float   m_frameTime; // time required for last frame

private:

	//諸々
	LARGE_INTEGER m_timeStart;    // Performance Counter start value
	LARGE_INTEGER m_timeEnd;      // Performance Counter end value
	LARGE_INTEGER m_timerFreq;    // Performance Counter frequency
	       
	float   m_fps;                // frames per second
	DWORD   m_sleepTime;          // number of milli-seconds to sleep between frames

	bool m_initialized;
	bool m_paused;
	bool m_minOrMaxed;

	
};

#endif