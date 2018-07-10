#pragma once
#include <map>
#include <string>
#include <memory>
#include <vector>
#include "Uniform.h"
#include "GL\glew.h"
#include "SoundPlayer.h"
using namespace std;
class Shape;
class Material;
class CTMaterial;
class BallInGame;
class OctTree;
class BallCursor;
class Texture;
class Sound::SoundPlayer;
class ShadowFBO;
class PerspectiveCamera;
class BallCursur;
class BallShooter;
class Font;
class Window;
class  JoyPad;
class StaticCube;
class ballSim
{
public:

	ballSim(const Window & window, const JoyPad & joypad);
	~ballSim();
	void update();
private:
	static constexpr GLfloat Lpos[] = { 200.0f, 200.0f, 100.0f };
	static constexpr GLfloat Lamb[] = { 0.4f, 0.4f, 0.4f };
	static constexpr GLfloat Ldiff[] = { 0.7f, 0.7f, 0.7f };
	static constexpr GLfloat Lspec[] = { 1.0f, 1.0f, 1.0f };

	
	const Window& window;
	const JoyPad& pad;

	map<string, shared_ptr<Shape>> shapes;
	map<string, unsigned int> shaderIDs;
	map<string, shared_ptr<Sound::SoundPlayer>> sounds;
	map<string, shared_ptr<Texture>> tex;
	unique_ptr<Uniform<Material>> material;
	unique_ptr<Uniform<CTMaterial>> ctMaterial;

	std::shared_ptr<StaticCube> cube;

	std::vector<std::shared_ptr<BallInGame>> ballArray;
	std::unique_ptr<OctTree> tree;
	std::unique_ptr<ShadowFBO> shadowMapObject;
	std::unique_ptr<PerspectiveCamera> mainCam;
	std::unique_ptr<Font> font;

	shared_ptr<BallCursur> cursur;
	shared_ptr<BallShooter> shooter;


	float wallHalfDepth;
	float wallHalfWidth;

	unsigned intersectCount;
	unsigned shooterMode;
	bool pressedEnter;
	float camcount;
	int bcount;
	unsigned int count;

	float radius;
	GLuint program;
};

