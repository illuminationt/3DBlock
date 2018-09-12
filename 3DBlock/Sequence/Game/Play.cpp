#include"Sequence/Game/Parent.h"
#include"Sequence/Game/Play.h"
#include"Sequence/Game/Clear.h"
#include"Sequence/Game/Load.h"
#include"Play/Player.h"
#include"Graphics.h"
#include"Input.h"
#include"Root.h"
#include"Const.h"
#include"Play/Kuzushi.h"
#include<sstream>
#include<d3d9.h>
#include<d3dx9.h>
#include<tchar.h>
#include<memory>
#include"Play/Kuzushi.h"

	
namespace Sequence {
	namespace Game {
		Play::Play(){
		}
		Play::~Play(){
		};

		Child* Play::update(Parent* parent,float dt) {
			Child* next = this;

			parent->kuzushi()->update(dt);

			Kuzushi::State s = parent->kuzushi()->state();
			switch (s) {
			case Kuzushi::State::ALIVE:break;
			case Kuzushi::State::DEAD_RETRY:
				next = new Load();
				break;
			case Kuzushi::State::DEAD_TITLE:
				parent->moveTo(Parent::NEXT_TITLE);

			case Kuzushi::State::CLEAR:
				parent->addStageID(1);
				next = new Clear();
				break;
			}



			if (Input::instance()->wasKeyPressed(VK_SPACE)) {
				parent->moveTo(Parent::NEXT_TITLE);
			}

			return next;
		}

	}
}

