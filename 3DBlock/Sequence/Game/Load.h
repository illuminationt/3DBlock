#ifndef SEQUENCE_GAME_LOAD_H
#define SEQUENCE_GAME_LOAD_H

#include"Sequence/Game/Child.h"
class Image;
namespace Sequence {
	namespace Game {

		class Load:public Child
		{
		public:
			Load();
			virtual ~Load();

			Child* update(Parent* parent,float dt);

		private:
			Image* m_image;
			float m_count;
		};

	}
}
#endif