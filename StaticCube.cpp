#include "StaticCube.h"
#include "Shape.h"
#include "BallObject.h"
#include "ColliderSphere.h"
#include "MathAndPhysic.h"
StaticCube::StaticCube(std::shared_ptr<Shape> shape, const Vector3f halfSize):
	StaticObject(shape),
	collider(std::make_unique<ColliderCube>())
{
	collider->setHalfSize(halfSize);
	setScale(halfSize);
	setRotation(Vector3f(0, 0, 0));
}

StaticCube::~StaticCube()
{
}

void StaticCube::setPosition(const Vector3f & position)
{
	StaticObject::setPosition(position);
	collider->setPosition(position);
}

const ColliderCube & StaticCube::getCollider() const
{
	return *collider;
}

void correctSpherepos(const std::shared_ptr<BallObject> ball0, const StaticCube& cube)
{
	const ColliderSphere& sphereCol = ball0->getCollider();
	const ColliderCube& cubeCol = cube.getCollider();

	Vector3f boxCenterF = cubeCol.getPosition();
	Vector3f boxSizeF = cubeCol.getHalfSize();
	Vector3d boxCenter(boxCenterF.x, boxCenterF.y, boxCenterF.z);
	Vector3d boxSize(boxSizeF.x, boxSizeF.y, boxSizeF.z);
	//this equal to ball positon
	Vector3f pointF = sphereCol.getPosition();
	Vector3d point(pointF.x, pointF.y, pointF.z);

	Vector3d max(boxCenter.x + boxSize.x, boxCenter.y + boxSize.y, boxCenter.z + boxSize.z);
	Vector3d min(boxCenter.x - boxSize.x, boxCenter.y - boxSize.y, boxCenter.z - boxSize.z);



	int count = 0;
	Vector3d positiveCheck;

	//距離の二乗ということに注意 1日気が付かなかった
	double sqrtlength = 0;
	for (int i = 0; i < 3; ++i)
	{
		positiveCheck.setByIndex(i, 0);

		if (point.getByIndex(i) < min.getByIndex(i))
		{
			sqrtlength += (point.getByIndex(i) - min.getByIndex(i))* (point.getByIndex(i) - min.getByIndex(i));
			positiveCheck.setByIndex(i, -1);
			count++;
		}


		if (point.getByIndex(i) > max.getByIndex(i))
		{
			sqrtlength += (point.getByIndex(i) - max.getByIndex(i))* (point.getByIndex(i) - max.getByIndex(i));
			positiveCheck.setByIndex(i, 1);
			count++;
		}

	}

	double correctLength = sphereCol.getRadius() - sqrt(sqrtlength);
	//if (correctLength < 0) {
		//return;
	//}
	Vector3f tempVelocity(ball0->getVelocity());
	if (count == 1)
	{
		for (int i = 0; i < 3; i++)
		{
			if (positiveCheck.getByIndex(i) != 0) {
				tempVelocity.setByIndex(i, 0.0f);
			}
		}
	}

	ball0->setVelocity(tempVelocity);
	Vector3d temp = positiveCheck;
	double tempLength = 0;
	//一致してないとダメ
	double merikomi = max.y - (point.y - ball0->getRadius());


	temp.normalize();
	
	temp *= correctLength;
	
	//cout << correctLength<< endl;
	temp += point;
	ball0->setPosition(Vector3f(temp.x, temp.y, temp.z));
}

void collisionResponseForBallAndCube(const std::shared_ptr<BallObject> ball0, const StaticCube & cube, float restitution)
{
	const ColliderSphere& sphereCol = ball0->getCollider();
	const ColliderCube& cubeCol = cube.getCollider();

	Vector3f boxCenterF = cubeCol.getPosition();
	Vector3f boxSizeF = cubeCol.getHalfSize();
	Vector3d boxCenter(boxCenterF.x, boxCenterF.y, boxCenterF.z);
	Vector3d boxSize(boxSizeF.x, boxSizeF.y, boxSizeF.z);
	//this equal to ball positon
	Vector3f pointF = sphereCol.getPosition();
	Vector3d point(pointF.x, pointF.y, pointF.z);

	Vector3d max(boxCenter.x + boxSize.x, boxCenter.y + boxSize.y, boxCenter.z + boxSize.z);
	Vector3d min(boxCenter.x - boxSize.x, boxCenter.y - boxSize.y, boxCenter.z - boxSize.z);



	int count = 0;
	Vector3f positiveCheck;
	Vector3f distanceVector;
	//距離の二乗ということに注意 1日気が付かなかった
	double sqrtlength = 0;
	for (int i = 0; i < 3; ++i)
	{
		positiveCheck.setByIndex(i, 0.0);

		if (point.getByIndex(i) < min.getByIndex(i))
		{
			double dist = point.getByIndex(i) - min.getByIndex(i);
			sqrtlength += dist*dist;
			positiveCheck.setByIndex(i,-1.0);
			distanceVector.setByIndex(i, dist);
			count++;
		}


		if (point.getByIndex(i) > max.getByIndex(i))
		{
			double dist = point.getByIndex(i) - max.getByIndex(i);
			sqrtlength += dist*dist;
			positiveCheck.setByIndex(i, 1.0);
			distanceVector.setByIndex(i, dist);
			count++;
		}

	}

	double correctLength = sphereCol.getRadius() - sqrt(sqrtlength);
	//if (correctLength < 0) {
	//return;
	//}
	Vector3f tempVelocity(ball0->getVelocity());
	if (count== 1)
	{
		for (int i = 0; i < 3; i++)
		{
			if (positiveCheck.getByIndex(i) != 0) {
				tempVelocity.setByIndex(i,-tempVelocity.getByIndex(i)*restitution);
			}
		}
		
		ball0->setVelocity(tempVelocity);
	}
	if (count > 1)
	{
		Vector3f ft = MyMath::calcReflex(tempVelocity, positiveCheck, ball0->getWeight(), restitution);
		ball0->addImpluse(ft);
		//cout << "count" << count << endl;
		//Vector3f ft = MyMath::calcReflex(tempVelocity, distanceVector*(-1.0), ball0->getWeight(), restitution);
		//distanceVector.debugWrite("dist");
		//ft.debugWrite("ft");
		//ball0->addImpluse(ft*10);
	}
	
	Vector3f temp(positiveCheck.x, positiveCheck.y, positiveCheck.z);
	double tempLength = 0;
	//一致してないとダメ
	double merikomi = max.y - (point.y - ball0->getRadius());


	temp.normalize();

	temp *= correctLength;

	//cout << correctLength<< endl;
	temp += pointF;
	ball0->setPosition(Vector3f(temp.x, temp.y, temp.z));
}

void collisonResponse(const std::shared_ptr<BallObject> ball0, const StaticCube & cube)
{
}
