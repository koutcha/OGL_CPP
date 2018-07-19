#include "StaticCube.h"
#include "Shape.h"
#include "BallObject.h"
#include "ColliderSphere.h"
#include "MathAndPhysic.h"
#include "ColliderCube.h"
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
//collisionresposnse������΂����̂Ō�ŏ���
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

	//�����̓��Ƃ������Ƃɒ��� 1���C���t���Ȃ�����
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
	//��v���ĂȂ��ƃ_��
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
	//�����`����_�ւ̍ŒZ�x�N�g�������߂�
	Vector3f d = MyMath::Vector3AABBToPoint(cubeCol.getPosition(), cubeCol.getHalfSize(), sphereCol.getPosition());
	//�����߂荞��ł��鋗�������߂�
	double sinkLength = sphereCol.getRadius() - d.length();
	d.normalize();
	//d����p�x�N�g���Ƃ���Փˎ��̗͐�(�����`��m=���Av=0�Ƃ���j�����Ƃ߂�
	Vector3f ft = MyMath::calcReflex(ball0->getVelocity(),d, ball0->getWeight(), restitution);
	ball0->addImpluse(ft);
	//�߂荞�݂̏C��
	d *= sinkLength;
	d += ball0->getPosition();
	ball0->setPosition(d);
}

void collisonResponse(const std::shared_ptr<BallObject> ball0, const StaticCube & cube)
{
}
