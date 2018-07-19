#include "ParticleTest.h"
#include "Vector3.h"
#include <array>
#include <random>
#include <iostream>
#include "PerspectiveCamera.h"
#include "InitShader.h"
#include "Textureh.h"
#include "SolidShapeIndex.h"
#include "Matrix.h"

using namespace std;
ParticleTest::ParticleTest() :
	particleVertex({
	-0.5f, -0.5f, 0.0f,
	0.5f, -0.5f, 0.0f,
	-0.5f, 0.5f, 0.0f,
	0.5f, 0.5f, 0.0f,
		}),
	rnd(0),
	program(0)
{
	//shape = SolidShapeIndex::createSphereShape(10,10,1.0);
	cout << "init" << endl;
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);
	//glGenBuffers(1, &vertexBuffer);
	//glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
	//glBufferData(GL_ARRAY_BUFFER, sizeof(particleVertex), particleVertex.data(), GL_STATIC_DRAW);

	glGenBuffers(1, &positionBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, positionBuffer);
	// 空の(NULL)バッファで初期化します。各フレームで後で更新します。
	glBufferData(GL_ARRAY_BUFFER, maxParticles * 4 * sizeof(GLfloat), NULL, GL_STREAM_DRAW);

	glGenBuffers(1, &colorBuffer);
	glBindBuffer(GL_ARRAY_BUFFER,colorBuffer);
	//空の(NULL)バッファで初期化します。各フレームで後で更新します。
	glBufferData(GL_ARRAY_BUFFER, maxParticles * 4 * sizeof(GLubyte), NULL, GL_STREAM_DRAW);



	//インスタンシング
	//attribarrayをずらさないと不具合が出るのでずらす（4~6) 0~3は通常のレンダリングで使っている
	//vaoを使っていないせいだった模様、vaoを追加

	//// 1rst attribute buffer : vertices
	//glEnableVertexAttribArray(0);
	//glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
	//glVertexAttribPointer(
	//	0,                  // attribute. No particular reason for 0, but must match the layout in the shader.
	//	3,                  // size
	//	GL_FLOAT,           // type
	//	GL_FALSE,           // normalized?
	//	0,                  // stride
	//	(void*)0            // array buffer offset
	//);

	// 2nd attribute buffer : positions of particles' centers
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, positionBuffer);
	glVertexAttribPointer(
		0,                                // attribute. No particular reason for 1, but must match the layout in the shader.
		4,                                // size : x + y + z + size => 4
		GL_FLOAT,                         // type
		GL_FALSE,                         // normalized?
		0,                                // stride
		(void*)0                          // array buffer offset
	);

	// 3rd attribute buffer : particles' colors
	glEnableVertexAttribArray(1);
	glBindBuffer(GL_ARRAY_BUFFER, colorBuffer);
	glVertexAttribPointer(
		1,                                // attribute. No particular reason for 1, but must match the layout in the shader.
		4,                                // size : r + g + b + a => 4
		GL_FLOAT,                 // type
		GL_FALSE,                          // normalized?    *** YES, this means that the unsigned char[4] will be accessible with a vec4 (floats) in the shader ***
		0,                                // stride
		(void*)0                          // array buffer offset
	);

//	glVertexAttribDivisor(0, 0); // particles vertices : always reuse the same 4 vertices -> 0
//	glVertexAttribDivisor(1, 1); // positions : one per quad (its center)                 -> 1
//	glVertexAttribDivisor(2, 1); // color : one per quad                                  -> 1

	glVertexAttribDivisor(0, 1);
	glVertexAttribDivisor(1, 1);

	glBindVertexArray(0);





	//program = loadProgram("particle.vert", "particle.frag");
	program = loadProgram("geoTest.vert", "geoTest.geom", "geoTest.frag");
	
	for (int i = 0; i<maxParticles; i++) {
		objects[i].life = -1.0f;
		objects[i].distanceToCamera = -1.0f;
	}

	
}

ParticleTest::~ParticleTest()
{

	glDeleteBuffers(1, &vao);
	//glDeleteBuffers(1, &vertexBuffer);
	glDeleteBuffers(1, &positionBuffer);
	//glDeleteBuffers(1, &colorBuffer);
	glDeleteProgram(program);

}

