#pragma once
#include "ColliderObject.h"
class ColliderCube :
	public ColliderObject
{
	Vector3f halfSize;
	Vector3f position;
public:
	ColliderCube();
	~ColliderCube();
	bool intersect(const ColliderObject&) const override;
	void setPosition(const Vector3f&)override;
	void setHalfSize(const Vector3f&)override;
	const Vector3f getHalfSize()const override;
	const Vector3f getPosition()const override;
private:
	float calcLengthToPointSquaring(const Vector3f& p)const;
};

