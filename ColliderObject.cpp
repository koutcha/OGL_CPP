#include "ColliderObject.h"




ColliderObject::ColliderObject(Type type):
	type(type)
{

}


ColliderObject::~ColliderObject()
{
}

ColliderObject::Type ColliderObject::getType() const
{
	return type;
}
