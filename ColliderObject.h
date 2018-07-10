#pragma once
#include "Vector3.h"
class ColliderObject
{
	
public:
	enum Type {
		TYPE_LINE,
		TYPE_PLANE,
		TYPE_SPHERE,
		TYPE_AABB,
		TYPE_UNKNOWN,

	};
	ColliderObject(Type type);
	virtual ~ColliderObject() = 0;
	//shere func
	//衝突判定でダイナミックキャストしたくない
	virtual void setPosition(const Vector3f&) {};
	virtual const Vector3f getPosition()const { return Vector3f(0, 0, 0); };

	virtual void setRadius(float r) {};
	virtual float getRadius()const { return 0; };
	
	virtual void setHalfSize(const Vector3f&) {};
	virtual const Vector3f getHalfSize()const { return Vector3f(0, 0, 0); };


	virtual bool intersect  (const ColliderObject& collider) const =0;

	Type getType()const;
private:
	const Type type;
};

