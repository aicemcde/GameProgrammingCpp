#include "FollowActor.h"
#include "GameSystem.h"
#include "MoveComponent.h"
#include "FollowCamera.h"
#include "OrbitCamera.h"
#include "MeshComponent.h"
#include "ResourceManager.h"
#include "InputSystem.h"
#include "Log.h"

FollowActor::FollowActor(GameContext* context)
	:Actor(context)
{
	SetScale(0.3f);
	SetPosition(Vector3(0.0f, 0.0f, -100.0f));
	mMoveComp = AddComponent_Pointer<MoveComponent>();
	mMeshComp = AddComponent_Pointer<MeshComponent>();
	mFollowCamera = AddComponent_Pointer<FollowCamera>();

	mMeshComp->SetMesh(context->resource->GetMesh("Assets/RacingCar.gpmesh"));
}

void FollowActor::UpdateActor(float deltaTime)
{
	Actor::UpdateActor(deltaTime);
}

void FollowActor::ActorInput(const InputState& state)
{
	float forwardSpeed = 0.0f;
	float angularSpeed = 0.0f;

	if (mGameContext->input->GetMappedButtonState("Mouse_Right") != EHeld)
	{
		mFollowCamera->SetIsFollowCamera(true);

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
	}

	mMoveComp->SetForwardSpeed(forwardSpeed);
	mMoveComp->SetAngularSpeed(angularSpeed);

	float yawSpeed = 0.0f;
	float pitchSpeed = 0.0f;
	if (mGameContext->input->GetMappedButtonState("Mouse_Right") == EHeld)
	{
		mFollowCamera->SetIsFollowCamera(false);

		if (SDL_GetRelativeMouseMode() != SDL_TRUE)
		{
			Log::Warn("SDL relative mouse mode off");
			return;
		}

		if (state.Mouse.GetPosition().x != 0)
		{
			yawSpeed = static_cast<float>(state.Mouse.GetPosition().x) / MAX_MOUSE_SPEED;
			yawSpeed*= MAX_ANGULAR_SPEED;
		}
		if (state.Mouse.GetPosition().y != 0)
		{
			pitchSpeed = static_cast<float>(state.Mouse.GetPosition().y) / MAX_MOUSE_SPEED;
			pitchSpeed *= MAX_ANGULAR_SPEED;
		}
	}
	mFollowCamera->SetYawSpeed(yawSpeed);
	mFollowCamera->SetPitchSpeed(pitchSpeed);
}