#include "OrthoCamera.h"
#include "Matrix.h"


OrthoCamera::OrthoCamera():
	Camera(),
	left(-25.0f),
	right(25.0f),
	bottom(-25.0f),
	top(25.0)
{
}


OrthoCamera::OrthoCamera(const Vector3f & position, const Vector3f & up, const Vector3f & gaze, 
	float left, float right, float bottom, float top, float near, float far):
	Camera(position,up,gaze,near,far),
	left(left),
	right(right),
	bottom(bottom),
	top(top)
{

}

OrthoCamera::~OrthoCamera()
{
}

void OrthoCamera::setXRange(float left, float right)
{
	this->left;
	this->right;
}

void OrthoCamera::setYRange(float bottom, float top)
{
	this->bottom = bottom;
	this->top = top;
}

void OrthoCamera::setCamera(GLuint viewLoc, GLuint projectionLoc) const
{

}
