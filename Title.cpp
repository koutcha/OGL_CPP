#include "Title.h"
#include "GameMain.h"
#include "Controller.h"
#include "Play.h"
#include "GameResourceManager.h"
#include "Font.h"
#include"GL\glew.h"
#include <iostream>

using namespace PongGame;
using namespace std;
Title::Title():
	seq(SEQ_TITLE_MENU),
	count(0),
	cursur(MENU_VS_CPU),
	resource(make_unique<GameResourceManager>()),
	controllerMenuCusur(0),
	p1TempID(0),
	p2TempID(1),
	configMenuItems({ " P1 joypad ID:","  P2 joypad ID : ","  Exit Menu" })
{
	std::shared_ptr<Font> font = make_shared<Font>("FreeSans.ttf");
	font->init();
	resource->add("font", font);
	modeName = { string("VS CPU"),string("VS PLAYER"),string("CONTROLLER SETTING") };
}


Title::~Title()
{
}

std::unique_ptr<GameState> PongGame::Title::update(GameMain& gmain)
{
	glClearColor(0.0f, 0.0f, 1.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	const Controller& ct = gmain.getControllerRef(GameMain::CONTROLLER_P1);
	const Controller& ct2 = gmain.getControllerRef(GameMain::CONTROLLER_P2);
	Font& font = *resource->getFont("font");
	std::array<float,2> size = gmain.getWindowSize();

	GLfloat color[] = { 1,1,1 };
	GLfloat red[] = { 1,0,0 };

	auto f = [](int a, int b)
	{
		if (a >= b)
		{
			return  0;
		}
		else if (a < 0)
		{

			return b - 1;
		}
		else
		{
			return a;
		}
	};


	switch(seq)
	{
	case SEQ_TITLE_MENU:
	{
		int menuFontY = 300;
		for (int i = 0; i < modeNum; ++i)
		{
			
			if (cursur == i)
			{
				font.renderText("#"+modeName[i], 180, menuFontY, 1.3, color, size[0], size[1]);
			}
			else
			{
				font.renderText(modeName[i], 200, menuFontY, 1.3, color, size[0], size[1]);
			}
			
			menuFontY -= 100;
		}
		
		int cursurInt = static_cast<int>(cursur);
		if (ct.getOperateingFrames(Controller::DOWN) == 1)
		{
			cursurInt++;
		}
		else if (ct.getOperateingFrames(Controller::UP) == 1)
		{
			cursurInt--;
			
		}
		cursurInt = f(cursurInt, modeNum);
		cursur = static_cast<CursurIndicates>(cursurInt);
		if (ct.getOperateingFrames(Controller::START) == 1)
		{
			if (cursur == MENU_CONTROLLER_CONFIG)
			{
				seq = SEQ_CONTROLLER_CONFIG;
			}
			else
			{
				seq = SEQ_WITING_GAME_SEQ;
			}
			
		}

	}
	break;
	case SEQ_CONTROLLER_CONFIG:
	{
		int conectNum = gmain.getContllorNumber();
		font.renderText("connecting joypad:" + std::to_string(conectNum), 200, 600, 1.0, color, size[0], size[1]);

		if (conectNum == 0)
		{

		}
		else if (conectNum == 1)
		{


			//std::string check0("P1 :");

			//ct.checkConnection() ? check0 += "connected" : check0 += "disconected";
			//font.renderText(check0, 200, 300, 1.0, color, size[0], size[1]);


		}
		else if (conectNum > 1)
		{
			if (ct.getOperateingFrames(Controller::UP) == 1)
			{
				controllerMenuCusur--;
			}
			else if (ct.getOperateingFrames(Controller::DOWN) == 1)
			{
				controllerMenuCusur++;
			}

			controllerMenuCusur = f(controllerMenuCusur, configMenuItems.size());

			switch (controllerMenuCusur)
			{
			case 0:
				if (ct.getOperateingFrames(Controller::LEFT) == 1)
				{
					p1TempID++;
				}
				else if (ct.getOperateingFrames(Controller::RIGHT) == 1)
				{
					p1TempID--;
				}
				p1TempID = f(p1TempID, conectNum);
				break;
			case 1:
				if (ct.getOperateingFrames(Controller::LEFT) == 1)
				{
					p2TempID++;
				}
				else if (ct.getOperateingFrames(Controller::RIGHT) == 1)
				{
					p2TempID--;
				}
				p2TempID = f(p2TempID, conectNum);
				break;
			case 2:
				if (ct.getOperateingFrames(Controller::START) == 1)
				{
					seq = SEQ_TITLE_MENU;
				}
			default:
				break;
			}

		}
		for (int i = 0; i < configMenuItems.size(); ++i)
		{
			if (i == controllerMenuCusur)
			{
				configMenuItems[i][0] = '#';
			}
			else
			{
				configMenuItems[i][0] = ' ';
			}
		}

		font.renderText(configMenuItems[0] +"  " +std::to_string(p1TempID), 200, 500, 1.0, color, size[0], size[1]);
		font.renderText(configMenuItems[1] +"  "  +std::to_string(p2TempID), 200, 400, 1.0, color, size[0], size[1]);
		font.renderText(configMenuItems[2], 200, 100, 1.0, color, size[0], size[1]);

		if (p1TempID == p2TempID)
		{
			font.renderText(" warning: use same id's" + p2TempID, 200, 0, 1.0, red, size[0], size[1]);
		}



		gmain.setContllorID(GameMain::CONTROLLER_P1, p1TempID);
		gmain.setContllorID(GameMain::CONTROLLER_P2, p2TempID);


		std::string check0("P1 :");
		std::string check1("P2 :");

		ct.checkConnection() ? check0 += "connected" : check0 += "disconected";
		ct2.checkConnection() ? check1 += "connected" : check1 += "disconected";

		font.renderText(check0, 200, 300, 1.0, color, size[0], size[1]);
		font.renderText(check1, 200, 200, 1.0, color, size[0], size[1]);
	}
		break;
	case SEQ_WITING_GAME_SEQ:
	{
		count++;
			if (count > 60)
			{
				switch (cursur)
				{
				case MENU_VS_CPU:
					return make_unique<Play>(Play::VS_AI);
					break;
				case MENU_VS_PLAYER:
					return make_unique<Play>(Play::VS_PLAYER);
				default:
					seq = SEQ_TITLE_MENU;
					return nullptr;
				}

			}
		}
		break;
	}
	return nullptr;
	
}
