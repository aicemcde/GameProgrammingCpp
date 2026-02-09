#include "OrbitCamera.h"
#include "../actor/Actor.h"

OrbitCamera::OrbitCamera(Actor* owner)
	: CameraComponent(owner)
	,mPitchSpeed(0.0f)
	,mYawSpeed(0.0f)
	,mUp(Vector3::UnitZ)
{
	InitialPosition();
}

void OrbitCamera::Update(float deltaTime)
{
	CameraComponent::Update(deltaTime);

	Quaternion yaw(Vector3::UnitZ, mYawSpeed * deltaTime);
	mOffset = Vector3::Transform(mOffset, yaw);
	mUp = Vector3::Transform(mUp, yaw);

	Vector3 forward = -1.0f * mOffset;
	forward.Normalize();
	Vector3 right = Vector3::Cross(mUp, forward);
	right.Normalize();

	Quaternion pitch(right, mPitchSpeed * deltaTime);
	mOffset = Vector3::Transform(mOffset, pitch);
	mUp = Vector3::Transform(mUp, pitch);

	Vector3 target = mOwner->GetPosition();
	Vector3 CameraPos = target + mOffset;
	Matrix4 view = Matrix4::CreateLookAt(CameraPos, target, mUp);
	SetViewMatrix(view, target, deltaTime);
}

void OrbitCamera::InitialPosition()
{
	mOffset = Vector3(-300.0f, 0.0f, 100.0f);
	Vector3 target = mOwner->GetPosition();
	Vector3 cameraPos = target + mOffset;
	Matrix4 view = Matrix4::CreateLookAt(cameraPos, target, mUp);
	SetViewMatrix(view, target, 0);
}