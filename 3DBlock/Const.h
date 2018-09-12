#ifndef SHIO_CONST_H
#define SHIO_CONST_H


	static const int WINDOW_WIDTH = 800;
	static const int WINDOW_HEIGHT = 600;

	static const int FRAME_RATE = 1000;
	static const int MIN_FRAME_RATE = 1000;
	static const float MIN_FRAME_TIME = 1.f / FRAME_RATE;
	static const float MAX_FRAME_TIME = 1.f / MIN_FRAME_RATE;

	static const char* CLASS_NAME = "test";
	static const char* APP_NAME = "Application";

	static const float BLOCK_RADIUS = 0.5f;
	static const float BLOCK_HEIGHT = 1.f;

	static const float BALL_RADIUS = 0.5f;

	static const float EPSILON = 0.1f;

#if true
	static const char* TITLE_FILENAME = "resource/title.png";
	static const char* LOAD_FILENAME = "resource/load.png";
	static const char* CLEAR_FILENAME = "resource/clear.png";
	static const char* MODE_FILENAME = "resource/mode.png";
#endif

#if false
	static const char* TITLE_FILENAME = "temporary/boy1.jpg";
	static const char* LOAD_FILENAME = "temporary/boy2.jpg";
	static const char* CLEAR_FILENAME = "temporary/boy3.jpg";
	static const char* MODE_FILENAME = "temporary/boy.jpg";
#endif


#endif