#pragma once

#ifndef SHIO_UTILITY_H
#define SHIO_UTILITY_H

#if defined(DEBUG) | defined(_DEBUG)
#ifndef D3D_DEBUG_INFO
#define D3D_DEBUG_INFO
#endif
#endif

#include <d3d9.h>
#include <d3dx9.h>
#include <DxErr.h>
#include <string>
#include <sstream>
#include<Windows.h>
#include<vector>
#include<random>
#define SAFE_DELETE(x){if(x){delete (x);(x)=(0);}}
#define SAFE_DELETE_ARRAY(x){if(x){delete[] (x);(x)=(0);}}
#define SAFE_RELEASE(x) { if(x){ (x)->Release();(x) = 0; } }

#ifndef WARN
#define WARN
inline void warn(bool isSatisfied, const char* msg = "an error occurred") {
	if (isSatisfied) {
		MessageBox(NULL, msg, 0, 0);
	}
}
inline void warn(const char* msg = "an error occurred") {
	warn(true, msg);
}

float random(int min,int max);


const static float INF = FLT_MAX;
const D3DXCOLOR WHITE(1.f, 1.f, 1.f, 1.f);
#endif

/**
hrを返す関数を入れて、簡潔にエラーチェック
*/
#define HR(x) if(FAILED(x))warn()




#endif





