#pragma once
#include "RagidObject.h"
#include <memory>
class ColliderCube;
class BallObject;
class DynamicCube:public RagidObject
{
public:
	//shared_ptr Ç‡Å@const éQè∆Ç≈ìnÇµÇΩÇŸÇ§Ç™ëÅÇ¢ÇÁÇµÇ¢
	DynamicCube(const std::shared_ptr<const Shape>& shape,const Vector3f& halfSize);
	~DynamicCube();
	const ColliderCube& getCollider()const;
	Vector3f getHalfSize()const;
	void setPosition(const Vector3f &p)override;
	void update(float dt)override;
private:
	std::unique_ptr<ColliderCube> collider;

};
void collisionResponseForBallAndCube(const std::shared_ptr<BallObject>& ball0, const std::shared_ptr<DynamicCube>& cube, float restitution);
