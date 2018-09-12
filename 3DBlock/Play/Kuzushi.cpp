#include "Play/Kuzushi.h"
#include "Play/Frame.h"
#include"Play/Player.h"
#include"MyCamera.h"
#include"Input.h"
#include"Block.h"
#include"Ball.h"
#include"BlocksData.h"
#include"Graphics.h"
#include"Text.h"
#include"Const.h"
#include<sstream>
#include<string>
#include<fstream>
#include<iostream>
#include<vector>
const int Kuzushi::m_colNumber = 17;
const int Kuzushi::m_rowNumber = 11;
const int Kuzushi::m_maxBallNumber = 3;

using namespace std;
namespace {
	vector<string> split(string& input, char delimiter)
	{
		istringstream stream(input);
		string field;
		vector<string> result;
		while (getline(stream, field, delimiter)) {
			result.push_back(field);
		}
		return result;
	}

	bool hasValue(const std::vector<int>& v, int value) {
		std::vector<int>::const_iterator it = v.cbegin();
		for (; it != v.end(); it++) {
			if (*it== value) {
				return true;
			}
		}
		return false;
	}
}

Kuzushi::Kuzushi(int stageID) :
	m_frame(0),
	m_dirToSun(0.f, 1.f, 0.f),
	m_player(0),
	m_camera(0),
	m_blocks(0),
	m_ball(0),
	m_totalScore(0),
	m_remainedBlockNumber(-1),
	m_remainedBallNumber(1),
	m_state(State::ALIVE),
	m_CSVdata(0),
	m_maxBlockNumber(0),
	m_remainedBallList(0),
	m_isStarted(false)
{
	m_frame = new Frame();
	m_player = new Player();
	m_camera = new MyCamera();

	m_ball = new Ball*[m_maxBallNumber];
	for (int j = 0; j < m_maxBallNumber; j++) {
		m_ball[j] = new Ball();
	}


	m_ball[0]->setPosition(D3DXVECTOR3(0.f, 1.f, -5.f));

	m_ball[0]->rebirth();


	m_player->setPosition(D3DXVECTOR3(0.f, 1.f, -15.f));

	{
		m_camera->setPosition(D3DXVECTOR3(0.f, 20.f, -30.f));
		m_camera->setTarget(D3DXVECTOR3(0.f, 0.f, -5.f));
	}

	ostringstream oss;
	oss << "csv/blocks_";
	if (0 <= stageID&&stageID <= 9) {
		oss << "0";
	}
	oss << stageID << ".csv";
	readCSVFile(oss.str().c_str());
	locateBlocks();

	m_remainedBallList = new bool[m_maxBallNumber];
	for (int j = 0; j < m_maxBallNumber; j++) {
		m_remainedBallList[j] = false;
	}
	m_remainedBallList[0] = true;
}


Kuzushi::~Kuzushi()
{
	SAFE_DELETE(m_frame);
	SAFE_DELETE(m_player);
	SAFE_DELETE(m_camera);

	for (int j = 0; j < m_maxBlockNumber; j++) {
		SAFE_DELETE(m_blocks[j]);
	}
	SAFE_DELETE_ARRAY(m_blocks);

	SAFE_DELETE_ARRAY(m_remainedBallList);
}
void Kuzushi::update(float dt) {
	if (isGameOver()) {
		Graphics::text()->drawTrue("Game Over", 200, 200);
		Graphics::text()->drawTrue("1 : Retry\n2 : Title", 200, 300);
		if (Input::instance()->wasKeyPressed('1')) {
			m_state = State::DEAD_RETRY;
			return;
		}
		else if (Input::instance()->wasKeyPressed('2')) {
			m_state = State::DEAD_TITLE;
			return;
		}
		return;
	}
	
	if (!m_isStarted) {
		beforeStart(&m_isStarted);
	}

	//当たり判定
	for (int j = 0; j < m_maxBallNumber; j++) {
		m_ball[j]->reflectWall(m_frame->mesh(), m_frame->vertex());
		m_ball[j]->reflectWall(m_player->mesh(), m_player->vertex(), m_player->velocity(), m_player->invWorld(), m_player->invAngle(),m_player);
		for (int k = 0; k < m_maxBlockNumber; k++) {
			unsigned score = 0;
			int ID = -1;
			bool hit=m_ball[j]->reflectSylinder(m_blocks[k],&score,&ID);
			if (hit) {
				afterHit(ID, *m_ball[j]->position());
				m_totalScore += score;
				m_remainedBlockNumber--;
			}
		}
	}
	
	if (m_remainedBlockNumber == 0) {
		m_state = State::CLEAR;
	}

	for (int j = 0; j < m_maxBallNumber; j++) {
		if (!m_ball[j]->isAlive()) {
			if (m_remainedBallList[j]) {
				m_remainedBallList[j] = false;
				m_remainedBallNumber--;
			}
		}
	}

	
	
	

	m_camera->testMove();
	
	//showUI();
	//debug();

	setCamera();
	D3DXMATRIX pvm;
	m_camera->createProjMatrix(&pvm);

	m_player->update(dt,m_ball,m_isStarted);
	for (int j = 0; j < m_maxBallNumber; j++) {
		m_ball[j]->update(dt);
	}
	
	draw(pvm);
	

}

