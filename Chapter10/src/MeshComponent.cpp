#include "MeshComponent.h"
#include "Shader.h"
#include "Actor.h"
#include "Texture.h"
#include "Mesh.h"
#include "VertexArray.h"
#include <glew.h>
#include "Renderer.h"
#include "GameSystem.h"
#include "Log.h"
#include "Collision.h"

MeshComponent::MeshComponent(Actor* owner)
	:Component(owner)
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
			Log::Error("MeshComponent : Texture does not get");
		}
		VertexArray* va = mMesh->GetVertexArray();
		SDL_assert(va != nullptr);
		va->SetActive();
		glDrawElements(GL_TRIANGLES, va->GetNumIndices(),
			GL_UNSIGNED_INT, nullptr);
	}
	else
	{
		Log::Error("Mesh could not draw");
	}
}

const AABB& MeshComponent::GetBox()
{
	return mMesh->GetBox();
}