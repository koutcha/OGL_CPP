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
		Controller(const std::shared_ptr<const Window> window,const std::shared_ptr<const JoyPad> joypad,int joypadID);
		~Controller();
		void updateFrame();

		void assignButton(Operate operate, JoyPad::Buttons button);
		void assignKey(Operate operate, unsigned int openGLKeyID);

		void disableKeyBoard();
		void enableKeyBoard();

		bool getOperateIsOn(Operate operate);
		unsigned int getOperateingFrames(Operate operate);
		//retrun leftStickAxis
		const Vector2f getLeftAxis()const;
		//return rightStickAxis
		const Vector2f getRightAxis()const;
	private:
		static const unsigned int buttonNumber = 8;
		bool isKeyBoardInvalid;
		int joypadID;

		std::array<unsigned int, buttonNumber> assignKeyID;
		std::array<JoyPad::Buttons, buttonNumber> assignButtonID;

		std::array<unsigned int, buttonNumber> pushingFrame;

		std::weak_ptr<const Window> window;
		std::weak_ptr<const JoyPad> joypad;

		Vector2f leftStickPos;
		Vector2f rightStickPos;
	};

}