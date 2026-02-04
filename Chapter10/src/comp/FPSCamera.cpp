#include "FPSCamera.h"
#include "../actor/Actor.h"

FPSCamera::FPSCamera(class Actor* owner)
	: CameraComponent(owner)
{

}

void FPSCamera::Update(float deltaTime)
{
	CameraComponent::Update(deltaTime);
	Vector3 cameraPos = mOwner->GetPosition();

	mPitch += mPitchSpeed * deltaTime;
	mPitch = Math::Clamp(mPitch, -mMaxPitch, mMaxPitch);
	Quaternion q(mOwner->GetRight(), mPitch);
	Vector3 viewForward = Vector3::Transform(mOwner->GetForward(), q);
	Vector3 targetPos = cameraPos + viewForward * 100.0f;
	Vector3 up = Vector3::Transform(Vector3::UnitZ, q);
	Matrix4 view = Matrix4::CreateLookAt(cameraPos, targetPos, up);
	SetViewMatrix(view, cameraPos, deltaTime);
}