#pragma once
#include "Vector3.h"
#include "Quaternion.h"
namespace Physics
{
	class RealRagid
	{
	public:
		RealRagid();
		~RealRagid();
	private:
		Vector3f velocity;
		Quaternion angular;
	};
}


