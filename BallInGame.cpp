#include "BallInGame.h"
#include "Shape.h"
BallInGame::BallInGame(shared_ptr<Shape> shape, float Radius,Set set):
	BallObject(shape,Radius),
	set(set),
	isActive(true)
{

}

BallInGame::~BallInGame()
{
}

BallInGame::Set BallInGame::getInSet() const
{
	return set;
}

void BallInGame::setIsActive(bool flag)
{
	isActive = flag;
}

bool BallInGame::getIsActive() const
{
	return isActive;
}


void collisionResponseForGameBall(const shared_ptr<BallInGame> ball0, const shared_ptr<BallInGame> ball1)
{
	if (ball0->getIsActive() && ball1->getIsActive())
	{
		if (ball0->getInSet() == ball1->getInSet() || ball0->getInSet() == BallInGame::SPECIAL || ball1->getInSet() == BallInGame::SPECIAL)
		{
			collisionResponseForBalls(ball0, ball1);
		}
		else
		{
			ball0->setIsActive(false);
			ball1->setIsActive(false);

			ball0->setVelocity(Vector3f(0.f,0.f,0.f));
			ball1->setVelocity(Vector3f(0.f, 0.f, 0.f));

		}
	}
}
