#include "Reference.h"


Reference::Reference(void):
mCount(0)
{

}

Reference::~Reference(void)
{
}

void Reference::incRef()
{
	mCount++;
}

void Reference::decRef()
{
	mCount--;
	if (mCount <= 0)
		this->deleteThis();
}

void Reference::deleteThis()
{
	delete this;
}


