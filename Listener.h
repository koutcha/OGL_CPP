#pragma once
#include "Vector3.h"
namespace Sound
{
	class Listener
	{
	public:
		Listener();
		Listener(const Vector3f& position);
		~Listener();
		void setPosition(const Vector3f& postion);
		void setDirection(const Vector3f& at,const Vector3f& up);
	private:
		Vector3f position;
		Vector3f up;
		Vector3f at;
	};

}

