#include "Shared.hpp"

RTTI Object::classObject("Object", sizeof(class Object), Object::createObject, NULL, NULL);

Object::Object(void)
{
}


Object::~Object(void)
{
}

Object* Object::createObject()
{
	return new Object;
}

RTTI* Object::getThisClass()
{
	return &classObject;
}

RTTI* Object::getBaseClass()
{
	return classObject.mBaseClass;
}

const char* Object::getClassName()
{
	return "Object";
}

bool Object::operator<<(BinaryStream& bytes)
{
	return true;
}

bool Object::operator >> (BinaryStream& bytes)
{
	return true;
}

