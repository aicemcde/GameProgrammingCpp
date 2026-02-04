#pragma once
#include "CameraComponent.h"

class FPSCamera : public CameraComponent
{
public:
	FPSCamera(class Actor* owner);
	~FPSCamera() = default;

	void Update(float deltaTime) override;

	void SetPitchSpeed(float value) { mPitchSpeed = value; }
	void SetMaxPitch(float value) { mMaxPitch = value; }
	float GetPitch() const { return mPitch; }
private:
	float mPitchSpeed = 0.0f;
	float mMaxPitch = 0.0f;
	float mPitch = 0.0f;
};