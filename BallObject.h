#pragma once
#include<memory>
#include "RagidObject.h"
using namespace std;
class RagidObject;
class ColliderSphere;
class BallObject:public RagidObject
{
public:
	BallObject(const shared_ptr<const Shape>& shape,float r);
	~BallObject();
	float getRadius()const;
	void setPosition(const Vector3f&) override;
	//�R���C�_�[���Ă�œ����蔻��@���̂��ƏC���@
	const ColliderSphere& getCollider() const;
	void update(float dt) override;

private:
	unique_ptr<ColliderSphere> collider;
	float time;
};

void collisionResponseForBalls(const shared_ptr<BallObject> ball0, const shared_ptr<BallObject> ball1);
//<const A>��<A>�̈Ⴂ�ɋC��t����
void correctShinking(const shared_ptr<BallObject> ball0,const shared_ptr<BallObject> ball1);