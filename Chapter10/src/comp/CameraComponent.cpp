#include "CameraComponent.h"
#include "../actor/Actor.h"
#include "../sys/GameSystem.h"
#include "../sys/Renderer.h"
#include "../sys/AudioSystem.h"

CameraComponent::CameraComponent(Actor* owner)
	: Component(owner)
{

}

void CameraComponent::Update(float deltaTime)
{
}

void Update(float deltaTime)
{

}

void CameraComponent::SetViewMatrix(const Matrix4& view, const Vector3& playerPos, float deltaTime)
{
	mOwner->GetRenderer()->SetViewMatrix(view);
	mOwner->GetAudioSystem()->SetListener(view, playerPos, deltaTime);
}
