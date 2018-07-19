#include "Listener.h"
#include "OpenAL\al.h"
#include "OpenAL\alc.h"


Sound::Listener::Listener():
	position(0,0,0),
	up(0,1,0),
	at(0,0,-1)
{
	alListenerfv(AL_POSITION, position.getArrayData().data());
	setDirection(up, at);
}

Sound::Listener::Listener(const Vector3f & position):
	position(position)
{
}


Sound::Listener::~Listener()
{
}


void Sound::Listener::setPosition(const Vector3f& position)
{
	this->position = position;
	alListenerfv(AL_POSITION, position.getArrayData().data());
}

void Sound::Listener::setDirection(const Vector3f & at, const Vector3f & up)
{
	this->at = at;
	this->up = up;
	std::array<ALfloat, 6> orientation = { at.x,at.y,at.z,up.x,up.y,up.z };
	alListenerfv(AL_ORIENTATION, orientation.data());

}
