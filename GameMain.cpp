#include "GameMain.h"
#include "Controller.h"
#include "GameResourceManager.h"
#include "GameState.h"
#include "Title.h"
#include <iostream>
using namespace PongGame;

GameMain::GameMain(const std::shared_ptr<const Window>& window, const std::shared_ptr<JoyPad>& joypad):
	window(window),
	joypad(joypad),
	controllers({ std::make_shared<Controller>(window,joypad,0),std::make_shared<Controller>(window,joypad,1) }),
	state(make_unique<Title>()),
	resources(make_unique<GameResourceManager>())
{
	controllers[1]->assignKeys({ { GLFW_KEY_UP,GLFW_KEY_DOWN, GLFW_KEY_LEFT, GLFW_KEY_RIGHT,
		GLFW_KEY_ENTER, GLFW_KEY_SPACE, GLFW_KEY_KP_0, GLFW_KEY_KP_1 } });

}


GameMain::~GameMain()
{
}

int PongGame::GameMain::updateForMainLoop()
{
	
	for (auto controller : controllers)
	{
		controller->updateFrame();
	}
	std::unique_ptr<GameState> check = state->update(*this);
	if(check != nullptr)
	{
		cout << "go sequence" << endl;
		state = std::move(check);
	}
	return 0;
}


const GameResourceManager & PongGame::GameMain::getResourcesRef()
{
	return *resources;
}

const Controller & PongGame::GameMain::getControllerRef(ControllerIndex index)
{
	return *controllers[index];
}
void GameMain::setContllorID(ControllerIndex index,int id)
{
	controllers[index]->setID(id);
}
int GameMain::getContllorNumber()const
{
	return joypad->getJoypadNum();
}




std::array<float, 2> PongGame::GameMain::getWindowSize() const
{
	return std::array<float, 2>({ window->getSize()[0], window->getSize()[1] });
}

