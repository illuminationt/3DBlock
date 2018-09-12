#include"Sequence/Child.h"
#include"Sequence/Parent.h"
#include"Sequence/Game/Parent.h"
#include"Sequence/Title.h"
#include"Sequence/Game/Play.h"
#include"Sequence/Game/Load.h"
#include"Sequence/Game/Mode.h"
#include"Root.h"
#include"Const.h"
#include"Utility.h"
#include"Graphics.h"
#include"Text.h"
#include"Play/Kuzushi.h"
#include<ostream>
using namespace std;
namespace Sequence {
	namespace Game {
		
		Parent::Parent():
			m_totalScore(0),
			m_child(0),
			m_next(NEXT_NONE),
			m_kuzushi(0),
			m_stageID(0){
			m_child = new Mode();
		}

		Parent::~Parent() {
			SAFE_DELETE(m_child)
		};

		//このSequence::updateの中で、Sequence::Game::update()を呼び出す。
		//返り値はSequence::Child.

		Sequence::Child* Parent::update(Sequence::Parent* grandparent,float dt) {
			Sequence::Child* next = this;
			Game::Child* nextChild = m_child->update(this,dt);

			if (nextChild != m_child) {
				SAFE_DELETE(m_child);
				m_child = nextChild;
			}

			switch (m_next) {
			case NEXT_TITLE:
				next = new Title();
				break;
			case NEXT_NONE:
				break;
			}

			ostringstream oss;
			oss << "stage : " << m_stageID;
			Graphics::text()->draw(oss.str().c_str(), 600, 80);
			handleDevice();
			return next;
		}

		void Parent::setPlayMode(PlayMode pm) {
			m_playMode = pm;
		}

		const PlayMode Parent::playMode()const {
			return m_playMode;
		}

		void Parent::loadKuzushi() {
			SAFE_DELETE(m_kuzushi);
			m_kuzushi = new Kuzushi(m_stageID);
		}

		void Parent::moveTo(NextSequence ns) {
			m_next = ns;
		}

	}
}