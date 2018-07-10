#pragma once
#include "RagidObject.h"
class GameRagid:RagidObject
{
public:
	GameRagid(shared_ptr<Shape> shape);
	~GameRagid();
};

