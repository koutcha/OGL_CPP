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

		//宣言した順にしたから破毀
		std::shared_ptr<ALCdevice> device;
		std::shared_ptr<ALCcontext> context;

	};


}
