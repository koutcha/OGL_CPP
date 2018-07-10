#pragma once
#include "JoyPad.h"
#include "Window.h"
namespace PongGame
{
	class InputSystemManager
	{
	public:
		InputSystemManager();
		~InputSystemManager();
	private:
		std::weak_ptr<JoyPad> joypad;
		std::weak_ptr<Window> window;
	};

}

