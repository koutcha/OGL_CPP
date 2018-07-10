#pragma once
#include "BallObject.h"
class Shape;
class BallInGame :
	public BallObject
{

public:
	enum Set
	{
		RED_SET,
		BLUE_SET,
		//ÉvÉåÉCÉÑÅ[ÇÃã Ç∆Ç©
		SPECIAL,
	};

	BallInGame(shared_ptr<Shape> shape, float Radius, Set set);
	~BallInGame();
	Set getInSet()const;
	void setIsActive(bool flag);
	bool getIsActive() const;
private:
	bool isActive;
	Set set;

};
void collisionResponseForGameBall(const shared_ptr<BallInGame> ball0, const shared_ptr<BallInGame> ball1);
