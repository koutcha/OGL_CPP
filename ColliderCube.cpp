#include "ColliderCube.h"
#include "MathAndPhysic.h"


ColliderCube::ColliderCube():
	ColliderObject(ColliderObject::TYPE_AABB),
	position(0,0,0),
	halfSize(0,0,0)
{

}


ColliderCube::~ColliderCube()
{

}
//–¢ŽÀ‘•‚ ‚è
bool ColliderCube::intersect(const ColliderObject &obj) const
{
	bool result = false;
	switch (obj.getType())
	{
	case TYPE_LINE:
		break;
	case   TYPE_PLANE:
		break;
	case TYPE_SPHERE:
	{
		if (calcLengthToPointSquaring(obj.getPosition()) < obj.getRadius()*obj.getRadius())
			result = true;
	}
	break;
	case TYPE_AABB:
	{
		if (obj.getPosition().x + obj.getHalfSize().x >= position.x - halfSize.x
			&& position.x + halfSize.x >= obj.getPosition().x - obj.getHalfSize().x)
		{
			if (obj.getPosition().y + obj.getHalfSize().y >= position.y - halfSize.y
				&& position.y + halfSize.y >= obj.getPosition().y - obj.getHalfSize().y)
			{
				if (obj.getPosition().z + obj.getHalfSize().z >= position.z - halfSize.z
					&& position.z + halfSize.x >= obj.getPosition().z - obj.getHalfSize().z)
				{
					result = true;
				}
			}
		}
	}
	break;
	case TYPE_UNKNOWN:
		break;

	}
	return result;
}

void ColliderCube::setPosition(const Vector3f &position)
{
	this->position = position;
}

void ColliderCube::setHalfSize(const Vector3f &halfSize)
{
	this->halfSize = halfSize;
}

const Vector3f ColliderCube::getHalfSize() const
{
	return halfSize;
}

const Vector3f ColliderCube::getPosition() const
{
	return position;
}

float ColliderCube::calcLengthToPointSquaring(const Vector3f & p)const
{
	return MyMath::SquaringlengthAABBToPoint(position, halfSize, p);
}
