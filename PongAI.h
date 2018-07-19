#pragma once
#include "Controller.h"
class BallObject;
class DynamicCube;
namespace PongGame
{
	class PongAI
	{
	public:
		PongAI();
		~PongAI();
		void update(const BallObject&,const DynamicCube&);

		bool getOperateIsOn(Controller::Operate operate)const;
		unsigned int getOperateingFrames(Controller::Operate operate)const;

	private:

	};

}

