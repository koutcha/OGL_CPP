#include "Audio.h"
#include "OpenAL\al.h"
#include "OpenAL\alc.h"
#include <iostream>
using namespace std;
namespace Sound {
	Audio::Audio():
		device(alcOpenDevice(nullptr), [](ALCdevice* device) {  cout << "device del" << endl; return alcCloseDevice(device); }),
		context(alcCreateContext(device.get(), nullptr), [](ALCcontext* context) {cout << "context del" << endl; return alcDestroyContext(context);  })
	{
		//カスタムデリータはmake uniqueできない
		alcMakeContextCurrent(context.get());
	}


	Audio::~Audio()
	{
		
		alcMakeContextCurrent(nullptr);
		cout << "delAudio" << endl;
	}
}


