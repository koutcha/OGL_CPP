#define _USE_MATH_DEFINES
#include "Play.h"
#include "GameMain.h"
#include "Controller.h"
#include "Play.h"
#include "BallObject.h"
#include "ShadowFBO.h"
#include "PerspectiveCamera.h"
#include "Skybox.h"
#include "MainLight.h"
#include "StaticObject.h"
#include "SolidShapeIndex.h"
#include "Shader.h"
#include "SoundPlayer.h"
#include "Listener.h"
#include "Textureh.h"
#include "Font.h"
#include "GameResourceManager.h"
#include "DynamicCube.h"
#include "StaticCube.h"
#include "GL\glew.h"
#include "Matrix.h"
#include "MathAndPhysic.h"
#include "Material.h"
#include "ColliderCube.h"
#include "ColliderSphere.h"
#include "Title.h"

#include <iostream>
#include <cmath>
#include <string>
using namespace PongGame;


Play::Play():
	state(make_unique<Play::Ready>()),
	camera(make_unique<PerspectiveCamera>(Vector3f(0,34,10.0),Vector3f(0,0,-1),Vector3f(0,0,0),1.0,300, M_PI / 3.0f,16.0/9.0)),
	mainLight(make_unique<MainLight>(Vector3f(30,100,-50),Vector3f(1.0,1.0,0.7),Vector3f(0.2,0.2,0.2),Vector3f(0.8,0.7,0.7))),
	skybox(make_unique<Skybox>(std::array<std::string, 6>({
			"front.tga",
			"back.tga",
			"up.tga",
			"bottom.tga",
			"right.tga",
			"left.tga"
		}))),
	shadowMapFBO(make_unique<ShadowFBO>()),
	resource(make_unique<GameResourceManager>()),
	score({0,0}),
	matchPoint(3
	),
	requestedState(GameMain::NONE),
	vsMode(VS_AI)
{
	

	skybox->setHalfScale(100);

	resource->add("sphere", SolidShapeIndex::createSphereShape(40, 30, 1.0));

	resource->add("cube0", SolidShapeIndex::createBoxShape(3,3,3));
	resource->add("cube1", SolidShapeIndex::createBoxShape(0.5, 0.5, 3.0));
	resource->add("cube2", SolidShapeIndex::createBoxShape(4.45, 0.5, 0.3));


	resource->add("plane", SolidShapeIndex::createPlaneShape(2.0,2.0));


	resource->add("w", std::make_shared<Texture>("stone.png"));
	resource->add("n", std::make_shared<Texture>("nStone.png"));
	resource->add("toon", std::make_shared<Texture>("toon.png"));

	resource->add("sound", std::make_shared<Sound::SoundPlayer>("kotu.wav"));
	
	std::shared_ptr<Font> font = make_shared<Font>("FreeSans.ttf");
	font->init();
	resource->add("font", font);

	resource->add("general", std::make_shared<Shader>("cel.vert","cel.frag"));


	constexpr Material color[] =
	{
		// Kamb Kdiff Kspec Kshi
		{ 0.2f, 0.2f, 0.2f, 1.0f, 0.2f, 1.0f, 1.0f, 1.0f, 1.0f, 10.0f },
		{ 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 1.0f, 1.0f, 1.0f, 1.0f, 10.0f },
		{ 0.2, 0.2f,  0.2f, 1.0f,1.0f,1.0f , 1.0f,1.0f,1.0f , 50.0f},
	};
	resource->add("material", std::make_shared<Uniform<Material>>(color,3));

	shadowMapFBO->setProjection(Matrix::orthogonal(-50, 50, -50, 50, 1.0f, 200.0f));
	Vector3f Lpos = mainLight->getPostiion();
	shadowMapFBO->setView(Matrix::lookat( Lpos.x / 5, Lpos.y / 5, Lpos.z / 5, 0, 0, 0, 0, 1, 0));
	double radius = 1.;
	ball = make_shared<BallObject>(resource->getShape("sphere"), radius);
	ball->setPosition(Vector3f(0, -2 + radius, 0));


	floor= make_shared<StaticCube>(resource->getShape("cube0"), Vector3f(30, 30, 20));
	floor->setPosition(Vector3f(0, -32, 0));

	paddles[0] = make_shared<DynamicCube>(resource->getShape("cube1"), Vector3f(1.0, 2.0, 4.5));
	paddles[1] = make_shared<DynamicCube>(resource->getShape("cube1"), Vector3f(1.0, 2.0, 4.5));

	paddles[0]->setPosition(Vector3f(-25, 0, 0));
	paddles[1]->setPosition(Vector3f(25, 0, 0));

	LFwall[0] = make_shared<StaticCube>(resource->getShape("cube1"), Vector3f(2, 2, 20));
	LFwall[1] = make_shared<StaticCube>(resource->getShape("cube1"), Vector3f(2, 2, 20));

	LFwall[0]->setPosition(Vector3f(-28, 0, 0));
	LFwall[1]->setPosition(Vector3f(29, 0, 0));

	UDwall[0] = make_shared<StaticCube>(resource->getShape("cube2"), Vector3f(30, 2, 2));
	UDwall[1] = make_shared<StaticCube>(resource->getShape("cube2"), Vector3f(30, 2, 2));

	UDwall[0]->setPosition(Vector3f(0,0,-18));
	UDwall[1]->setPosition(Vector3f(0,0,18));

	state->enter(*this);

}

