#include"Sequence/Parent.h"
#include"Sequence/Child.h"
#include"Sequence/Game/Parent.h"
#include"Sequence/Title.h"
#include"Const.h"
#include"Root.h"
#include"Utility.h"

namespace Sequence {
	Parent* Parent::m_instance = 0;

	void Parent::create() {
		if (m_instance) {
			warn("already Parend::instance is created!");
		}
		m_instance = new Parent();
	}

	void Parent::destroy() {
		if (!m_instance) {
			warn("instance haven't been created yet");
		}
		SAFE_DELETE(m_instance);
	}

	Parent* Parent::instance() {
		return m_instance;
	}
	void Parent::update(float dt) {
		//update‚µ‚Â‚Ânext‚ð’²’B
		Child* next = m_child->update(this,dt);
		
		if (next != m_child) {
			SAFE_DELETE(m_child);
			m_child = next;
		}
		next = 0;
	}

	Parent::Parent() {
		m_child = new Title();
	}

	Parent::~Parent(){
		SAFE_DELETE(m_child);
	}

}//namespace Sequence