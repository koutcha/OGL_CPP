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

		//�錾�������ɂ�������j��(destructor->context->audio)
		std::unique_ptr<ALCdevice,decltype(&alcCloseDevice)> device;
		std::unique_ptr<ALCcontext,decltype(&alcDestroyContext)> context;

	};


}
