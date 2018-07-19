#pragma once
#include <memory>
namespace PongGame
{
	class GameMain;
	class GameState
	{
	public:
		GameState();
		virtual ~GameState();
		virtual std::unique_ptr<GameState> update(GameMain& gmain)  =0;
	};
}


