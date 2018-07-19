#pragma once
#include "JoyPad.h"
#include "Window.h"
#include "GLFW\glfw3.h"
#include "Vector2.h"
#include <array>
#include "Unocopyable.h"

namespace PongGame
{
	class Controller:public UnCopyable
	{
	
	public:
		enum Operate
		{
			UP = 0,
			DOWN = 1,
			LEFT = 2,
			RIGHT = 3,
			START = 4,
			SELECT = 5,
			ACTION0 = 6,
			ACTION1 = 7
		};
		static const unsigned int buttonNumber = 8;
		Controller(const std::shared_ptr<const Window> window,const std::shared_ptr<const JoyPad> joypad,int joypadID);
		~Controller();
		void updateFrame();
		void setID(int id);

		void assignButtons(const std::array<JoyPad::Buttons, buttonNumber>& buttonList);

		void assignKeys(const std::array<unsigned int, buttonNumber>& openGLKeyID);

		void assignButton(Operate operate, JoyPad::Buttons button);
		void assignKey(Operate operate, unsigned int openGLKeyID);

		bool getOperateIsOn(Operate operate)const;
		unsigned int getOperateingFrames(Operate operate)const;
		//retrun leftStickAxis
		const Vector2f getLeftAxis()const;
		//return rightStickAxis
		const Vector2f getRightAxis()const;
		 bool checkConnection() const;
	private:
		int joypadID;

		std::array<unsigned int, buttonNumber> assignKeyID;
		std::array<JoyPad::Buttons, buttonNumber> assignButtonID;

		std::array<unsigned int, buttonNumber> pushingFrame;

		std::shared_ptr<const Window> window;
		std::shared_ptr<const JoyPad> joypad;

		Vector2f leftStickPos;
		Vector2f rightStickPos;
	};

}