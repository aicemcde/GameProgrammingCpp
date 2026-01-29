#pragma once
#include "CameraComponent.h"
#include "StructData.h"

class SplineCamera : public CameraComponent
{
public:
	SplineCamera(class Actor* owner);
	~SplineCamera() = default;

	void Update(float deltaTime) override;
	void SetSpline(const Spline& path) { mPath = path; }
	void SetPaused(bool pause) { mPaused = pause; }
	void Restart();
private:
	Spline mPath;
	size_t mIndex;
	float mT;
	float mSpeed;
	bool mPaused;
};