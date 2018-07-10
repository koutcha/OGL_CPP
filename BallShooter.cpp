#include "BallShooter.h"
#include "BallObject.h"
#include <iostream>
#include <cmath>
#include"Matrix.h"
#include "Shape.h"

using namespace std;

BallShooter::BallShooter(shared_ptr<Shape> cursorShape, shared_ptr<BallObject> ball):
	ball(ball),
	cursorShape(cursorShape),
	count(0),
	ratio(0.05),
	maxmumSize(10.0),
	minimumSize(5.0),
	direction(0,0.0,1.0),
	size(5.0),
	iniSize(5.0),
	state(STATE_FULL)
{
}

BallShooter::~BallShooter()
{
}

void BallShooter::shotBall()
{
	if (ball == nullptr)
	{
		return;
	}

	switch (state)
	{
	case BallShooter::STATE_EMPTY:
		break;
	case BallShooter::STATE_FULL:
		{
		cout << "shot" << endl;
			Vector3f  force(direction);
			force *= size;
			ball->addImpluse(force);
			size = iniSize;
			state = STATE_EMPTY;
		}
	
		break;
	default:
		break;
	}
	
}

void BallShooter::reloadBall()
{
	if (ball == nullptr)
	{
		return;
	}

	state = STATE_FULL;
}

void BallShooter::setBall(shared_ptr<BallObject> ball)
{
	this->ball = ball;
	state = STATE_FULL;
}

shared_ptr<BallObject> BallShooter::getBallPtr()
{
	return ball;
}


void BallShooter::disAppendBall()
{
	this->ball = nullptr;
	state = STATE_EMPTY;
}



void BallShooter::setSizeRatio(float ratio)
{
	this->ratio = ratio;
}

void BallShooter::setMaxSize(float size)
{
	if (size < 0 ||  size < minimumSize)
	{
		cout << "this size is not valid" << endl;
		return;
	}
	this->maxmumSize = size;
}

void BallShooter::setMinSize(float size)
{
	if (size < 0 || maxmumSize < size) 
	{
		cout << "this size is not valid" << endl;
		return;
	}
	this->minimumSize = size;
}

void BallShooter::updateDirection()
{
	count += ratio;
	direction.set(cos(count), 0, sin(count));

}

void BallShooter::updateSize()
{
	//decide size from this sin curve
	count += ratio;
	
	float amplitude = maxmumSize - minimumSize;
	float refferencePoint = (maxmumSize + minimumSize) / 2.0;

	size = (amplitude/2.0) * sin(count) + refferencePoint;


}



void BallShooter::draw(GLuint modelLoc)
{

	//cout << size << endl;
	Vector3f position = ball->getPosition();
	

	float theta = std::atan2(direction.x, direction.z);
	float maxValue = maxmumSize * maxmumSize;
	float minValue = minimumSize * minimumSize;

	float maxScale = 3.0;
	float minScale = 1.0;




	//float scalingSize = size * size / 100.0;
	//スケールサイズをminScaleからmaxScaleまで縮小 
	float scalingSize = ((maxScale - minScale)*size*size + (minScale * maxValue-maxScale * minValue )) / (maxValue - minValue);

	const Matrix positionMat(Matrix::translate(position.x, position.y, position.z));
	const Matrix transform(Matrix::translate(0, 0, scalingSize + 1.5));
	const Matrix scaling(Matrix::scale(0.4, 1.0, scalingSize));
	const Matrix rotation(Matrix::rotateY(theta));

	Matrix model(positionMat*rotation);

	model *= transform;
	model *= scaling;

	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, model.data());
	cursorShape->draw();

}
