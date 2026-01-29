#pragma once
#include "CameraComponent.h"

class FollowCamera : public CameraComponent
{
public:
	FollowCamera(class Actor* owner);
	~FollowCamera() = default;

	void Update(float deltaTime) override;
	Vector3 ComputeCameraPos() const;

	void SnapToIdeal();
	void OrbitProcess(float delatTime);
	void FollowProcess(float deltaTime);

	void SetPitchSpeed(float speed) { mPitchSpeed = speed; }
	void SetYawSpeed(float speed) { mYawSpeed = speed; }
	void SetIsFollowCamera(bool value);
private:
	float mHorizonDist;
	float mVertDist;
	float mTargetDist;

	Vector3 mActualPos;
	Vector3 mVelocity;
	float mSpringConstant;

	Vector3 mOffset;
	Vector3 mUp;
	float mPitchSpeed;
	float mYawSpeed;
	bool mIsFollowCamera;
};