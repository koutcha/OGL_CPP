#pragma once
#include <memory>
#include "OpenAL\al.h"
#include "OpenAL\alc.h"
namespace Sound
{
	class Audio
	{
	public:
		Audio();
		~Audio();
	private:

		//宣言した順にしたから破毀(destructor->context->audio)
		std::unique_ptr<ALCdevice,decltype(&alcCloseDevice)> device;
		std::unique_ptr<ALCcontext,decltype(&alcDestroyContext)> context;

	};


}
