#include "DustParticle.h"
#include "GL\glew.h"
#include "Shader.h"

DustParticle::DustParticle(float time):
	existTime(time)
{
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);
	glGenBuffers(1, &bufferID);
	glBindBuffer(GL_ARRAY_BUFFER, bufferID);
	glBufferData(GL_ARRAY_BUFFER, maxParticles*4*sizeof(GLfloat),NULL,GL_STREAM_DRAW); 
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, bufferID);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 0, static_cast<char*>(0));

	glVertexAttribDivisor(0, 1);
	glBindVertexArray(0);

	shader = std::make_unique<Shader>("dust.vert", "dust.geom", "dust.frag");
		
}


DustParticle::DustParticle(float existTime)
{
}

DustParticle::~DustParticle()
{
}

void DustParticle::generate(const Vector3f & position)
{
}

void DustParticle::update(float dt)
{
}

void DustParticle::draw(const Camera & camera)
{
	glBindVertexArray(vao);
	glBindBuffer(GL_ARRAY_BUFFER, bufferID);
	glBufferData(GL_ARRAY_BUFFER, maxParticles * 4 * sizeof(GLfloat), NULL, GL_STREAM_DRAW);
	glBufferSubData(GL_ARRAY_BUFFER, 0, particleCount * sizeof(GLfloat) * 4,renderingBuffer.data());
}
