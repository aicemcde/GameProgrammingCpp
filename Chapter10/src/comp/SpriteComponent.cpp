#include "SpriteComponent.h"
#include <gl/glew.h>
#include "../util/Math.h"
#include "../actor/Actor.h"
#include "../util/Shader.h"
#include "../core/Game.h"
#include "../sys/Renderer.h"
#include "../util/Texture.h"
#include "../sys/GameSystem.h"

SpriteComponent::SpriteComponent(Actor* owner, int updateOrder)
	:ComponentBase(owner, updateOrder)
	, mTexHeight(0)
	, mTexWidth(0)
	, mTexture(nullptr)
{
	mOwner->GetRenderer()->AddSprite(this);
}

SpriteComponent::~SpriteComponent()
{
	
}

void SpriteComponent::Draw(Shader* shader)
{
	Matrix4 scaleMat = Matrix4::CreateScale(
		static_cast<float>(mTexWidth),
		static_cast<float>(mTexHeight),
		1.0f
	);
	Matrix4 world = scaleMat * mOwner->GetWorldTransform();
	shader->SetMatrixUniform("uWorldTransform", world);
	mTexture->SetActive();
	glDrawElements(
		GL_TRIANGLES,
		6,
		GL_UNSIGNED_INT,
		nullptr
	);
}

void SpriteComponent::SetTexture(Texture* texture)
{
	mTexture = texture;
	mTexWidth = mTexture->GetWidth();
	mTexHeight = mTexture->GetHeight();
}