void ParticleTest::updateAndDraw(float delta,const PerspectiveCamera& camera)
{

	int newparticles = (int)(delta*10000.0);
	if (newparticles > (int)(0.016f*10000.0))
		newparticles = (int)(0.016f*10000.0);

	for (int i = 0; i<newparticles; i++) {
		int particleIndex = findParticle();
		objects[particleIndex].life = 5.0f; // This particle will live 5 seconds.
		objects[particleIndex].pos = Vector3f(0, 0, 0.0f).getArrayData();

		float spread = 1.5f;
		Vector3f maindir(0.0f, 10.0f, 0.0f);
		// Very bad way to generate a random direction; 
		// See for instance http://stackoverflow.com/questions/5408276/python-uniform-spherical-distribution instead,
		// combined with some user-controlled parameters (main direction, spread, etc)
		Vector3f randomdir(
			(rnd() % 2000 - 1000.0f) / 1000.0f,
			(rnd() % 2000 - 1000.0f) / 1000.0f,
			(rnd() % 2000 - 1000.0f) / 1000.0f
		);

		objects[particleIndex].velocity = (maindir + randomdir * spread).getArrayData();


		// Very bad way to generate a random color
		objects[particleIndex].r = static_cast<float>(rnd() % 255) / 255.f;
		objects[particleIndex].g = static_cast<float>(rnd() % 255) / 255.f;
		objects[particleIndex].b = static_cast<float>(rnd() % 255) / 255.f;
		objects[particleIndex].a = (static_cast<float>(rnd() % 255) / 255.f) / 2.0f + 0.2f;

		objects[particleIndex].size = (rand() % 1000) / 2000.0f + 0.1f;

	}

	// Simulate all particles
	particleCount = 0;

	for (int i = 0; i<maxParticles; i++) {

		ParticleObj& p = objects[i]; // shortcut

		if (p.life > 0.0f) {

			// Decrease life
			p.life -= delta;
			if (p.life > 0.0f) {
				Vector3f velocity(p.velocity);
				Vector3f pos(p.pos);
				// Simulate simple physics : gravity only, no collisions
				velocity += Vector3f(0.0f, -9.81f, 0.0f) * static_cast<float>(delta) * 0.5f;
				pos += velocity * static_cast<float>(delta);
				p.distanceToCamera = (pos - camera.getPosition()).length();
				//ParticlesContainer[i].pos += glm::vec3(0.0f,10.0f, 0.0f) * (float)delta;
				p.velocity = velocity.getArrayData();
				p.pos = pos.getArrayData();
				//pos.debugWrite("aaaaa");
				// Fill the GPU buffer
				positionAndSizeData[4 * particleCount + 0] = pos.x;
				positionAndSizeData[4 * particleCount + 1] = pos.y;
				positionAndSizeData[4 * particleCount + 2] = pos.z;

			
				positionAndSizeData[4 * particleCount + 3] = p.size;

				colorData[4 * particleCount + 0] = p.r;
				colorData[4 * particleCount + 1] = p.g;
				colorData[4 * particleCount + 2] = p.b;
				colorData[4 * particleCount + 3] = p.a;

			

			}
			else {
				
				p.distanceToCamera = -1.0f;
			}

			particleCount++;

		}
	}
	sortParticle();
	glClearColor(0.2, 0.2, 0.2, 0.2);
	glClearDepth(1.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glUseProgram(program);
	camera.setCamera(glGetUniformLocation(program, "view"), glGetUniformLocation(program, "proj"));
	GLuint CameraRight_worldspace_ID = glGetUniformLocation(program, "right");
	GLuint CameraUp_worldspace_ID = glGetUniformLocation(program, "up");

	////cout << CameraRight_worldspace_ID << endl;
	////cout << CameraUp_worldspace_ID << endl;
	//
	Vector3f up = camera.getUp();
	Vector3f right = (camera.getGaze() - camera.getPosition()).cross(up);
	up.normalize();
	right.normalize();

	//glUniform3fv(CameraUp_worldspace_ID, 1, up.getArrayData().data());
	//glUniform3fv(CameraRight_worldspace_ID, 1, right.getArrayData().data());
	//tex.bind(0);
	//glUniform1i(glGetUniformLocation(program, "myTextureSampler"), 0);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glBindVertexArray(vao);
	//毎フレーム初期化、更新
	glBindBuffer(GL_ARRAY_BUFFER, positionBuffer);
	glBufferData(GL_ARRAY_BUFFER, maxParticles * 4 * sizeof(GLfloat), NULL, GL_STREAM_DRAW); // Buffer orphaning, a common way to improve streaming perf. See above link for details.
	glBufferSubData(GL_ARRAY_BUFFER, 0, particleCount * sizeof(GLfloat) * 4, positionAndSizeData.data());

	glBindBuffer(GL_ARRAY_BUFFER, colorBuffer);
	glBufferData(GL_ARRAY_BUFFER, maxParticles * 4 * sizeof(GLfloat), NULL, GL_STREAM_DRAW); // Buffer orphaning, a common way to improve streaming perf. See above link for details.
	glBufferSubData(GL_ARRAY_BUFFER, 0, particleCount * sizeof(GLfloat) * 4, colorData.data());

	//glDrawArraysInstanced(GL_TRIANGLE_STRIP, 0, 4, particleCount);
	glDrawArraysInstanced(GL_POINTS, 0, 4, particleCount);

	glBindVertexArray(0);


}


int ParticleTest::findParticle()
{
	for (int i = lastUse; i < maxParticles; ++i)
	{
		if (objects[i].life < 0)
		{
			lastUse = i;
			return i;
		}
	}

	for (int i = 0; i < lastUse; ++i)
	{
		if (objects[i].life < 0)
		{
			lastUse = i;
			return i;
		}
	}

	return 0;
}

void ParticleTest::sortParticle()
{
	std::sort(objects.begin(),objects.end());
}
