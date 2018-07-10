#define _USE_MATH_DEFINES
#include <iostream>
#include <array>
#include "GL\glew.h"
#include "GLFW\glfw3.h"
#include "GLFW\glfw3native.h"
#include "Window.h"
#include "JoyPad.h"

#include "Particle.h"
#include "Audio.h"
#include "SoundPlayer.h"
#include "ballSim.h"
#include "PerspectiveCamera.h"
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

	

	joypad->intialize();

	Sound::Audio audio;
	Sound::SoundPlayer test("dtydcvsdfafafafafa.wav");

	GLuint cMap(loadProgram("cubemapping.vert", "cubemapping.frag"));

	//----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

	//Particle particle;
	//double lastTime = 0;
	//ballSim sim(window,pad);
	PerspectiveCamera mainCam(Vector3f(0.0f, 0.0f, 0.0f), Vector3f(0, 1, 0), Vector3f(0, 0, 0)
		, 1.0f, 100.0f, M_PI / 3.0f,
		static_cast<float>(window->getSize()[0]) / static_cast<float>(window->getSize()[1]));

	std::shared_ptr<Shape> shape(SolidShapeIndex::createBoxShape());

	MyMath::calcReflex(Vector3f(0, -1, 0), Vector3f(0, 1, 0), 1.0, 1.0).debugWrite("reflex");
	std::array<std::string,6> faces
	{
		"front.tga",
		"back.tga",
		"up.tga",
		"bottom.tga",
		"right.tga",
		"left.tga"
	};
	Skybox box(faces);
	box.setHalfScale(30);
	float count = 0;
	double lastTime = glfwGetTime();
	cout << "init render" << endl;
	Controller ct(window,joypad,0);

	while (window->shouldClose() == GL_FALSE)
	{
		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		mainCam.setGaze(Vector3f(0,0,
			0));
		mainCam.setPosition(Vector3f(10*sin(count), 0.0,10*cos(count)));
		ct.updateFrame();

		if (window->keyTriggered(GLFW_KEY_ENTER))
		{
			test.play();
		}

		double currentTime = glfwGetTime();
		double delta = currentTime - lastTime;
		lastTime = currentTime;
		//particle.updateAndDraw(delta, mainCam);

			
		//sim.update();
		////swap color buffers and take out events
		box.draw(mainCam);
		glUseProgram(cMap);
		box.bindThisCubeMap(0);
		glUniform1i(glGetUniformLocation(cMap, "cubeTexture"), 0);
		glUniform1i(glGetUniformLocation(cMap, "isReflection"),true);
		glUniform3fv(glGetUniformLocation(cMap, "viewPos"), 1, mainCam.getPosition().getArrayData().data());
		mainCam.setCamera(glGetUniformLocation(cMap,"view"), glGetUniformLocation(cMap,"projection"));
		for (int i = 0; i < 3; ++i)
		{
			for (int j = 0; j < 3; ++j)
			{
				glUniformMatrix4fv(glGetUniformLocation(cMap, "model"), 1, GL_FALSE, (Matrix::translate(-3.0 + i * 3,-3.0 + 3*j, 0)*
					Matrix::rotateY(20)).data());
				shape->draw();
			}

		}
		if (ct.getOperateIsOn(Controller::UP))
		{
			count += 0.01;
		}
		else if (ct.getOperateIsOn(Controller::DOWN))
		{
			count -= 0.01;
		}
		
		window->swapBuffers();
		window->pollEvents();
		
	}

	glfwTerminate();

}