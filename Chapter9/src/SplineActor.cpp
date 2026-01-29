#include "SplineActor.h"
#include "SplineCamera.h"
#include "MoveComponent.h"
#include "MeshComponent.h"
#include "GameSystem.h"
#include "ResourceManager.h"
#include "InputSystem.h"

SplineActor::SplineActor(GameContext* context)
	: Actor(context)
{
	SetScale(0.3f);
	SetPosition(Vector3(0.0f, 0.0f, -100.0f));

	mCameraComp = AddComponent_Pointer<SplineCamera>();
	mMoveComp = AddComponent_Pointer<MoveComponent>();
	mMeshComp = AddComponent_Pointer<MeshComponent>();

	Spline path;
	path.mControlPoints.emplace_back(Vector3::Zero);
	for (int i = 0; i < 5; i++)
	{
		if (i % 2 == 0)
		{
			path.mControlPoints.emplace_back(Vector3(300.0f * (i + 1), 300.0f, 300.0f));
		}
		else
		{
			path.mControlPoints.emplace_back(Vector3(300.0f * (i + 1), 0.0f, 0.0f));
		}
	}

	mCameraComp->SetSpline(path);
	mCameraComp->SetPaused(false);

	mMeshComp->SetMesh(context->resource->GetMesh("Assets/RacingCar.gpmesh"));
}

void SplineActor::UpdateActor(float deltaTime)
{
	Actor::UpdateActor(deltaTime);
}

void SplineActor::ActorInput(const InputState& state)
{
	float forwardSpeed = 0.0f;
	float angularSpeed = 0.0f;
	if (mGameContext->input->GetMappedButtonState("Move_Forward") == EHeld)
	{
		forwardSpeed += 300.0f;
	}
	if (mGameContext->input->GetMappedButtonState("Move_Back") == EHeld)
	{
		forwardSpeed -= 300.0f;
	}
	if (mGameContext->input->GetMappedButtonState("Perspective_Left") == EHeld)
	{
		angularSpeed -= Math::TwoPi;
	}
	if (mGameContext->input->GetMappedButtonState("Perspective_Right") == EHeld)
	{
		angularSpeed += Math::TwoPi;
	}

	mMoveComp->SetForwardSpeed(forwardSpeed);
	mMoveComp->SetAngularSpeed(angularSpeed);

	if (mGameContext->input->GetMappedButtonState("Restart") == EPressed)
	{
		mCameraComp->Restart();
	}
}