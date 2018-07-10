#pragma once
#include "GL\glew.h"
#include <memory>

using namespace std;
class Shape;
class BallObject;
class BallCursur
{
public:
	BallCursur(shared_ptr<Shape> shape,shared_ptr<BallObject> ball);
	~BallCursur();
	void setBall(shared_ptr<BallObject> ball);
	void draw(GLuint modelLoc);
private:
	shared_ptr<Shape> shape;
	shared_ptr<BallObject> ball;
	float count;
};

