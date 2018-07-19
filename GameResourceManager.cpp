#include "GameResourceManager.h"
#include "StaticObject.h"

using namespace PongGame;
GameResourceManager::GameResourceManager()
{
}


GameResourceManager::~GameResourceManager()
{
	
}

void PongGame::GameResourceManager::add(const std::string & name, std::shared_ptr<Texture> texture)
{
	textures.insert(std::pair<std::string, std::shared_ptr<Texture >>(name,texture));
}

void PongGame::GameResourceManager::add(const std::string & name, std::shared_ptr<Sound::SoundPlayer> sound)
{
	sounds.insert(std::pair < std::string, std::shared_ptr < Sound::SoundPlayer >> (name, sound));

}

void PongGame::GameResourceManager::add(const std::string &name, std::shared_ptr<Shape> shape)
{
	shapes.insert(std::pair<std::string, std::shared_ptr<Shape >>(name,shape));
}

void PongGame::GameResourceManager::add(const std::string &name, std::shared_ptr<Font> font)
{
	fonts.insert(std::pair<std::string, std::shared_ptr<Font >>(name, font));

}

void PongGame::GameResourceManager::add(const std::string &name, std::shared_ptr<Shader> shader)
{
	shaders.insert(std::pair<std::string, std::shared_ptr<Shader >>(name, shader));
}

void  PongGame::GameResourceManager::add(const std::string&name, std::shared_ptr<Uniform<Material>> material)
{
	materials.insert(std::pair<std::string, std::shared_ptr<Uniform<Material>>>(name, material));
}

void  PongGame::GameResourceManager::add(const std::string& name, std::shared_ptr<Uniform<CTMaterial>> ctMaterial)
{
	ctMaterials.insert(std::pair<std::string, std::shared_ptr<Uniform<CTMaterial>>>(name,ctMaterial));
}


std::shared_ptr<Texture> PongGame::GameResourceManager::getTexture(const std::string &name) const
{
	return textures.at(name);
}

std::shared_ptr<Sound::SoundPlayer> PongGame::GameResourceManager::getSound(const std::string &name) const
{
	return sounds.at(name);
}

std::shared_ptr<Shape> PongGame::GameResourceManager::getShape(const std::string &name) const
{
	return shapes.at(name);
}

std::shared_ptr<Font> PongGame::GameResourceManager::getFont(const std::string &name) const
{
	return  fonts.at(name);
}

std::shared_ptr<Shader> PongGame::GameResourceManager::getShader(const std::string &name) const
{
	return shaders.at(name);
}

std::shared_ptr<Uniform<Material>> PongGame::GameResourceManager::getMaterial(const std::string &name) const
{
	return materials.at(name);
}

std::shared_ptr<Uniform<CTMaterial>> PongGame::GameResourceManager::getCTMaterial(const std::string &name) const
{
	return ctMaterials.at(name);
}
