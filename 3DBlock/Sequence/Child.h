#ifndef SEQUENCE_CHILD_H
#define SEQUENCE_CHILD_H


namespace Sequence {

	class Parent;

	class Child
	{
	public:
		virtual Child* update(Parent* parent,float dt) = 0;
		virtual void initialize() {};
		virtual ~Child() {};
	};

}

#endif