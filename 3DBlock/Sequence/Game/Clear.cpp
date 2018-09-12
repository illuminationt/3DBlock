#include "Clear.h"
#include"Image.h"
#include"Utility.h"
#include"Input.h"
#include"Const.h"
#include"Sequence/Game/Load.h"
namespace Sequence {
	namespace Game {

		Clear::Clear()
		{
			m_image = new Image(CLEAR_FILENAME);
		}


		Clear::~Clear()
		{
			SAFE_DELETE(m_image);
		}

		Child* Clear::update(Parent* parent,float dt) {
			Child* next = this;

			m_image->drawWindow();

			if (Input::instance()->wasKeyPressed(VK_SPACE)) {
				next = new Load();
			}

			return next;
		}

	}
}