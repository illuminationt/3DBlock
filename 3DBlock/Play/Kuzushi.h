#ifndef KUZUSHI_H
#define KUZUSHI_H
#include<d3dx9.h>
#include<vector>


class Frame;
class Player;
class MyCamera;
class Block;
class Ball;

class Kuzushi
{
public:
	explicit Kuzushi(int stageID);
	~Kuzushi();

	void update(float dt);
	bool isGameOver();
	enum State {
		ALIVE,
		DEAD_RETRY,
		DEAD_TITLE,
		CLEAR,
		NONE,
	};
	const State state()const;
	void onLostDevice()const;
	void onResetDevice()const;
private:

	void draw(const D3DXMATRIX& pvm);
	void drawAnotherWindow(const D3DXVECTOR3& eye,
		const D3DXVECTOR3& target);
private://ヘルパー関数
	void readCSVFile(const char* CSVfilename);
	void locateBlocks();
private://仮
	void setCamera();
	void moveSun();
	void showUI();
	void afterHit(int ID, const D3DXVECTOR3& ballPos);
	void rebirthBall(const D3DXVECTOR3&);
	//ゲーム開始時、弾を発射したらtrue
	void beforeStart(bool* isStarted);
	void debug();


private:
	Frame* m_frame;
	Player* m_player;
	MyCamera* m_camera;
	Block** m_blocks;
	Ball** m_ball;

private://状態を司る
	unsigned m_totalScore;
	int m_remainedBlockNumber;
	int m_remainedBallNumber;
	bool* m_remainedBallList;
	bool m_isStarted;
	State m_state;
private:
	D3DXVECTOR3 m_dirToSun;
	std::vector<int> m_CSVdata;

	
	int m_maxBlockNumber;

private://定数群
	static const int m_colNumber;
	static const int m_rowNumber;
	static const int m_maxBallNumber;
};

#endif
