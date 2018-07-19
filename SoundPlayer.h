#pragma once
#include <memory>
#include "OpenAL\al.h"
#include "OpenAL\alc.h"
#include "Vector3.h"
namespace Sound
{
	class SoundPlayer
	{
	public:
		SoundPlayer();
		SoundPlayer(const char* filename);
		~SoundPlayer();
		void play()const;
		void play3D(const Vector3f& position)const;
		void setVolume(float f);
		void setPosition(const Vector3f & p);
		void loadWav(const char* filename);
	private:

		ALuint bufferID;
		ALuint sourceID;
		ALuint format;
		ALuint dataSize;
		size_t pcmFrec;
		unsigned int length;
		bool is3D;

		std::unique_ptr<short[]> data16;
		std::unique_ptr<char[]> data8;

	
	};

}


