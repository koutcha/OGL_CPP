#pragma once
#include"GameState.h"
#include <array>
#include <string>
namespace PongGame
{
	class GameMain;
	class GameResourceManager;
	class Title : public GameState
	{
	public:
		Title();
		~Title();
		std::unique_ptr<GameState> update(GameMain&) override;
	private:
		enum CursurIndicates {
			MENU_VS_CPU = 0,
			MENU_VS_PLAYER = 1,
			MENU_CONTROLLER_CONFIG = 2,
			MENU_BALL_SIMULATAR =3,
		};
		static const int modeNum = 3;
		enum InnerSequence
		{
			SEQ_TITLE_MENU = 0,
			SEQ_CONTROLLER_CONFIG = 1,
			SEQ_WITING_GAME_SEQ = 2
		};
		InnerSequence seq;


		//main menu member
		int count;
		CursurIndicates cursur;
		std::array<std::string, modeNum> modeName;

		//config menu member
		int controllerMenuCusur;
		int p1TempID;
		int p2TempID;
		std::array<std::string, 3> configMenuItems;

		//resource

		std::unique_ptr<GameResourceManager> resource;
		
		
	};
}