bool Kuzushi::isGameOver() {
	for (int j = 0; j < m_maxBallNumber; j++) {
		if (m_ball[j]->isAlive()) {
			return false;
		}
	}
	return true;
}

void Kuzushi::draw(const D3DXMATRIX& pvm) {
	m_frame->draw(pvm, *m_ball[0]->position(), *m_ball[1]->position(),*m_ball[2]->position());
	
	m_player->draw(pvm, *m_camera->position(),m_ball[0]->position(),m_ball[1]->position(),m_ball[2]->position());
	
	for (int j = 0; j < m_maxBallNumber; j++) {
		m_ball[j]->draw(pvm, *m_camera->position(), m_ball[0]->position(),m_ball[1]->position(),m_ball[2]->position());
	}
	for (int j = 0; j < m_maxBlockNumber; j++) {
		m_blocks[j]->draw(pvm, *m_camera->position(), m_ball[0]->position(),m_ball[1]->position(),m_ball[2]->position());
	}
}

void Kuzushi::drawAnotherWindow(const D3DXVECTOR3& eye,const D3DXVECTOR3& target) {
	//MyCamera* camera;
	
	m_camera->setPosition(eye);
	m_camera->setTarget(target);
	D3DXMATRIX pvm;
	m_camera->createProjMatrix(&pvm);

	float w = (float)WINDOW_WIDTH;
	float h = (float)WINDOW_HEIGHT;
	
	
	D3DVIEWPORT9 q = { w/2.f,0,w / 2.f,h / 2.f,0.f,1.f };
	Graphics::D3DDev()->SetViewport(&q);
	
	draw(pvm);
}
/*
void Kuzushi::setCamera() {
	D3DXVECTOR3 playerPos = *(m_player->position());


	float angleY = m_player->angleY();
	D3DXVECTOR3 dir(sin(D3DXToRadian(angleY)), 0, cos(D3DXToRadian(angleY)));

	D3DXVECTOR3 eye, target;
	eye = playerPos - dir*20.f;
	eye += D3DXVECTOR3(0.f, 18.f, 0.f);
	target = playerPos + dir*1.5f;
	target.y += 0.f;

	Input* i = Input::instance();
	if (i->isKeyDown('I')) {
		target.y += 0.4f;
	}
	else if (i->isKeyDown('K')) {
		target.y -= 0.4f;
	}

	//target = D3DXVECTOR3(0.f, 0.f, 0.f);
	m_camera->setPosition(eye);
	m_camera->setTarget(target);
}
*/

const Kuzushi::State Kuzushi::state()const { return m_state; }
void Kuzushi::setCamera() {
	m_camera->setPosition(D3DXVECTOR3(0.f, 20.f, -28.f));
	m_camera->setTarget(D3DXVECTOR3(0.f, 0.f, -3.f));
	
	//float h = (float)WINDOW_HEIGHT;
	//float w = (float)WINDOW_WIDTH;
	
	//D3DVIEWPORT9 q = { 0.f,0,w,h ,0.f,1.f };
	//Graphics::D3DDev()->SetViewport(&q);
}
/*
z
angleY<0|   angleY>0
/-|-|
| +-|-- x
|---/
|


*/

void Kuzushi::moveSun() {
	Input* i = Input::instance();
	if (i->isKeyDown('Y')) {
		m_dirToSun.x += 0.04f;
	}
	if (i->isKeyDown('B')) {
		m_dirToSun.x -= 0.04f;
	}
}

