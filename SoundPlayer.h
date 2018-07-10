#pragma once
#include <memory>
#include "OpenAL\al.h"
#include "OpenAL\alc.h"
namespace Sound
{
	class SoundPlayer
	{
	public:
		SoundPlayer();
		SoundPlayer(const char* filename);
		~SoundPlayer();
		void play()const;
		void loadWav(const char* filename);
	private:

		ALuint bufferID;
		ALuint sourceID;
		ALuint format;
		ALuint dataSize;
		size_t pcmFrec;
		unsigned int length;

		std::unique_ptr<short[]> data16;
		std::unique_ptr<char[]> data8;

	
	};

}


