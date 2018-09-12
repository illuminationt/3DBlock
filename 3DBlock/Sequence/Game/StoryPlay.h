#ifndef SEQUENCE_GAME_STORYPLAY_H
#define SEQUENCE_GAME_STORYPLAY_H

#include"Sequence/Game/Child.h"

class Image;

namespace Sequence {
	namespace Game {

		class StoryPlay :public Child
		{
		public:
			StoryPlay();
			~StoryPlay();

			Child* update(Parent* parent, float dt);
		private:
			Image* m_image;
		};

	}
}
#endif