void Kuzushi::readCSVFile(const char* CSVfilename) {
	ifstream ifs(CSVfilename);
	string str;
	int rowNum = 0;
	//「j番目に何を描画するか」を司るm_indexに代入してく。
	while (getline(ifs, str)) {
		vector<string> strvec = split(str, ',');
		char a=str[0];
		for (unsigned i = 0; i < strvec.size(); i++) {
			m_CSVdata.push_back(stoi(strvec.at(i)));
		}
	}
	
	for (int j = 0; j < m_colNumber*m_rowNumber; j++) {
		if (m_CSVdata[j] != -1) {
			m_maxBlockNumber++;
		}
	}

	m_remainedBlockNumber = m_maxBlockNumber;
	m_blocks = new Block*[m_maxBlockNumber];
	int t = 0;
	for (int j = 0; j < m_colNumber*m_rowNumber; j++) {
		if (m_CSVdata[j] != -1) {
			switch (m_CSVdata[j]) {
			case 0:m_blocks[t] = new BlockDefault(); break;
			case 1:m_blocks[t] = new BlockBig(); break;
			case 2:m_blocks[t] = new BlockDRed(); break;
			case 3:m_blocks[t] = new BlockLong(); break;
			case 4:m_blocks[t] = new BlockBall(); break;
			case 5:m_blocks[t] = new BlockMoveRound(); break;
			case 6:m_blocks[t] = new BlockRainbow(); break;
			}
			m_blocks[t]->setCSVposition(j);
			t++;
		}
	}
}

void Kuzushi::locateBlocks() {
	if (m_rowNumber < 0) {
		warn("何かがおかしい");
	}
	int p = -1;
	for (int j = 0; j < m_maxBlockNumber; j++) {
		p = m_blocks[j]->CSVposition();

		int col = p / m_rowNumber;
		int row = p%m_rowNumber;

		m_blocks[j]->setPosition(D3DXVECTOR3(row*2.f - 10.f, 1.f, -col + 14.f));
	}
}

void Kuzushi::showUI() {
	std::ostringstream oss;
	oss << "score : " << m_totalScore;
	Graphics::text()->draw(oss.str().c_str(), 600, 0);

	oss.str("");
	oss << "remained : " << m_remainedBlockNumber;
	Graphics::text()->draw(oss.str().c_str(), 600, 40);

}

void Kuzushi::afterHit(int ID, const D3DXVECTOR3& ballPos) {
	switch (ID) {
	case 4:
		rebirthBall(ballPos);
		break;

	}
}

void Kuzushi::rebirthBall(const D3DXVECTOR3& pos) {
	if (m_remainedBallNumber >= m_maxBallNumber) {
		return;
	}
	
	//リスト内でfalseになってるボールをrebirth
	int t = -1;
	for (int j = 0; j < m_maxBallNumber; j++) {
		if (!m_remainedBallList[j]) {
			m_remainedBallList[j] = true;
			t = j;
			break;
		}
	}


	m_ball[t]->rebirth();
	m_ball[t]->setPosition(pos);

	m_remainedBallNumber++;

}

void Kuzushi::beforeStart(bool* isStarted) {
	D3DXVECTOR3 p =*m_player->position();
	float r = m_ball[0]->radius();
	m_ball[0]->setPosition(D3DXVECTOR3(p.x, p.y, p.z + r*2.1f));

	if (Input::instance()->wasKeyPressed('S')) {
		D3DXVECTOR3 v = *m_player->velocity();
		m_ball[0]->setVelocity(D3DXVECTOR3(v.x, 0.f, 10.f));
		*isStarted = true;
		return;
	}
	*isStarted = false;
}

void Kuzushi::debug() {
	Input* i = Input::instance();
	
	if (i->isKeyDown('1')) {
		m_state = State::CLEAR;
	}

	ostringstream oss;
	oss << "remainedBall : " << m_remainedBallNumber;
	Graphics::text()->draw(oss.str().c_str(), 600, 150);
}



void Kuzushi::onLostDevice()const {
	if(m_frame)
	m_frame->onLostDevice();
	if(m_player)
	m_player->onLostDevice();
	for (int j = 0; j < m_maxBlockNumber; j++) {
		if(m_blocks[j])
		m_blocks[j]->onLostDevice();
	}
	for (int j = 0; j < 3; j++) {
		if(m_ball[j])
		m_ball[j]->onLostDevice();
	}
}

void Kuzushi::onResetDevice()const {
	if(m_frame)
	m_frame->onResetDevice();
	if(m_player)
	m_player->onResetDevice();
	for (int j = 0; j < m_maxBlockNumber; j++) {
		if(m_blocks[j])
		m_blocks[j]->onResetDevice();
	}
	for (int j = 0; j < 3; j++) {
		if(m_ball[j])
		m_ball[j]->onResetDevice();
	}
}