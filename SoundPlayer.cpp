#include "SoundPlayer.h"
#include "inWave.h"
using namespace std;
namespace Sound
{
	SoundPlayer::SoundPlayer():
		data8(nullptr),
		data16(nullptr)
	{
		alGenBuffers(1, &bufferID);
		alGenSources(1, &sourceID);
	}

	SoundPlayer::SoundPlayer(const char * filename)
	{
		alGenBuffers(1, &bufferID);
		alGenSources(1, &sourceID);
		loadWav(filename);
	}


	SoundPlayer::~SoundPlayer()
	{
		alDeleteSources(1, &sourceID);
		alDeleteBuffers(1, &bufferID);
		cout << "kaihou" << endl;
	}


	void SoundPlayer::play() const
	{
		alSourcei(sourceID, AL_BUFFER, bufferID);
		alSourcef(sourceID, AL_GAIN, 1.0);
		alSourcePlay(sourceID);
	}

	void SoundPlayer::play3D(const Vector3f & p) const
	{
		alSourcefv(sourceID, AL_POSITION, p.getArrayData().data());
		play();
	}

	void SoundPlayer::setPosition(const Vector3f& p)
	{
		alSourcefv(sourceID, AL_POSITION, p.getArrayData().data());
	}
	

	void SoundPlayer::loadWav(const char * filename)
	{
		InWave wav(filename);
		if (!wav.isSuccessLoad())
		{
			cout << "error: can't load wav" << endl;
			return;
		}
		unsigned int tempFormatID;
		unsigned int tempBitSize;
		
		pcmFrec = wav.sampleingRate();
		cout << "frec" << pcmFrec << endl;
		tempFormatID = wav.formatID();
		tempBitSize = wav.bitSize();


		long length = wav.length();

		if(tempBitSize == 8)
		{
			cout << "8bit" << endl;

			if (tempFormatID == 1)
			{
				dataSize = length * sizeof(char);
				data8 = move(make_unique<char[]>(length));
				wav.getData(data8.get());
				format = AL_FORMAT_MONO8;
			}
			else
			{
				
				dataSize = length * sizeof(char) * 2;
				unique_ptr<char[]> left(make_unique<char[]>(length));
				unique_ptr<char[]> right(make_unique<char[]>(length));
				wav.getData(left.get(), right.get());
				data8 = std::move(make_unique<char[]>(length * 2));
				for (int i = 0; i < wav.length(); ++i)
				{
					data8[2 * i] = left[i];
					data8[2 * i + 1] = right[i];
				}
				format = AL_FORMAT_STEREO8;
			}

			alBufferData(bufferID, format, data8.get(), dataSize, pcmFrec);
		}
		else if(tempBitSize == 16)
		{
			cout << "16bit" << endl;
			if (tempFormatID == 1)
			{
				cout << "mono" << endl;
				dataSize = length * sizeof(short);
				data16 = move(make_unique<short[]>(length));
				wav.getData(data16.get());
				format = AL_FORMAT_MONO16;
			}
			else
			{
				dataSize = length * sizeof(short) * 2;

				unique_ptr<short[]> left(make_unique<short[]>(length));
				unique_ptr<short[]> right(make_unique<short[]>(length));
				wav.getData(left.get(), right.get());
				data16= std::move(make_unique<short[]>(length * 2));
				for (int i = 0; i < wav.length(); ++i)
				{
					data16[2 * i] = left[i];
					data16[2 * i + 1] = right[i];
				}
				format = AL_FORMAT_STEREO16;
			}

			alBufferData(bufferID, format,data16.get(), dataSize,pcmFrec);
		}

		cout << bufferID << endl;

	}

}
