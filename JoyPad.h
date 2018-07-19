#pragma once
#include <memory>
#include "Unocopyable.h"
struct joyinfoex_tag;
typedef joyinfoex_tag JOYINFOEX;
class JoyPad:public UnCopyable {
public:
	enum Buttons {
		Button1 = 0x1,
		Button2 = 0x2,
		Button3 = 0x4,
		Button4 = 0x8,
		Button5 = 0x10,
		Button6 = 0x20,
		Button7 = 0x40,
		Button8 = 0x80,
		Button9 = 0x100,
		Butoon10 = 0x200,

		BUTTON_UP = 0x400,
		BUTTON_LEFT = 0x800,
		BUTTON_DOWN = 0x1000,
		BUTTON_RIGHT = 0x2000,

		NONE =  0xfffff
	};
	enum POVDirection {
		POV_UP = 0,
		POV_UP_LEFT = 4500,
		POV_LEFT = 9000,
		POV_DOWN_LEFT = 13500,
		POV_DOWN = 18000,
		POV_DOWN_RIGHT = 22500,
		POV_RIGHT = 27000,
		POV_UP_RIGHT = 31500,
		POV_NONE = 0xffff
	};
	
	static const uint16_t CENTER_VALUE = 32767;
	JoyPad();
	~JoyPad();

	bool intialize();

	bool isOn(Buttons, int id)const;
	POVDirection getPOVDirection(int i)const;
	uint16_t getXAxsis(int id)const;
	uint16_t getYAxsis(int id)const;
	uint16_t getZAxsis(int id)const;
	uint16_t getRAxsis(int id)const;

	bool checkConnection(int id)const;
	int getJoypadNum()const;
private:
	
	//JoyPad* instance;
	std::unique_ptr<int[]> supoertedId;
	int joyPadNum;

	bool checkIDisValid(int id)const;
};


