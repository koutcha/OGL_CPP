#pragma once
#include "StaticObject.h"
#include <memory>
#include "Vector3.h"
class BallObject;
class ColliderCube;
class StaticCube :
	public StaticObject
{
public:
	StaticCube(std::shared_ptr<Shape> shape,const Vector3f halfSize);
	~StaticCube();
	void setPosition(const Vector3f& position) override;
	const ColliderCube& getCollider()const;
private:
	std::unique_ptr<ColliderCube> collider;
};
void correctSpherepos(const std::shared_ptr<BallObject> ball0, const StaticCube& cube);
void collisionResponseForBallAndCube(const std::shared_ptr<BallObject> ball0, const StaticCube& cube,float restitution);