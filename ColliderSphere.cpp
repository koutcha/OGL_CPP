#include "ColliderSphere.h"



ColliderSphere::ColliderSphere():
	ColliderObject(ColliderObject::TYPE_SPHERE)
{
}


ColliderSphere::~ColliderSphere()
{
}

bool ColliderSphere::intersect(const ColliderObject &obj) const
{
	bool result;
	switch (obj.getType())
	{
	case TYPE_LINE:
		break;
	case   TYPE_PLANE:
		result = false;
		break;
	case TYPE_SPHERE:
	{
		Vector3f v(position);
		v -= obj.getPosition();
		float dist = v.length();
		result = dist < (r + obj.getRadius());
	}
		break;
	case TYPE_AABB:
		result =obj.intersect(*this);
		break;

	case TYPE_UNKNOWN:
		result = false;
		break;

	}
	return result;
}

void ColliderSphere::setPosition(const Vector3f &v)
{
	position = v;
}

void ColliderSphere::setRadius(float radius)
{
	r = radius;
}

float ColliderSphere::getRadius() const
{
	return r;
}

const Vector3f ColliderSphere::getPosition() const
{
	return position;
}
