#pragma once
#include <string>
#include <vector>
#include <memory>
#include "Collision.h"

struct GameContext;

class Mesh
{
public:
	Mesh();
	~Mesh();

	bool Load(const std::string& fileName, class Renderer* renderer, GameContext* context);
	void Unload();

	class VertexArray* GetVertexArray() { return mVertexArray.get(); }
	class Texture* GetTexture(size_t index);
	const std::string& GetShaderName() const { return mShaderName; }
	float GetRadius() const { return mRadius; }
	float GetSpecPower() const noexcept { return mSpecPower; }
	const AABB& GetBox() const { return mObjectBox; }
private:
	std::vector<class Texture*> mTextures;
	std::unique_ptr<class VertexArray> mVertexArray;
	std::string mShaderName;
	float mRadius;
	float mSpecPower;
	AABB mObjectBox;
};