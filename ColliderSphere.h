#pragma once
#include "ColliderObject.h"
#include "Vector3.h"
class ColliderSphere :
	public ColliderObject
{
	float r;
	Vector3f position;
public:
	ColliderSphere();
	~ColliderSphere();
	bool intersect(const ColliderObject&) const override;
	void setPosition(const Vector3f&)override;
	void setRadius(float r)override;
	float getRadius()const override;
	const Vector3f getPosition()const override;

};

