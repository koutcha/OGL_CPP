#pragma once
#include "Controller.h"
#include <memory>
#include <array>


namespace PongGame {
	class GameState;
	class GameResourceManager;
	class GameMain
	{

	public:
		enum ControllerIndex
		{
			CONTROLLER_P1 = 0,
			CONTROLLER_P2 = 1
		};
		enum StateIndex
		{
			TITLE,
			PLAY,
			NONE,
		};
		GameMain(const std::shared_ptr<const Window>& window, const std::shared_ptr<JoyPad>& joypad);
		~GameMain();
		
		void setContllorID(ControllerIndex index, int id);
		int getContllorNumber() const;
		int updateForMainLoop();
		const GameResourceManager& getResourcesRef();
		const Controller& getControllerRef(ControllerIndex index);
		std::array<float, 2> getWindowSize()const;
	private:
		std::shared_ptr<const Window> window;
		std::shared_ptr<JoyPad> joypad;
		std::unique_ptr<GameState> state;
		//共有リソース置き場
		std::unique_ptr<GameResourceManager> resources;
		std::array<std::shared_ptr<Controller>, 2> controllers;

	};
}