PongGame::Play::Play(VSmode mode):
	Play()
{
	vsMode = mode;
}


Play::~Play()
{
}
;
std::unique_ptr<GameState> PongGame::Play::update(GameMain& gmain)
{
	
	size = gmain.getWindowSize();

	std::unique_ptr<Play::State> check = state->update(*this, gmain);
	if (requestedState != GameMain::NONE)
	{
		switch (requestedState)
		{
		case GameMain::TITLE:
			return std::make_unique<Title>();
		default:
			break;
		}
	}
	if (check != nullptr)
	{
		state = std::move(check);
		state->enter(*this);
	}
	count++;
	return nullptr;
}

void PongGame::Play::generalRender() const
{
	const Shader& shader = *resource->getShader("general");
	const GLuint model = shader.getUniformLoc("model");
	const Texture& skin = *resource->getTexture("w");
	const Texture& bump = *resource->getTexture("n");
	const Texture& toon = *resource->getTexture("toon");
	const Uniform<Material>& material = *resource->getMaterial("material");
	shadowMapFBO->execute();
	ball->draw(shadowMapFBO->getModelLocation());
	floor->draw(shadowMapFBO->getModelLocation());
	for (auto& x : paddles)
	{
		x->draw(shadowMapFBO->getModelLocation());
	}
	for (int i = 0; i < 2; ++i)
	{
		//LFwall[i]->draw(shadowMapFBO->getModelLocation());
		UDwall[i]->draw(shadowMapFBO->getModelLocation());

	}
	shadowMapFBO->unbind();;

	
	glViewport(0, 0, size[0], size[1]);

	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	glClearColor(1.0, 1.0, 1.0, 1.0);
	glClearDepth(1.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	skybox->draw(*camera);


	shader.use();
	shader.blockBinding("Material", 0);
	camera->setCamera(shader.getUniformLoc("view"), shader.getUniformLoc("projection"));
	camera->setViewPos(shader.getUniformLoc("viewPos"));
	shadowMapFBO->setLightSpaceMatrixToShader(shader.getUniformLoc("lightSpaceMatrix"));
	mainLight->setOnShader(shader);
	skin.bind(1);
	bump.bind(2);
	toon.bind(3);
	shadowMapFBO->setShadowMap(0);
	glUniform1i(shader.getUniformLoc("skin"), 1);
	glUniform1i(shader.getUniformLoc("normalMap"), 2);
	glUniform1i(shader.getUniformLoc("shadowMap"), 0);
	glUniform1i(shader.getUniformLoc("celSample"), 3);


	material.select(0);
	
	int materialCount = 0;
	for (auto& x : paddles)
	{
		material.select(materialCount);
		x->draw(model);
		materialCount++;
	}
	glUniform1i(shader.getUniformLoc("skinTexOn"), true);
	glUniform1i(shader.getUniformLoc("normalMapOn"), true);
	material.select(2);
	ball->draw(model);
	floor->draw(model);
	for (int i = 0; i < 2; ++i)
	{
		UDwall[i]->draw(model);
	}
	glUniform1i(shader.getUniformLoc("skinTexOn"), false);
	glUniform1i(shader.getUniformLoc("normalMapOn"), false);
}

void PongGame::Play::scoreRender() const
{
	Font& font = *resource->getFont("font");
	GLfloat color[] = { 1.0f,1.0f,1.0f };
	font.renderText("P1 :" + std::to_string(score[1]), 50, 50, 3.0, color, size[0], size[1]);
	font.renderText("P2 :" + std::to_string(score[0]), 900, 50, 3.0, color, size[0], size[1]);

}

void PongGame::Play::inputForPaddles(GameMain& gmain)
{
	switch (vsMode)
	{
	case VS_PLAYER:
	{
		for (int i = 0; i < 2; ++i)
		{
			GameMain::ControllerIndex index;
			DynamicCube& paddle = *paddles[i];
			if (i == 0)
			{
				index = GameMain::CONTROLLER_P1;
			}
			else
			{
				index = GameMain::CONTROLLER_P2;
			}
			const Controller& input = gmain.getControllerRef(index);

			if (input.getOperateIsOn(Controller::UP))
			{
				paddle.setVelocity(Vector3f(0, 0, -50));
			}
			else if (input.getOperateIsOn(Controller::DOWN))
			{
				paddle.setVelocity(Vector3f(0, 0, 50));
			}
			else
			{
				paddle.setVelocity(Vector3f(0, 0, 0));
			}

		}
	}
		break;
	case VS_AI:
	{
		GameMain::ControllerIndex index;
		DynamicCube& pPaddle = *paddles[0];

		index = GameMain::CONTROLLER_P1;


		const Controller& input = gmain.getControllerRef(index);

		if (input.getOperateIsOn(Controller::UP))
		{
			pPaddle.setVelocity(Vector3f(0, 0, -50));
		}
		else if (input.getOperateIsOn(Controller::DOWN))
		{
			pPaddle.setVelocity(Vector3f(0, 0, 50));
		}
		else
		{
			pPaddle.setVelocity(Vector3f(0, 0, 0));
		}
		//TO DO :AI のクラス化
		if (count % 1 == 0)
		{
			DynamicCube&  cpuPaddle = *paddles[1];
			Vector3f ballPos(ball->getPosition());
			Vector3f ballVel(ball->getVelocity());
			Vector3f paddlePos(cpuPaddle.getPosition());
			Vector3f paddleSize(cpuPaddle.getHalfSize());
			float ballZ = ballPos.z;
			float paddleZ = paddlePos.z;
			float paddleWidth = paddleSize.z;

			if (ballVel.x < 0)
			{
			
			}
			else
			{

			}
			if (ballZ < paddleZ - paddleWidth)
			{
				cpuPaddle.setVelocity(Vector3f(0, 0, -50));
			}
			else if (ballZ  > paddleZ + paddleWidth)
			{
				cpuPaddle.setVelocity(Vector3f(0, 0, 50));
			}
			else
			{
				cpuPaddle.setVelocity(Vector3f(0, 0, 0));
			}
		}
		}

		break;
	default:
		break;
	}
	


}


void PongGame::Play::changeStateRequest(GameMain::StateIndex index)
{
	requestedState = index;
}

std::unique_ptr<Play::State> PongGame::Play::Ready::update(Play& parent, GameMain&gmain)
{

	parent.inputForPaddles(gmain);
	for (auto& paddle : parent.paddles)
	{
		paddle->update(0.016);
		const ColliderCube& cCube = paddle->getCollider();
		for (int i = 0; i<2; ++i)
		{
			const ColliderCube& wall = parent.UDwall[i]->getCollider();
			if (cCube.intersect(wall))
			{
				Vector3f tempPosition(paddle->getPosition());

				if (i == 0)
				{
					tempPosition.z = wall.getPosition().z + wall.getHalfSize().z + paddle->getHalfSize().z;
					paddle->setPosition(tempPosition);

				}
				else
				{
					tempPosition.z = wall.getPosition().z - wall.getHalfSize().z - paddle->getHalfSize().z;
					paddle->setPosition(tempPosition);
				}
			}
		}
	}
	
	parent.ball->addGravity(Vector3f(0, -40, 0));
	parent.ball->update(0.016);


	const ColliderCube& c = parent.floor->getCollider();
	const ColliderSphere& s = parent.ball->getCollider();

	if (c.intersect(s))
	{
		collisionResponseForBallAndCube(parent.ball, *parent.floor, 0.2);
	}
	Vector3f ballPos = parent.ball->getPosition();
	ballPos.x /= 20.0f;
	ballPos.y  = 0;
	ballPos.z /= 20.0f;
	parent.camera->setGaze(ballPos);

	parent.generalRender();
	parent.scoreRender();

	count++;

	if (count>200)
	{
		return make_unique<Play::Playing>();
	}
	return nullptr;
}

void PongGame::Play::Ready::enter(Play & play)
{
	play.ball->setVelocity(Vector3f(0, 0, 0));
	play.ball->setPosition(Vector3f(0, 40, 0));
	count = 0;
}

std::unique_ptr<Play::State> PongGame::Play::Playing::update(Play& parent, GameMain&gmain)
{
	


	int scoredPlayer = 0;
	parent.inputForPaddles(gmain);

	//-------------------update and collsion --------------------
	for (auto& paddle : parent.paddles)
	{
		paddle->update(0.016);
	}

	parent.ball->addGravity(Vector3f(0, -40, 0));
	parent.ball->update(0.016);
	const Sound::SoundPlayer& se = *parent.resource->getSound("sound");
	for (auto& x : parent.paddles)
	{
		const ColliderCube& cCube = x->getCollider();
		const ColliderSphere& cSphere = parent.ball->getCollider();
		if (cCube.intersect(cSphere))
		{
			
			collisionResponseBallAndCubeForPong(parent.ball, x);

			se.play();

		}

		for (int i=0; i<2; ++i)
		{
			const ColliderCube& wall = parent.UDwall[i]->getCollider();
			if (cCube.intersect(wall))
			{
				Vector3f tempPosition(x->getPosition());
			
				if (i == 0)
				{
					tempPosition.z = wall.getPosition().z + wall.getHalfSize().z + x->getHalfSize().z;
					x->setPosition(tempPosition);
				}
				else
				{
					tempPosition.z = wall.getPosition().z - wall.getHalfSize().z - x->getHalfSize().z;
					x->setPosition(tempPosition);
				}
			}
		}
	}

	for (int i = 0; i<2; ++i)
	{
		const ColliderCube& cCube = parent.LFwall[i]->getCollider();
		const ColliderSphere& cSphere = parent.ball->getCollider();
		if (cCube.intersect(cSphere))
		{
			//collisionResponseForBallAndCube(parent.ball, *parent.LFwall[i], 1.0);
			if (nextCount == 0)
			{
				cout << "scored" << endl;
				parent.score[i]++;
				scoredPlayer = i + 1;
			}
			//se.play();


		}

	}

	for (auto& x : parent.UDwall)
	{
		const ColliderCube& cCube = x->getCollider();
		const ColliderSphere& cSphere = parent.ball->getCollider();
		if (cCube.intersect(cSphere))
		{
			collisionResponseForBallAndCube(parent.ball, *x, 1.0);
			se.play();

		}
	}

	const ColliderCube& c = parent.floor->getCollider();
	const ColliderSphere& s = parent.ball->getCollider();

	if (c.intersect(s))
	{
		collisionResponseForBallAndCube(parent.ball, *parent.floor, 0.0);
	}


	Vector3f ballPos = parent.ball->getPosition();
	ballPos.x /= 20.0f;
	ballPos.z /= 20.0f;
	parent.camera->setGaze(ballPos);

	//rendering
	parent.generalRender();
	parent.scoreRender();

	//sequence
	if (parent.score[0] > parent.matchPoint || parent.score[1] > parent.matchPoint)
	{
		nextCount++;
		if (nextCount > 100)
		{

			scoredPlayer = 0;
			int winner = parent.score[0] > parent.matchPoint ? 2 : 1;
			return make_unique<Play::EndMatch>(winner);

		}

	}
	else if (scoredPlayer > 0 || nextCount>0)
	{
		nextCount++;
		cout << nextCount << endl;
		if (nextCount > 100)
		{
			
			return make_unique<Play::Ready>();
		}

	}
	else if (parent.ball->getPosition().y < -50)
	{
		return make_unique<Play::Ready>();
	}

	return nullptr;
}

void PongGame::Play::Playing::enter(Play & play)
{
	play.ball->setVelocity(Vector3f(30, 0, 0));
	nextCount = 0;
}

PongGame::Play::EndMatch::EndMatch(int winner):
	winner(winner),
	count(0)
{

}

PongGame::Play::EndMatch::~EndMatch()
{
}

std::unique_ptr<Play::State> PongGame::Play::EndMatch::update(Play& parent, GameMain&)
{
	Font& font = *parent.resource->getFont("font");
	GLfloat color[] = { 1.0f,1.0f,1.0f };

	parent.generalRender();
	font.renderText("Player" + std::to_string(winner) + " is Win!!", 50, 500, 3.0, color, parent.size[0], parent.size[1]);
	if (count > 100)
	{
		return make_unique<Play::Exit>();
	}
	count++;
	return nullptr;
}

std::unique_ptr<Play::State> PongGame::Play::Exit::update(Play& parent, GameMain&)
{
	parent.changeStateRequest(GameMain::TITLE);
	return nullptr;
}

void PongGame::collisionResponseBallAndCubeForPong(const std::shared_ptr<BallObject> ball0, const std::shared_ptr<DynamicCube>& cube)
{
	//物理的な正確性は無し

	const ColliderSphere& sphereCol = ball0->getCollider();
	const ColliderCube& cubeCol = cube->getCollider();
	//正方形から点への最短ベクトルを求める
	Vector3f d = MyMath::Vector3AABBToPoint(cubeCol.getPosition(), cubeCol.getHalfSize(), sphereCol.getPosition());
	float restitution = 1.0;
	if (d.length() != 0)
	{
	

		//球がめり込んでいる距離を求める
		double sinkLength = sphereCol.getRadius() - d.length();
		d.normalize();
		Vector3f tempVeloctiy(ball0->getVelocity());
		for (int i = 0; i < 3; ++i)
		{
			if (d.getByIndex(i) != 0.0)
			{
				tempVeloctiy.setByIndex(i,-(tempVeloctiy.getByIndex(i) * restitution));
			}
		}
		ball0->setVelocity(tempVeloctiy);
		//めり込みの修正
		d *= sinkLength;
		d += ball0->getPosition();
		ball0->setPosition(d);
	}
	else
	{
		//中心が内部に入り込んでいた時の処理

		Vector3f vel = ball0->getVelocity();
		Vector3f pos = ball0->getPosition();
		if (vel.x > 0)
		{
			pos.x = cube->getPosition().x - cube->getHalfSize().x - ball0->getRadius() - 0.0000001;
		}
		else
		{
			pos.x = cube->getPosition().x + cube->getHalfSize().x + ball0->getRadius() + 0.0000001;
		}
		ball0->setPosition(pos);
		Vector3f tempVeloctiy(ball0->getVelocity());
		tempVeloctiy.x = -(tempVeloctiy.x*restitution);
		ball0->setVelocity(tempVeloctiy);

	}
	//float maxImpulse = 30.0;
	//float ratio = ball0->getPosition().z - cube->getPosition().z;
	//ratio /= cube->getHalfSize().z*1.1;

	//ball0->addImpluse(Vector3f(0, 0, ratio*maxImpulse));

	float sign = MyMath::sign(cube->getVelocity().z);
	float coefficient = 20.0;
	float friction = sign * coefficient;
	ball0->addImpluse(Vector3f(0, 0, sign*coefficient));
}
