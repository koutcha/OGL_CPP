#pragma once
#include "GL\glew.h"
#include "Vector3.h"
#include "Uniform.h"
#include "Material.h"
#include <array>
#include <random>
#include <memory>

class Shape;
class PerspectiveCamera;
class Texture;
class Particle
{
public:
	struct ParticleObj
	{
		std::array<float, 3> pos, velocity;
		float r, g, b, a;
		float size;
		float angle;
		float weight;

		float life;
		float distanceToCamera;

		bool operator<(const ParticleObj& particle)
		{
			return this->distanceToCamera > particle.distanceToCamera;
		}
	};
	Particle();
	~Particle();

	void updateAndDraw(float delta, const PerspectiveCamera & camera);

	//void updateAndDraw(float delta, const PerspectiveCamera & KOcamera, const Texture & tex,const Uniform<Material>& mat);
private:
	static constexpr GLuint maxParticles = 300;
	static constexpr GLfloat Lpos[] = { 200.0f, 200.0f, 100.0f};
	static constexpr GLfloat Lamb[] = { 0.4f, 0.4f, 0.4f };
	static constexpr GLfloat Ldiff[] = { 0.7f, 0.7f, 0.7f};
	static constexpr GLfloat Lspec[] = { 1.0f, 1.0f, 1.0f };


	std::array<float, 12> particleVertex;
	std::array<ParticleObj,maxParticles> objects;

	std::array<float, 4*maxParticles> positionAndSizeData;
	std::array<float, 4*maxParticles> colorData;

	std::mt19937 rnd;
	int lastUse = 0;

	GLuint vao;

	GLuint vertexBuffer;
	GLuint positionBuffer;
	GLuint colorBuffer;
	GLuint program;

	GLuint particleCount;
	std::shared_ptr<Shape> shape;

	int findParticle();
	void sortParticle();

	
	
};

