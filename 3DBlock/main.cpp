#include<d3d9.h>
#include<Windows.h>
#include<crtdbg.h>
#include"Const.h"
#include"Utility.h"
#include"AppBase.h"
#include"Root.h"
#include"Vertex.h"

//テスト
//#include"test.h"
#define _CRT_SECURE_NO_WARNINGS

/**
main.cppでのみ使う変数
*/

	HWND g_hWnd = 0;
	Root* g_root = 0;
LRESULT WINAPI WinProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) {
	

	return g_root->handleMessage(hWnd, msg, wParam, lParam);
	
}

bool createMainWindow(HWND& hWnd, HINSTANCE hInstance, int nCmdShow) {
	WNDCLASSEX wcx;

	wcx.cbSize = sizeof(wcx);
	wcx.style = CS_HREDRAW | CS_VREDRAW;
	wcx.lpfnWndProc = WinProc;
	wcx.cbClsExtra = 0;
	wcx.cbWndExtra = 0;
	wcx.hInstance = hInstance;
	wcx.hIcon = NULL;
	wcx.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);    // black background 
	wcx.hCursor = LoadCursor(NULL, IDC_ARROW);
	wcx.lpszMenuName = NULL;            // name of menu resource 
	wcx.lpszClassName = CLASS_NAME;   // name of window class 
	wcx.hIconSm = NULL;
	/*
	if (!RegisterClassEx(&wcx)) {
		MessageBox(0, "RegisterClass FAILED", 0, 0);
		PostQuitMessage(0);
	}
	*/
	warn(!RegisterClassEx(&wcx), "RegisterClass failed");

	//ウインドウ作っていきます
	//アプリ立ち上げ時はウインドウモード.
	DWORD style = 0;
	style = WS_OVERLAPPEDWINDOW;

	hWnd = CreateWindow(
		CLASS_NAME,
		APP_NAME,
		style,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		WINDOW_WIDTH,
		WINDOW_HEIGHT,
		NULL,
		NULL,
		hInstance,
		NULL);

	if (!hWnd) {
		return false;
	}

	//初めはウインドウモードなのでこの設定は要る
	RECT rect;
	GetClientRect(hWnd, &rect);
	int w = WINDOW_WIDTH + (WINDOW_WIDTH - rect.right);
	int h = WINDOW_HEIGHT + (WINDOW_HEIGHT - rect.bottom);
	//MoveWindow(hWnd, 0, 0, w, h, TRUE);

	ShowWindow(hWnd, nCmdShow);

	return true;
}





int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR cmdLine, int nCmdShow) {

	//メモリリーク検出用
#if defined(DEBUG) | defined(_DEBUG)
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
#endif

	g_root = new Root();

	if (!createMainWindow(g_hWnd, hInstance, nCmdShow)) {
		return -1;
	}
	
	g_root->initialize(g_hWnd);
	initAllVertexDeclaration();
	MSG msg;
	do {
		if (PeekMessage(&msg, 0, 0, 0, PM_REMOVE)) {
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else {
			//メイン処理
			g_root->run();
		}

		
	} while (msg.message != WM_QUIT);
	SAFE_DELETE(g_root);
	return msg.wParam;
	
}

