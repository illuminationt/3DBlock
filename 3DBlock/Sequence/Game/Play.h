#ifndef SEQUENCE_GAME_PLAY_H
#define SEQUENCE_GAME_PLAY_H

#include<d3dx9.h>
#include"Sequence/Game/Child.h"
#include"Play/Kuzushi.h"
class Stage;
class Test;
class Kuzushi;
namespace Sequence {
	namespace Game {

		class Play :public Child {
		public:
			Play();
			virtual ~Play();

			Child* update(Parent* parnt,float dt);

		private:
			
		};
	}
}


#endif