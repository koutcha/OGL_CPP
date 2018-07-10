#pragma once
#include <memory>
#include "GL\glew.h"
#include "Vector3.h"
using namespace std;
class Shape;
class BallObject;
class BallShooter
{
public:
	BallShooter(shared_ptr<Shape> cursorShape,shared_ptr<BallObject> ball);
	~BallShooter();
	//ボールを発射する
	void shotBall();
	void reloadBall();
	//ボールを割り当てる
	void setBall(shared_ptr<BallObject> ball);
	shared_ptr<BallObject> getBallPtr();
	//ボールの割り当てを外す
	void disAppendBall();

	void setSizeRatio(float ratio);
	void setMaxSize(float size);
	void setMinSize(float size);

	void updateDirection();
	void updateSize();

	void draw(GLuint model);
private:
	enum STATE{
		STATE_EMPTY,
		STATE_FULL,
	};
	shared_ptr<Shape> cursorShape;
	shared_ptr<BallObject> ball;

	Vector3f direction;
	float size;
	float iniSize;

	float count;
	float ratio;

	float minimumSize;
	float maxmumSize;

	STATE state;
};

