#include "SplineCamera.h"

SplineCamera::SplineCamera(Actor* owner)
	: CameraComponent(owner)
	, mPaused(true)
	, mIndex(0)
	, mT(0.0f)
	, mSpeed(0.5f)
{

}

void SplineCamera::Update(float deltaTime)
{
	CameraComponent::Update(deltaTime);

	if (!mPaused)
	{
		mT += mSpeed * deltaTime;

		if (mT >= 1.0f)
		{
			if (mIndex < mPath.GetNumPoints() - 3)
			{
				mIndex++;
				mT = mT - 1.0f;
			}
			else
			{
				mPaused = true;
			}
		}
	}

	Vector3 cameraPos = mPath.Compute(mIndex, mT);
	Vector3 target = mPath.Compute(mIndex, mT + 0.01f);
	const Vector3 up = Vector3::UnitZ;
	Matrix4 view = Matrix4::CreateLookAt(cameraPos, target, up);
	SetViewMatrix(view, target, deltaTime);
}

void SplineCamera::Restart()
{
	mIndex = 0;
	mT = 0.0f;
	mPaused = false;
}