#include "Sequence/Title.h"
#include"Sequence/Game/Child.h"
#include"Sequence/Parent.h"
#include"Sequence/Game/Parent.h"
#include"Input.h"
#include"Root.h"
#include"Image.h"
#include"Const.h"

Image* g_image;
namespace Sequence {

	Title::Title(){
		//m_image=new Image("temporary/boy1.jpg");
	}


	Title::~Title()
	{
	
	}

	Child* Title::update(Parent* parent,float dt) {
		if (!g_image) { g_image = new Image(TITLE_FILENAME); }
		Child* next = this;
		
		g_image->draw(0.f, 0.f, WINDOW_WIDTH, WINDOW_HEIGHT);
		
		Input* i = Input::instance();

		if (i->wasKeyPressed('0')) {
			next = new Game::Parent();
		}

		return next;
	}



}