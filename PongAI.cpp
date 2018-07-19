#include "PongAI.h"

using namespace PongGame;

PongAI::PongAI()
{
}


PongAI::~PongAI()
{
}

void PongGame::PongAI::update(const BallObject &, const DynamicCube&)
{

}

bool PongGame::PongAI::getOperateIsOn(Controller::Operate operate) const
{
	return false;
}

unsigned int PongGame::PongAI::getOperateingFrames(Controller::Operate operate) const
{
	return 0;
}
