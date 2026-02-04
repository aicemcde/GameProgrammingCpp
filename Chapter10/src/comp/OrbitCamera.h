#pragma once
#include "CameraComponent.h"



class OrbitCamera : public CameraComponent
{
public:
	OrbitCamera(Actor* owner);
	~OrbitCamera() = default;

	void Update(float deltaTime) override;

	void InitialPosition();

	void SetPitchSpeed(float value) { mPitchSpeed = value; }
	void SetYawSpeed(float value) { mYawSpeed = value; }
private:
	Vector3 mOffset;
	Vector3 mUp;
	float mPitchSpeed;
	float mYawSpeed;
};