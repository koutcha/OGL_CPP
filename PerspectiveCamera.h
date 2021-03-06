#pragma once
#include "Camera.h"
class PerspectiveCamera :
	public Camera
{
public:
	PerspectiveCamera(float fovy,float aspect);
	//upとpositonとgazeの関係によって外積と内積潰れて表示がおかしくなる場合がある（おそらく基底の次数が減る
	PerspectiveCamera(const Vector3f& position, const Vector3f& up, const Vector3f& gaze, 
		float near, float far,
		float fovy, float aspect);

	~PerspectiveCamera();

	void setCamera(GLuint viewLoc, GLuint projectionLoc) const override;
	Matrix getPVMatrix()const;

private:
	float fovy;
	float aspect;
};

