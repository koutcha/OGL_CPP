#pragma once

#include "ShapeIndex.h"
#include <memory>
class SolidShapeIndex:
	public ShapeIndex
{
public:
	SolidShapeIndex(GLint size,GLsizei vertexcount,const Object::Vertex *vertex,
		GLsizei indexcount,const GLuint *index);
	virtual ~SolidShapeIndex();
	 void  execute() const override;
	 static std::shared_ptr<Shape>  createSphereShape(int slices,int stacks,float radius);
	 static std::shared_ptr<Shape> createBoxShape();
	 static std::shared_ptr<Shape> createBoxShape(float uvScaleX,float uvScaleY,float uvScaleZ);
	 static std::shared_ptr<Shape>	createPlaneShape(float width,float depth);
	 static std::shared_ptr<Shape> createTriangleShape();
	 

};

