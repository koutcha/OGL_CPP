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

		//éŒ¾‚µ‚½‡‚É‚µ‚½‚©‚ç”jšÊ(destructor->context->audio)
		std::unique_ptr<ALCdevice,decltype(&alcCloseDevice)> device;
		std::unique_ptr<ALCcontext,decltype(&alcDestroyContext)> context;

	};


}
