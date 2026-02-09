#include "MeshComponent.h"
#include "../util/Shader.h"
#include "../actor/Actor.h"
#include "../util/Texture.h"
#include "../util/Mesh.h"
#include "../util/VertexArray.h"
#include <gl/glew.h>
#include "../sys/Renderer.h"
#include "../sys/GameSystem.h"
#include "../util/Log.h"
#include "../util/Collision.h"

MeshComponent::MeshComponent(Actor* owner)
	:ComponentBase(owner)
	, mMesh(nullptr)
	, mTextureIndex(0)
{
	mOwner->GetRenderer()->AddMeshComp(this);
}

MeshComponent::~MeshComponent()
{
	mOwner->GetRenderer()->RemoveMeshComp(this);
}

void MeshComponent::Draw(Shader* shader)
{
	if (mMesh)
	{
		shader->SetMatrixUniform("uWorldTransform",
			mOwner->GetWorldTransform());
		shader->SetFloatUniform("uSpecPower", mMesh->GetSpecPower());
		Texture* t = mMesh->GetTexture(mTextureIndex);
		if (t)
		{ 
			t->SetActive();
		}
		else
		{
			LOG_ERROR("MeshComponent : Texture does not get");
		}
		VertexArray* va = mMesh->GetVertexArray();
		SDL_assert(va != nullptr);
		va->SetActive();
		glDrawElements(GL_TRIANGLES, va->GetNumIndices(),
			GL_UNSIGNED_INT, nullptr);
	}
	else
	{
		LOG_ERROR("Mesh could not draw");
	}
}

const AABB& MeshComponent::GetBox()
{
	return mMesh->GetBox();
}