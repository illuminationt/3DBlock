#ifndef SEQUENCE_GAME_CLEAR_H
#define SEQUENCE_GAME_CLEAR_H

#include"Sequence/Game/Parent.h"
class Image;

namespace Sequence {
	namespace Game {
		class Clear:public Child
		{
		public:
			Clear();
			virtual ~Clear();

			Child* update(Parent* parent,float dt);
		private:
			Image* m_image;
		};
	}
}

#endif