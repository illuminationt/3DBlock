#include "Load.h"
#include"Image.h"
#include"Utility.h"
#include"Const.h"
#include"Sequence/Game/Play.h"
#include"Sequence/Game/Parent.h"
#include"Sequence/Game/StoryPlay.h"


namespace Sequence {
	namespace Game {

		Load::Load():
			m_image(0),
			m_count(0)
		{
			m_image = new Image(LOAD_FILENAME);
		}


		Load::~Load()
		{
			SAFE_DELETE(m_image);
		}

		Child* Load::update(Parent* parent,float dt) {
			Child* next = this;

			m_image->draw(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);
		
			m_count += dt;
			if (m_count > 0.4f) {
				switch (parent->playMode()) {
				case PlayMode::STORY:

					next = new StoryPlay();
					break;

				case PlayMode::NORMAL:
					parent->loadKuzushi();
					next = new Play();
					break;

				}
			}


			return next;
		}
	}
}