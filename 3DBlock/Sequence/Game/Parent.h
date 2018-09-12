#ifndef SEQUENCE_GAME_PARENT_H
#define SEQUENCE_GAME_PARENT_H

#include"Sequence/Child.h"
#include"Sequence/Game/Child.h"
//‚±‚ÌParent‚ÍSequence‚ÌŽq‚Å‚ ‚èGame‚ÌeB
//
#include"Play/Kuzushi.h"
class Kuzushi;

namespace Sequence {
	namespace Game {

		enum PlayMode {
			STORY,
			NORMAL,
			NONE,
		};

		class Parent :public Sequence::Child{
		public:
			enum NextSequence {
				NEXT_TITLE,
				NEXT_GAMEOVER,
				NEXT_NONE,
			};
			

			Parent();
			virtual ~Parent();

			Child* update(Sequence::Parent* parent,float dt);
			void setPlayMode(PlayMode);
			const PlayMode playMode()const;
			void loadKuzushi();

			Kuzushi* kuzushi()const { return m_kuzushi; }
			void addStageID(int value) { m_stageID += value; }
			void moveTo(NextSequence);
			void handleDevice()const { if (m_kuzushi) { m_kuzushi->onLostDevice(); m_kuzushi->onResetDevice(); } }
		private:
			unsigned m_totalScore;


		private:
			Game::Child* m_child;
			NextSequence m_next;
			PlayMode m_playMode;

			Kuzushi* m_kuzushi;
			int m_stageID;
		};



	}
}






#endif