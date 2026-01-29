#include "CameraComponent.h"
#include "Actor.h"
#include "GameSystem.h"
#include "Renderer.h"
#include "AudioSystem.h"

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
