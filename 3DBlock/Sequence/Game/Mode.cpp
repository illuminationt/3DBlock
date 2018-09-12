#include "Sequence/Game/Mode.h"
#include"Sequence/Game/Parent.h"
#include"Sequence/Game/Load.h"
#include"Image.h"
#include"Input.h"
#include"Const.h"
#include "Utility.h"

namespace Sequence {
	namespace Game {
		Mode::Mode():m_image(0)
		{
			m_image = new Image(MODE_FILENAME);
		}


		Mode::~Mode()
		{
			SAFE_DELETE(m_image);
		}

		Child* Mode::update(Parent* parent, float dt) {
			Child* next = this;

			m_image->drawWindow();

			Input* i = Input::instance();
			if (i->wasKeyPressed('Q')) {
				parent->setPlayMode(PlayMode::STORY);
				next = new Load();
			}
			if (i->wasKeyPressed('W')) {
				parent->setPlayMode(PlayMode::NORMAL);
				next = new Load();
			}
			return next;

		}
	}
}