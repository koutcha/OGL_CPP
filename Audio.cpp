#include "Audio.h"
#include "OpenAL\al.h"
#include "OpenAL\alc.h"
#include <iostream>
using namespace std;
namespace Sound {
	Audio::Audio():
		device(nullptr),
		context(nullptr)
	{
		device.reset(alcOpenDevice(nullptr), [](ALCdevice* device) {alcCloseDevice(device);  cout << "device del" << endl; });
		context.reset(alcCreateContext(device.get(), nullptr), [](ALCcontext* context) {alcDestroyContext(context); cout << "context del" << endl;  });
		alcMakeContextCurrent(context.get());
	}


	Audio::~Audio()
	{
		alcMakeContextCurrent(nullptr);
	}
}


