#pragma once
//#include "StaticObject.h"
//#include "RagidObject.h"
//#include "SoundPlayer.h"
//#include "Textureh.h"
#include <map>
#include <memory>
#include<string>
#include "GL\glew.h"
#include "Uniform.h"
class StaticObject;
class RagidObject;
class Shape;
class Texture;
class Font;
class Shader;
class Material;
class CTMaterial;

namespace Sound
{
	class SoundPlayer;
}

using namespace std;
namespace PongGame
{
	class GameResourceManager
	{


	public:
		GameResourceManager();
		~GameResourceManager();



		void add(const std::string&,std::shared_ptr<Texture> texture);
		void add(const std::string&, std::shared_ptr<Sound::SoundPlayer> sound);
		void add(const std::string&, std::shared_ptr<Shape> shepe);
		void add(const std::string&, std::shared_ptr<Font> font);
		void add(const std::string&, std::shared_ptr<Shader> shader);
		void add(const std::string&, std::shared_ptr<Uniform<Material>> material);
		void add(const std::string&, std::shared_ptr<Uniform<CTMaterial>> ctMaterial);


		std::shared_ptr<Texture> getTexture(const std::string&)const;
		std::shared_ptr<Sound::SoundPlayer> getSound(const std::string&)const;
		std::shared_ptr<Shape> getShape(const std::string&)const;
		std::shared_ptr<Font> getFont(const std::string&)const;
		std::shared_ptr<Shader>   getShader(const std::string&)const;
		std::shared_ptr<Uniform<Material>> getMaterial(const std::string&)const;
		std::shared_ptr<Uniform<CTMaterial>> getCTMaterial(const std::string&)const;


	private:
	


		std::map<std::string, std::shared_ptr<Texture>> textures;
		std::map<std::string, std::shared_ptr<Sound::SoundPlayer>> sounds;
		std::map <std::string, std::shared_ptr<Shape>>  shapes;
		std::map<std::string, std::shared_ptr<Font>> fonts;
		std::map<std::string, std::shared_ptr<Shader>> shaders;
		std::map < std::string, std::shared_ptr<Uniform<Material>>> materials;
		std::map < std::string, std::shared_ptr<Uniform<CTMaterial>>> ctMaterials;

	};
}


