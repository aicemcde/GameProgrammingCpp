#pragma once
#include "Component.h"

struct AABB;

class MeshComponent : public Component
{
public:
	MeshComponent(class Actor* owner);
	~MeshComponent() final;

	virtual void Draw(class Shader* shader);
	virtual void SetMesh(class Mesh* mesh) { mMesh = mesh; };
	void SetTextureIndex(size_t index) { mTextureIndex = index; };
	const AABB& GetBox();
protected:
	class Mesh* mMesh;
	size_t mTextureIndex;
};