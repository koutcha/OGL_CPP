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

		//éŒ¾‚µ‚½‡‚É‚µ‚½‚©‚ç”jšÊ
		std::shared_ptr<ALCdevice> device;
		std::shared_ptr<ALCcontext> context;

	};


}
