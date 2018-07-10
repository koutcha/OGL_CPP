#include "Controller.h"
#include <iostream>
using namespace PongGame;



PongGame::Controller::Controller(const std::shared_ptr<const Window> window,const std::shared_ptr<const JoyPad> joypad,int joypadID):
	window(window),
	joypad(joypad),
	joypadID(joypadID),
	assignButtonID({JoyPad::BUTTON_UP,JoyPad::BUTTON_DOWN,JoyPad::BUTTON_LEFT,JoyPad::BUTTON_RIGHT,
		JoyPad::Button1,JoyPad::Button2,JoyPad::Button3 ,JoyPad::Button4 }),
	assignKeyID({GLFW_KEY_W,GLFW_KEY_S, GLFW_KEY_A, GLFW_KEY_D,
		GLFW_KEY_ENTER, GLFW_KEY_SPACE, GLFW_KEY_F, GLFW_KEY_G}),
	isKeyBoardInvalid(true)
{

}

Controller::~Controller()
{
}

void PongGame::Controller::updateFrame()
{

	//weak_ptr‚Å‚ ‚éˆÓ–¡‚ª‚ ‚é‚Ì‚©
	std::shared_ptr<const JoyPad> tempJoypad = joypad.lock();
	std::shared_ptr<const Window> tempWindow = window.lock();
	bool joypadExist = tempJoypad != nullptr;
	bool windowExist = tempWindow != nullptr;
	//std::cout << joypadExist;
	for (int i = 0; i < buttonNumber; ++i) {
		//std::cout << "exist" << joypadExist << " isOn" << tempJoypad->isOn(assignButtonID[i], joypadID) << std::endl;
		if (joypadExist && tempJoypad->isOn(assignButtonID[i], joypadID))
		{
			//std::cout << "up pad" << std::endl;
			pushingFrame[i]++;
			continue;
		}
		if (windowExist && isKeyBoardInvalid &&tempWindow->keyOn(assignKeyID[i]))
		{
			//std::cout << "up key" << std::endl;
			pushingFrame[i]++;
			continue;
		}

			pushingFrame[i] = 0;
	}
	if (joypadExist)
	{
		//std::cout << "up del" << std::endl;
		int padx = static_cast<int>(tempJoypad->getXAxsis(joypadID)) - static_cast<int>(JoyPad::CENTER_VALUE);
		int padz = static_cast<int>(tempJoypad->getYAxsis(joypadID)) - static_cast<int>(JoyPad::CENTER_VALUE);
		//cout << padx << "," << padz << endl;
		float padfx = static_cast<float>(padx) / static_cast<float>(JoyPad::CENTER_VALUE);
		float padfz = static_cast<float>(padz) / static_cast<float>(JoyPad::CENTER_VALUE);
		leftStickPos = Vector2f(padfx, padfz);

		padx = static_cast<int>(tempJoypad->getZAxsis(joypadID)) - static_cast<int>(JoyPad::CENTER_VALUE);
		padz = static_cast<int>(tempJoypad->getRAxsis(joypadID)) - static_cast<int>(JoyPad::CENTER_VALUE);
		//cout << padx << "," << padz << endl;
		padfx = static_cast<float>(padx) / static_cast<float>(JoyPad::CENTER_VALUE);
		padfz = static_cast<float>(padz) / static_cast<float>(JoyPad::CENTER_VALUE);
		rightStickPos = Vector2f(padfx, padfz);
	}
	
}

void PongGame::Controller::assignButton(Operate operate, JoyPad::Buttons button)
{
	assignButtonID[operate] = button;
}

void PongGame::Controller::assignKey(Operate operate, unsigned int openGLKeyID)
{
	assignKeyID[operate] = openGLKeyID;
}

void PongGame::Controller::disableKeyBoard()
{
	isKeyBoardInvalid = true;
}

void PongGame::Controller::enableKeyBoard()
{
	isKeyBoardInvalid = false;
}

bool PongGame::Controller::getOperateIsOn(Operate operate)
{
	return pushingFrame[operate] > 0;
}

unsigned int PongGame::Controller::getOperateingFrames(Operate operate)
{
	return pushingFrame[operate];
}

const Vector2f PongGame::Controller::getLeftAxis() const
{

	return leftStickPos;
}

const Vector2f PongGame::Controller::getRightAxis() const
{
	return rightStickPos;
}
   