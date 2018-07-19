#define _USE_MATH_DEFINES
#include "ballSim.h"
#include <cmath>
#include "SolidShapeIndex.h"
#include "BallInGame.h"
#include "OctTree.h"
#include "BallCursur.h"
#include "BallShooter.h"
#include "Material.h"
#include "CTMaterial.h"
#include "Textureh.h"
#include "Font.h"
#include "Window.h"
#include "InitShader.h"
#include"PerspectiveCamera.h"
#include "ColliderSphere.h"
#include "MathAndPhysic.h"
#include "ShadowFBO.h"
#include "JoyPad.h"
#include "StaticCube.h"
#include "DynamicCube.h"
ballSim::ballSim(const Window& window,const JoyPad& joypad):
	ballArray(2),
	window(window),
	pad(joypad)
{
	shadowMapObject = std::make_unique<ShadowFBO>();

	shapes.insert(std::make_pair("sphere", SolidShapeIndex::createSphereShape(40, 30, 1.0)));
	shapes.insert(std::make_pair("plane", SolidShapeIndex::createPlaneShape(40, 40)));
	shapes.insert(std::make_pair("triangle",SolidShapeIndex::createTriangleShape()));

	tex.insert(std::make_pair(("toon"), std::make_shared<Texture>("toon.png")));
	tex.insert(std::make_pair(("wall"), std::make_shared<Texture>("wall.jpg")));
	tex.insert(std::make_pair(("nMap"), std::make_shared<Texture>("nMap.png")));

	shaderIDs.insert(std::make_pair("basic", loadProgram("bump.vert", "bump.frag")));
	shaderIDs.insert(std::make_pair("ct", loadProgram("cooktrrance.vert", "cooktrrance.frag")));
	shaderIDs.insert(std::make_pair("cel", loadProgram("cel.vert", "cel.frag")));

	sounds.insert(std::make_pair("stone", std::make_shared<Sound::SoundPlayer>("stone.wav")));

	cube = std::make_shared<StaticCube>(SolidShapeIndex::createBoxShape(),Vector3f(6,6,6));
	dCube = std::make_shared<DynamicCube>(SolidShapeIndex::createBoxShape(), Vector3f(6, 6, 6));

	cube->setPosition(Vector3f(0, 0, 0));
	dCube->setPosition(Vector3f(0, 0, 0));

	font = std::make_unique<Font>("test.ttf");
	font->init();


	wallHalfDepth = 20;
	wallHalfWidth = 20;
	count = 0;

	const std::shared_ptr<Shape> shape = shapes.at("sphere");

	radius = 2.0;
	float x = -17;
	float z = -17;
	ballArray[0] = make_shared<BallInGame>(shape, radius, BallInGame::SPECIAL);
	ballArray[0]->setPosition(Vector3f(3, 15, 0));

	ballArray[0]->setWeight(5.0);
	//ballArray[0]->addRotation(0.75, Vector3f(0, 0, -1));
	x += 5;

	for (int i = 1; i < ballArray.size(); ++i) {
		if (i % 2 == 0)
		{
			ballArray[i] = make_shared<BallInGame>(shape, radius, BallInGame::RED_SET);
		}
		else
		{
			ballArray[i] = make_shared<BallInGame>(shape, radius, BallInGame::RED_SET);
		}
		ballArray[i]->setPosition(Vector3f(x, radius + 9, z));
		ballArray[i]->setVelocity(Vector3f(x, 9, z));
		x += 5;
		if (i % 7 == 0) {
			x = -17;
			z += 0.1;
		}
	}
	
	tree = std::make_unique<OctTree>(ballArray.size(), 4, Vector3f(0, 0, 0), Vector3f(20, 20, 20));

	const std::shared_ptr<Shape> triangle = shapes.at("triangle");
	cursur = std::make_shared<BallCursur>(triangle, ballArray[0]);
	shooter = std::make_shared<BallShooter>(triangle, ballArray[0]);


	shooter->setMaxSize(100.0);
	shooter->setMinSize(20.0);

	constexpr Material color[] =
	{
		// Kamb Kdiff Kspec Kshi
		{ 0.2f, 0.2f, 0.2f, 0.7f, 0.2f, 0.3f, 0.3f, 0.3f, 0.3f, 10.0f },
		{ 0.2f, 0.2f, 0.2f, 0.1f, 0.1f, 0.5f, 0.3f, 0.3f, 0.3f, 10.0f },
		{ 0.2f, 0.2f, 0.2f, 0.2f, 0.7f, 0.3f, 0.3f, 0.3f, 0.3f, 10.0f },
		{ 0.4f, 0.4f, 0.4f, 0.9f, 0.9f, 0.1f, 0.3f, 0.3f, 0.3f, 10.0f },
	};

	constexpr CTMaterial ctColor[] =
	{
		// Kamb Kdiff Kspec m frac
		{ 0.2f, 0.2f, 0.2f, 0.7f, 0.2f, 0.3f, 0.4f, 0.4f, 0.4f,0.4f,0.1f },
	{ 0.4f, 0.4f, 0.2f, 0.1f, 0.1f, 0.5f, 0.4f, 0.4f, 0.4f,0.4f,0.1f },
	{ 0.2f, 0.2f, 0.2f, 0.3f, 0.7f, 0.3f, 0.4f, 0.4f, 0.4f, 0.4f,0.1f },
	{ 0.4f, 0.4f, 0.4f, 0.9f, 0.9f, 0.1f, 0.4f, 0.4f, 0.4f, 0.4f,0.1f },
	};
	material = std::make_unique<Uniform<Material>>(color, 4);
	ctMaterial = std::make_unique<Uniform<CTMaterial>>(ctColor, 4);

	mainCam = std::make_unique<PerspectiveCamera>(Vector3f(0.0f, 1.0f, 30.0f), Vector3f(0, 1, 0), Vector3f(0, 0, -1)
		, 1.0f, 100.0f, M_PI / 3.0f,
		static_cast<float>(window.getSize()[0]) / static_cast<float>(window.getSize()[1]));

	for (auto ball : ballArray)
	{
		ball->setFriction(0.00);
	}
	//window.getScale()*0.01f
	const GLfloat* const size = window.getSize();

	// Dark blue background
	glClearColor(0.4f, 0.4f, 0.4f, 0.0f);

	// Enable depth test
	glEnable(GL_DEPTH_TEST);
	// Accept fragment if it closer to the camera than the former one
	glDepthFunc(GL_LESS);

	intersectCount = 0;
	shooterMode = 0;
	pressedEnter = false;
	camcount = 0;
	bcount = 0;
	program = shaderIDs.at("basic");
}


