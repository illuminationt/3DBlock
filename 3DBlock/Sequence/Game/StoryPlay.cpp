#include "StoryPlay.h"
#include"Image.h"
#include"Utility.h"
#include"Graphics.h"
#include"Text.h"

namespace {
	const char* story = "";
}

namespace Sequence {
	namespace Game {

		StoryPlay::StoryPlay()
		{
			m_image = new Image("resource/story.png");
		}


		StoryPlay::~StoryPlay()
		{
			SAFE_DELETE(m_image);
		}

		Child* StoryPlay::update(Parent* parent, float dt) {
			Child* next = this;

			//m_image->drawWindow();
			Graphics::text()->drawTrue(story);


			return next;
		}

	}
}