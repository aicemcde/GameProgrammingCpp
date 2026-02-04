#include "FollowCamera.h"
#include "../actor/Actor.h"
#include "../sys/Renderer.h"
FollowCamera::FollowCamera(Actor* owner)
	:CameraComponent(owner)
	,mHorizonDist(300.0f)
	,mVertDist(100.0f)
	,mTargetDist(100.0f)
	,mSpringConstant(32.0f)
	,mPitchSpeed(0.0f)
	,mYawSpeed(0.0f)
	,mUp(Vector3::UnitZ)
	,mIsFollowCamera(true)
{
	
}

void FollowCamera::Update(float deltaTime)
{
	CameraComponent::Update(deltaTime);

	if (mIsFollowCamera)
	{
		FollowProcess(deltaTime);
	}
	else
	{
		OrbitProcess(deltaTime);
	}
}

Vector3 FollowCamera::ComputeCameraPos() const
{
	Vector3 cameraPos = mOwner->GetPosition();
	cameraPos -= mOwner->GetForward() * mHorizonDist;
	cameraPos += Vector3::UnitZ * mVertDist;
	return cameraPos;
}

void FollowCamera::SnapToIdeal()
{
	mActualPos = ComputeCameraPos();
	mVelocity = Vector3::Zero;
	Vector3 target = mOwner->GetPosition() + mOwner->GetForward() * mTargetDist;
	Matrix4 view = Matrix4::CreateLookAt(mActualPos, target, Vector3::UnitZ);
	SetViewMatrix(view, mActualPos, 0);
}

void FollowCamera::OrbitProcess(float deltaTime)
{
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
	Vector3 cameraPos = target + mOffset;
	Matrix4 view = Matrix4::CreateLookAt(cameraPos, target, mUp);
	SetViewMatrix(view, target, deltaTime);
}

void FollowCamera::FollowProcess(float deltaTime)
{
	float dampening = 2.0f * Math::Sqrt(mSpringConstant);
	Vector3 idealPos = ComputeCameraPos();
	Vector3 diff = mActualPos - idealPos;
	Vector3 acel = -mSpringConstant * diff - dampening * mVelocity;
	mVelocity += acel * deltaTime;
	mActualPos += mVelocity * deltaTime;

	Vector3 target = mOwner->GetPosition() + mOwner->GetForward() * mTargetDist;
	Matrix4 view = Matrix4::CreateLookAt(mActualPos, target, Vector3::UnitZ);
	SetViewMatrix(view, mActualPos, deltaTime);
}

void FollowCamera::SetIsFollowCamera(bool value)
{
	if (mIsFollowCamera == value)
	{
		return;
	}

	Vector3 target = mOwner->GetPosition();

	if (value == true)
	{
		Vector3 cameraPos = target + mOffset;
		mActualPos = cameraPos;
		mIsFollowCamera = value;
	}
	else
	{
		mOffset = mActualPos - target;
		mUp = Vector3::UnitZ;
		mIsFollowCamera = value;
	}
}