ballSim::~ballSim()
{
	for (auto program : shaderIDs)
	{
		glDeleteProgram(program.second);
	}
	
}

void ballSim::update()
{
	const GLfloat* const size = window.getSize();
	const GLuint ctShader(shaderIDs.at("ct"));
	const GLuint generalShader(shaderIDs.at("basic"));
	const GLuint celShader(shaderIDs.at("cel"));

	Sound::SoundPlayer& sound = *sounds.at("stone");

	const Texture& tex0 =*tex.at("toon");
	const Texture&  tex1 = *tex.at("wall");
	const Texture& tex2 = *tex.at("nMap");
	const float g = 100.f;
	camcount += 0.000;
	mainCam->setPosition(Vector3f(20 * sin(camcount), 20, 20 * cos(camcount)));
	for (auto& x : ballArray) {
		//x->setPosition(Vector3f(0, 7.0, 0));
		x->addGravity(Vector3f(0, -g, 0));
		x->update(0.016);
		const ColliderSphere& c0 = x->getCollider();
		const ColliderCube& c1 = dCube->getCollider();
		if (c0.intersect(c1))
		{
			collisionResponseForBallAndCube(x, dCube, 1.0);
		}
		MyMath::checkWall(wallHalfWidth, wallHalfDepth, radius, 30.0, *x);
	}


	int counter = 0;
	intersectCount = 0;
	bool soundFrag = false;
	for (auto itr = tree->getPairList().begin(); itr != tree->getPairList().end(); ++itr)
	{
		int index0 = (*itr)[0];
		int index1 = (*itr)[1];

		const ColliderSphere& c0 = ballArray[index0]->getCollider();
		const ColliderSphere& c1 = ballArray[index1]->getCollider();
		counter++;
		if (c0.intersect(c1))
		{
			intersectCount++;
			intersectCount++;
			collisionResponseForGameBall(ballArray[index0], ballArray[index1]);
			if (!soundFrag) {
				sound.play();
				soundFrag = true;
			}
		}

	}


	shooter->updateDirection();
	switch (shooterMode) {
	case 0:
		shooter->updateDirection();
		break;
	case 1:
		shooter->updateSize();
		break;
	case 2:

		if (shooter->getBallPtr()->getVelocity().length() == 0.0f)
		{
			shared_ptr<BallInGame> shooterBall = dynamic_pointer_cast<BallInGame>(shooter->getBallPtr());
			if (shooterBall == nullptr) {
				cerr << "shooterBall has not BallInGame";
				shooter->reloadBall();
				break;
			}

			if (shooterBall->getIsActive())
			{
				cout << "reload" << endl;
				shooter->reloadBall();
			}
			else
			{
				cout << "reloat new ball" << endl;
				for (auto x : ballArray) {
					if (x->getIsActive())
					{
						shooter->setBall(x);
						break;
					}
				}
			}
			shooterMode = 0;
		}
		break;
	default:
		break;
	}
	tree->reset();
	tree->createColliderList(ballArray);
	tree->constructTree();



	Vector3f worldPositon(ballArray[0]->getPosition());
	Vector3f screenPosition(mainCam->getPVMatrix()*worldPositon);


	//---------------------------rendering--------------------------

	//draw shadowMap
	shadowMapObject->setProjection(Matrix::orthogonal(-25, 25, -25, 25, 1.0f, 200.0f));
	shadowMapObject->setView(Matrix::lookat(Lpos[0] / 10, Lpos[1] / 10, Lpos[2] / 10, 0, 0, 0, 0, 1, 0));
	shadowMapObject->execute();
	for (auto x : ballArray) {
		if (x->getIsActive())
		{
			x->draw(shadowMapObject->getModelLocation());
		}
	}
	dCube->draw(shadowMapObject->getModelLocation());
	glUniformMatrix4fv(shadowMapObject->getModelLocation(), 1, GL_FALSE, Matrix::Identity().data());
	shapes.at("plane")->draw();

	shadowMapObject->unbind();

	//draw main graphics
	glViewport(0, 0, window.getSize()[0], window.getSize()[1]);


	glUseProgram(program);
	glUniformBlockBinding(program, glGetUniformBlockIndex(program, "Material"), 0);
	shadowMapObject->setShadowMap(0);
	glUniform1i(glGetUniformLocation(program, "shadowMap"), 0);
	tex0.bind(1);
	glUniform1i(glGetUniformLocation(program, "celSample"), 1);
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	glClearColor(0.0, 0.0, 0.0, 1.0);
	glClearDepth(1.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);



	mainCam->setCamera(glGetUniformLocation(program, "view"), glGetUniformLocation(program, "projection"));
	shadowMapObject->setLightSpaceMatrixToShader(glGetUniformLocation(program, "lightSpaceMatrix"));

	const GLfloat viewPos[] = { mainCam->getPosition().x,mainCam->getPosition().y ,mainCam->getPosition().z };
	glUniform3fv(glGetUniformLocation(program, "viewPos"), 1, viewPos);
	//light
	glUniform3fv(glGetUniformLocation(program, "Lpos"), 1, Lpos);
	glUniform3fv(glGetUniformLocation(program, "Lamb"), 1, Lamb);
	glUniform3fv(glGetUniformLocation(program, "Ldiff"), 1, Ldiff);
	glUniform3fv(glGetUniformLocation(program, "Lspec"), 1, Lspec);



	if (program == ctShader)
	{
		ctMaterial->select(0);
	}
	else
	{
		material->select(0);
	}



	const GLuint shaderModelLoc(glGetUniformLocation(program, "model"));
	tex1.bind(2);
	tex2.bind(3);
	glUniform1i(glGetUniformLocation(program, "skin"), 2);
	glUniform1i(glGetUniformLocation(program, "normalMap"), 3);

	glUniform1i(glGetUniformLocation(program, "skinTexOn"), true);
	glUniform1i(glGetUniformLocation(program, "normalMapOn"), true);
	glUniformMatrix4fv(shaderModelLoc, 1, GL_FALSE, Matrix::Identity().data());
	shapes.at("plane")->draw();

	for (auto x : ballArray) {
		if (x->getIsActive() && x->getInSet() == BallInGame::RED_SET)

			x->draw(shaderModelLoc);
	}
	dCube->draw(shaderModelLoc);
	if (program == ctShader)
	{
		ctMaterial->select(2);
	}
	else
	{
		material->select(2);
	}
	ballArray[0]->draw(shaderModelLoc);



	if (program == ctShader)
	{
		ctMaterial->select(1);
	}
	else
	{
		material->select(1);
	}



	if (program == ctShader)
	{
		ctMaterial->select(3);
	}
	else
	{
		material->select(3);
	}


	if (shooterMode != 2) {
		glDisable(GL_DEPTH_TEST);
		//shooter->draw(shaderModelLoc);
		glEnable(GL_DEPTH_TEST);
	}
	//double currentTime = glfwGetTime();
	//double delta = currentTime - lastTime;
	//lastTime = currentTime;
	//cout << "do it" << endl;
	//particle.updateAndDraw(delta, mainCam, tex1, material);



	//drawFont
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	GLfloat color[] = { 1.0f,1.0f,1.0f };

	//font.renderText(shaderName.c_str(), (1.0+screenPosition.x)*size[0]/2.0-10,(1.0 + screenPosition.y)*size[1]/2.0, 0.5f, color, size[0], size[1]);
	font->renderText(to_string(counter).c_str(), 0, 0, 1.0f, color, size[0], size[1]);
	//font.renderText(to_string(intersectCount).c_str(), 0,40 , 1.0f, color, size[0], size[1]);


	//checkInput
	//shaderName = " ";
	Vector3f test(0, 0, 0);
	if (pad.isOn(JoyPad::Button1, 0))
	{
		bcount++;
		//cout << "on"  << count<< endl;

		ballArray[0]->addImpluse(Vector3f(0, 70, 0));
	}
	int padx = static_cast<int>(pad.getXAxsis(0)) - static_cast<int>(JoyPad::CENTER_VALUE);
	int padz = static_cast<int>(pad.getYAxsis(0)) - static_cast<int>(JoyPad::CENTER_VALUE);
	//cout << padx << "," << padz << endl;
	float padfx = static_cast<float>(padx) / static_cast<float>(JoyPad::CENTER_VALUE);
	float padfz = static_cast<float>(padz) / static_cast<float>(JoyPad::CENTER_VALUE);
	test.x = 100 * padfx;
	test.z = 100 * padfz;

	//ballArray[0]->addForce(test);



	if (window.keyOn(GLFW_KEY_A))
	{
		//program = celShader;
		//shaderName = "celShader";
		dCube->setPosition(Vector3f(5,0,0));
	}
	else if (window.keyOn(GLFW_KEY_S))
	{
		//program = generalShader;
		//shaderName = "standardShader(phong)";
		dCube->setPosition(Vector3f(0, 0, 0));
	}
	else if (window.keyOn(GLFW_KEY_D))
	{
		//program = ctShader;
		//shaderName = "standardShader(cook trrance)";
		dCube->setPosition(Vector3f(-5, 0, 0));
	}
	//shaderName += to_string((double)intersectCount / (double)counter);


	if (window.keyOn(GLFW_KEY_ENTER))
	{
		ballArray[0]->setPosition(Vector3f(-6.4, 15, 6.0000));
		ballArray[0]->setVelocity(Vector3f(0, 0, 0));
		if (!pressedEnter) {
			//alSourcePlay(sourceID);
			//sound.play();
			switch (shooterMode) {
			case 0:
				shooterMode++;
				break;
			case 1:
				shooter->shotBall();
				shooterMode++;
				break;
			case 2:
				//ball0->setVelocity(Vector3(0, 0, 0));

				break;
			default:
				break;
			}
		}
		pressedEnter = true;
	}
	else 
	{
		pressedEnter = false;
	}
	count++;
}
