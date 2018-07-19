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
		GLFW_KEY_ENTER, GLFW_KEY_SPACE, GLFW_KEY_F, GLFW_KEY_G})
{

}

Controller::~Controller()
{
}

void PongGame::Controller::updateFrame()
{

	bool joypadExist = joypad!= nullptr;
	bool windowExist = window!= nullptr;

	bool isConnecting = joypad->checkConnection(joypadID);

	for (int i = 0; i < buttonNumber; ++i) {

		if (windowExist  && window->keyOn(assignKeyID[i]))
		{
			pushingFrame[i]++;
			continue;
		}

		if (joypadExist &&   isConnecting && joypad->isOn(assignButtonID[i], joypadID))
		{

			pushingFrame[i]++;
			continue;
		}

		pushingFrame[i] = 0;
	}

	if (joypadExist && isConnecting)
	{
		int padx = static_cast<int>(joypad->getXAxsis(joypadID)) - static_cast<int>(JoyPad::CENTER_VALUE);
		int padz = static_cast<int>(joypad->getYAxsis(joypadID)) - static_cast<int>(JoyPad::CENTER_VALUE);

		float padfx = static_cast<float>(padx) / static_cast<float>(JoyPad::CENTER_VALUE);
		float padfz = static_cast<float>(padz) / static_cast<float>(JoyPad::CENTER_VALUE);
		leftStickPos = Vector2f(padfx, padfz);

		padx = static_cast<int>(joypad->getZAxsis(joypadID)) - static_cast<int>(JoyPad::CENTER_VALUE);
		padz = static_cast<int>(joypad->getRAxsis(joypadID)) - static_cast<int>(JoyPad::CENTER_VALUE);

		padfx = static_cast<float>(padx) / static_cast<float>(JoyPad::CENTER_VALUE);
		padfz = static_cast<float>(padz) / static_cast<float>(JoyPad::CENTER_VALUE);
		rightStickPos = Vector2f(padfx, padfz);
	}
	
}
void PongGame::Controller::setID(int id)
{
	joypadID = id;
}
void PongGame::Controller::assignButtons(const std::array<JoyPad::Buttons, buttonNumber>& buttonList)
{
	assignButtonID = buttonList;
}
void PongGame::Controller::assignKeys(const std::array<unsigned int, buttonNumber>& openGLKeyID)
{
	assignKeyID = openGLKeyID;
}
void PongGame::Controller::assignButton(Operate operate, JoyPad::Buttons button)
{
	assignButtonID[operate] = button;
}

void PongGame::Controller::assignKey(Operate operate, unsigned int openGLKeyID)
{
	assignKeyID[operate] = openGLKeyID;
}


bool PongGame::Controller::getOperateIsOn(Operate operate)const
{
	return pushingFrame[operate] > 0;
}

unsigned int PongGame::Controller::getOperateingFrames(Operate operate)const
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
bool PongGame::Controller::checkConnection()const
{
	return joypad->checkConnection(joypadID);
}