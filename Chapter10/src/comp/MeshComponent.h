#pragma once
#include "Component.h"

struct AABB;

class MeshComponent : public ComponentBase<MeshComponent>
{
public:
	explicit MeshComponent(class Actor* owner);
	~MeshComponent() override;

	virtual void Draw(class Shader* shader);
	virtual void SetMesh(class Mesh* mesh) { mMesh = mesh; };
	void SetTextureIndex(size_t index) { mTextureIndex = index; };
	const AABB& GetBox();
protected:
	class Mesh* mMesh;
	size_t mTextureIndex;
};