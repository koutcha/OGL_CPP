#include "JoyPad.h"

#pragma comment(lib, "winmm.lib")
#include <windows.h>
#include "JoyPad.h"
#include <list>
#include <iostream>
#include <algorithm>

using namespace std;

JoyPad::JoyPad():
	supoertedId(0)
{
	

}
JoyPad::~JoyPad()
{
}
bool JoyPad::checkIDisValid(int id) const
{
	
	if (!(joyPadNum >0) ||id > joyPadNum || id < 0)
	{
		
		return false;
	}
	else
	{
		return true;
	}

}

bool JoyPad::intialize()
{
	JOYINFOEX joyInfo;
	joyInfo.dwSize = sizeof(JOYINFOEX);
	joyInfo.dwFlags = JOY_RETURNALL;
	int padNum = 0;
	list<int> idList;
	for (unsigned int i = 0; i<joyGetNumDevs(); i++) {//サポートされているジョイスティックの数を返す
		if (JOYERR_NOERROR == joyGetPosEx(i, &joyInfo)) {
			//printf("ジョイスティック No.%d　接続されています\n", i);
			padNum++;
			idList.push_back(i);
		}
	}
	//パッドがつながってなかったら返す
	if (padNum < 1) {
		joyPadNum = 0;
		//cerr << "no pad is connecting" << endl;
		return false;
	}
	
	joyPadNum = padNum;
	cout << "padNum" << joyPadNum << endl;
	supoertedId = make_unique<int[]>(joyPadNum);
	auto itr = idList.begin();
	for (int i = 0; i < padNum; ++i) {
		supoertedId[i] = *itr;
		//printf("id:%dにNo.%dを登録\n", i, *itr);
		itr++;
	}
	return true;
}

bool JoyPad::isOn(Buttons button, int id)const
{
	JOYINFOEX joyInfo;
	joyInfo.dwSize = sizeof(JOYINFOEX);
	joyInfo.dwFlags = JOY_RETURNALL;
	if (!checkIDisValid(id)) {
		return false;
	}
	unsigned flag = (unsigned)button;
	if (JOYERR_NOERROR == joyGetPosEx(supoertedId[id], &joyInfo)) {
		if (button < 0x201)
		{
			flag = joyInfo.dwButtons & flag;
			if (flag > 0) {
				return true;
			}
			else {
				return false;
			}
		}
		else {
			POVDirection pov = (POVDirection)joyInfo.dwPOV;
			switch (button)
			{
			//cout << "pov" << pov <<endl;
			case BUTTON_UP:
				//cout << "UP" << endl;
				return (pov == POV_UP || pov == POV_UP_LEFT || pov == POV_UP_RIGHT);
			case BUTTON_LEFT:
				return (pov == POV_LEFT || pov == POV_UP_LEFT || pov == POV_DOWN_LEFT);
			case BUTTON_RIGHT:
				return (pov == POV_RIGHT || pov == POV_UP_RIGHT || pov == POV_DOWN_RIGHT);
			case BUTTON_DOWN:
				return (pov == POV_DOWN || pov == POV_DOWN_LEFT || pov == POV_DOWN_RIGHT);
			}
		}

	}
	else {
		//cout << " id " << id << " disconnected" << endl;
		return false;
	}

}



JoyPad::POVDirection JoyPad::getPOVDirection(int id)const
{
	JOYINFOEX joyInfo;
	joyInfo.dwSize = sizeof(JOYINFOEX);
	joyInfo.dwFlags = JOY_RETURNALL;

	if (!checkIDisValid(id)) {
		return POV_NONE;
	}
	if (JOYERR_NOERROR == joyGetPosEx(supoertedId[id], &joyInfo)) {
		return (POVDirection)joyInfo.dwPOV;
	}
	else {
		//cout <<" id " <<id << " disconnected" << endl;
		return POV_NONE;
	}

}

uint16_t JoyPad::getXAxsis(int id)const
{
	JOYINFOEX joyInfo;
	joyInfo.dwSize = sizeof(JOYINFOEX);
	joyInfo.dwFlags = JOY_RETURNALL;
	if (!checkIDisValid(id)) {
		return 32767;
	}
	if (JOYERR_NOERROR == joyGetPosEx(supoertedId[id], &joyInfo)) {
		//cout <<"x"<< joyInfo->dwXpos << endl;
		return joyInfo.dwXpos;

	}
	else {
		//cout << " id " << id << " disconnected" << endl;
		return 32767;
	}
}

uint16_t JoyPad::getYAxsis(int id)const
{
	JOYINFOEX joyInfo;
	joyInfo.dwSize = sizeof(JOYINFOEX);
	joyInfo.dwFlags = JOY_RETURNALL;
	if (!checkIDisValid(id)) {
		return 32767;
	}
	if (JOYERR_NOERROR == joyGetPosEx(supoertedId[id], &joyInfo)) {
		//cout << "y"<<joyInfo->dwYpos << endl;
		return joyInfo.dwYpos;
	}
	else {
		//cout << " id " << id << " disconnected" << endl;
		return 32767;
	}
}
uint16_t JoyPad::getZAxsis(int id)const
{
	JOYINFOEX joyInfo;
	joyInfo.dwSize = sizeof(JOYINFOEX);
	joyInfo.dwFlags = JOY_RETURNALL;
	if (!checkIDisValid(id)) {
		return 32767;
	}
	if (JOYERR_NOERROR == joyGetPosEx(supoertedId[id], &joyInfo)) {
		return joyInfo.dwZpos;
	}
	else {
		//cout << " id " << id << " disconnected" << endl;
		return 32767;
	}
}

uint16_t JoyPad::getRAxsis(int id)const
{
	JOYINFOEX joyInfo;
	joyInfo.dwSize = sizeof(JOYINFOEX);
	joyInfo.dwFlags = JOY_RETURNALL;
	if (!checkIDisValid(id)) {
		return 32767;
	}
	if (JOYERR_NOERROR == joyGetPosEx(supoertedId[id], &joyInfo)) {
		return joyInfo.dwRpos;
	}
	else {
		//cout << " id " << id << " disconnected" << endl;
		return 32767;
	}
}

bool JoyPad::checkConnection(int id)const
{
	JOYINFOEX joyInfo;
	joyInfo.dwSize = sizeof(JOYINFOEX);
	joyInfo.dwFlags = JOY_RETURNALL;
	if (!checkIDisValid(id)) {
		return false;
	}

	if (JOYERR_NOERROR == joyGetPosEx(supoertedId[id], &joyInfo)) {
		return true;
	}
	else
	{
		return false;
	}
}

int JoyPad::getJoypadNum() const
{
	return joyPadNum;
}
