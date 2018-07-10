#define _USE_MATH_DEFINES
#include <cmath>
#include<iostream>
#include "BallCursur.h"
#include "Vector3.h"
#include "Matrix.h"
#include "Shape.h"
#include "BallObject.h"
int rad_to_angle(float ratio)
{
	float rad = ratio / M_PI;
	return 180 * rad;
}
BallCursur::BallCursur(shared_ptr<Shape> shape, shared_ptr<BallObject> ball):
	shape(shape),
	ball(ball),
	count(0.f)
{
	
}

BallCursur::~BallCursur()
{
}

void BallCursur::setBall(shared_ptr<BallObject> ball)
{
	this->ball = ball;
}


void BallCursur::draw(GLuint modelLoc)
{
	count += 0.1f;

	Vector3f position = ball->getPosition();
	//Vector3 base(0, 0, 1);
	Vector3f velocity = ball->getVelocity();
	//v7velocity.normalize();


	float theta = std::atan2(velocity.x,velocity.z);
	std::cout << rad_to_angle(theta);
	const Matrix positionMat(Matrix::translate(position.x,position.y,position.z));
	const Matrix transform(Matrix::translate(0, 0, 2.0));
	const Matrix scaling(Matrix::scale(0.7, 1.0, velocity.length()*velocity.length()/30.0));
	const Matrix rotation(Matrix::rotateY(theta));

	Matrix model(positionMat*rotation);
	
	model *= transform;
	model *= scaling;

	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, model.data());
	shape->draw();

}
