#pragma once
#include "GameState.h"
#include "GameMain.h"
#include <memory>
#include<array>


class StaticObject;
class BallObject;
class DynamicCube;
class ShadowFBO;
class MainLight;
class PerspectiveCamera;
class Skybox;
class StaticCube;
namespace PongGame
{
	class GameResourceManager;

	class Play:public GameState
	{
	public:
		enum VSmode
		{
			VS_AI,
			VS_PLAYER
		};
		Play();
		Play(VSmode mode);
		~Play();
		std::unique_ptr<GameState> update(GameMain&) override;
	private:
		class State
		{
		public:
			State() {};
			virtual ~State() {};
			virtual std::unique_ptr<Play::State> update(Play& parent,GameMain&) = 0;
			virtual void enter(Play& play) {};
		};
		class Ready :public State
		{
		public:
			std::unique_ptr<Play::State> update(Play& parent, GameMain& gmain)override;
			void enter(Play& play)override;
		private:
			int count;
		};
		class Playing :public State
		{
		public:
			std::unique_ptr<Play::State> update(Play& parent, GameMain& gmain)override;
			void enter(Play& play)override;
		private:
			int nextCount;
		};
		class EndMatch :public State
		{


		public:
			EndMatch(int winner);
			~EndMatch();
			std::unique_ptr<Play::State> update(Play& parent, GameMain& gmain)override;
		private:
			int count;
			int winner;
		};
		class Exit :public State
		{
		public:
			std::unique_ptr<Play::State> update(Play& parent, GameMain& gmain)override;
		};

		std::unique_ptr<Play::State> state;
		
		std::unique_ptr<PerspectiveCamera> camera;
		std::unique_ptr<MainLight> mainLight;
		std::unique_ptr<Skybox> skybox;
		std::unique_ptr<ShadowFBO> shadowMapFBO;
		std::unique_ptr<GameResourceManager> resource;

		std::shared_ptr<BallObject> ball;
		std::array<std::shared_ptr<DynamicCube>, 2> paddles;
		std::shared_ptr<StaticCube> floor;

		std::array<std::shared_ptr<StaticCube>, 2> LFwall;
		std::array<std::shared_ptr<StaticCube>, 2> UDwall;

		std::array<float, 2> size;
		std::array<int, 2> score;

		int matchPoint;
		int count = 0;

		GameMain::StateIndex requestedState;
		VSmode vsMode;
		void generalRender()const;
		void scoreRender()const;
		void inputForPaddles(GameMain& gmain);
		void changeStateRequest(PongGame::GameMain::StateIndex index);
	
	};
	//物理シミュレーションしない衝突応答
	void collisionResponseBallAndCubeForPong(const std::shared_ptr<BallObject> ball0, const std::shared_ptr<DynamicCube>& cube);

}
