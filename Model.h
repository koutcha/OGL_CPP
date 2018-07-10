#pragma once
#include <memory>
#include "Vector3.h"
class SolidShapeIndex;
class Matrix;
class Model
{
	Vector3f position;
	Vector3f rotation;
	Vector3f scaling;
public:
	Model();
	~Model();
	void draw() const;
private:

};

