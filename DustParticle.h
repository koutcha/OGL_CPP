#pragma once
#include <array>
#include <memory>
#include "Vector3.h"
#include "Camera.h"
class Shader;
//çªâå
class DustParticle
{
public:
	struct Particle
	{
		Vector3f position;
		Vector3f velocity;
		float scale;
		float time;
	};
	DustParticle(float existTime);
	~DustParticle();
	void generate(const Vector3f& position);
	void update(float dt);
	void draw(const Camera& camera);
private:
	static constexpr unsigned int maxParticles = 100;
	std::array<float,4*maxParticles> positionAndScale;
	std::array<float, 4 * maxParticles> renderingBuffer;
	int last;
	int first;

	unsigned int bufferID;
	unsigned int vao;
	unsigned int program;



	float existTime;

	float maxSize;
	float minSize;

	std::unique_ptr<Shader> shader;
};

