#pragma once
#include <memory>
#include <unordered_map>
#include <string>

class Texture;
struct GameContext;

class ResourceManager
{
public:
	explicit ResourceManager(GameContext* context);
	~ResourceManager();
	Texture* GetTexture(const std::string& fileName);
	class Mesh* GetMesh(const std::string& fileName);
	void Unload();
private:
	GameContext* mGameContext;
	std::unordered_map<std::string, std::unique_ptr<Texture>> mTextures;
	std::unordered_map<std::string, std::unique_ptr<Mesh>> mMeshes;
};