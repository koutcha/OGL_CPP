#define _USE_MATH_DEFINES
#include <iostream>
#include <array>
#include<thread>
#include "GL\glew.h"
#include "GLFW\glfw3.h"
#include "GLFW\glfw3native.h"
#include "Window.h"
#include "JoyPad.h"
#include "GameMain.h"
#include "Audio.h"
#include "Listener.h"
#include "SoundPlayer.h"
#include "ballSim.h"
#include "PerspectiveCamera.h"
#include "ParticleTest.h"
#include "Skybox.h"
#include "InitShader.h"
#include "MathAndPhysic.h"
#include "Matrix.h"
#include "SolidShapeIndex.h"
#include "Controller.h"

using namespace std;
using namespace PongGame;


int main()
{
	if (glfwInit() == GL_FALSE)
	{
		std::cerr << "Can't initilize GLFW" << std::endl;
		return 1;
	}
	// register tha program for exit processing
	atexit(glfwTerminate);


	glfwWindowHint(GLFW_SAMPLES, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // To make MacOS happy; should not be needed
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	//FXAA
	glfwWindowHint(GLFW_SAMPLES, 4);


	std::shared_ptr<Window> window(new Window());
	std::shared_ptr<JoyPad> joypad(new JoyPad());
	// Enable depth test
	glEnable(GL_DEPTH_TEST);

	// Accept fragment if it closer to the camera than the former one
   glDepthFunc(GL_LESS);

	// Cull triangles which normal is not towards the camera
	glEnable(GL_CULL_FACE);

	//activate Sound System
	Sound::Audio audio;
	//Sound::Listener;
	//Sound::SoundPlayer sound("dtyd.wav");
	joypad->intialize();


	//fps setting 
	double FPS = 60.0;
	double currentTime = 0.0, lastTime = 0.0, nextTime = 0.0;
	double interval = 1.0 / FPS;
	double dSleep = 0.0;
	int sleep = 0;
	lastTime = 0;
	nextTime = interval;
	float delta = 0;
	GameMain gmain(window, joypad);

	//ballSim sim(*window,*joypad);
	
	while (window->shouldClose() == GL_FALSE)
	{
	
		currentTime = glfwGetTime();
		if (currentTime >= nextTime)
		{
			delta += interval;
			lastTime = currentTime;
			nextTime = currentTime + interval;
			gmain.updateForMainLoop();
			//sim.update();
			window->swapBuffers();
			window->pollEvents();

		}
		else 	if ((dSleep = nextTime - currentTime) >0)
		{
		
			sleep = dSleep * 1000;
			std::this_thread::sleep_for(std::chrono::milliseconds(sleep));
		}

		
	}

	glfwTerminate();

}