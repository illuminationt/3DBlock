#ifndef SEQUENCE_MODE_H
#define SEQUENCE_MODE_H

#include"Sequence/Game/Child.h"

class Image;
namespace Sequence {
	namespace Game {

		class Mode :public Child
		{
		public:
			Mode();
			virtual ~Mode();

			Child* update(Parent* parent, float dt);

		private:
			Image* m_image;
		};

	}
}

#endif