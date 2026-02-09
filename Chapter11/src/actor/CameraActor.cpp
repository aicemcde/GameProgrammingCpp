#include "CameraActor.h"
#include "../comp/MoveComponent.h"
#include "../core/Game.h"
#include "../sys/Renderer.h"
#include "../comp/AudioComponent.h"
#include "../sys/AudioSystem.h"
#include "../comp/MeshComponent.h"
#include "../sys/ResourceManager.h"
#include "../sys/GameSystem.h"
#include "../sys/InputSystem.h"

CameraActor::CameraActor(GameContext* context)
	:Actor(context)
	,mLastFootstep(0.0f)
	,mFootstepSurface(0.0f)
{
	mMoveComp = AddComponent_Pointer<MoveComponent>();
	mAudioComp = AddComponent_Pointer<AudioComponent>();
	MeshComponent* mc = AddComponent_Pointer<MeshComponent>();
	mc->SetMesh(mGameContext->resource->GetMesh("Assets/Sphere.gpmesh"));
	mFootstep = mAudioComp->PlayEvent("event:/Footstep", mGameContext);
	mFootstep.SetPaused(true);
}

void CameraActor::UpdateActor(float deltaTime)
{
	Actor::UpdateActor(deltaTime);

	mLastFootstep -= deltaTime;
	if (!Math::NearZero(mMoveComp->GetForwardSpeed()) && mLastFootstep <= 0.0f)
	{
		SoundEvent footstep = mAudioComp->PlayEvent("event:/Footstep", mGameContext);
		footstep.SetParameter("Surface", mFootstepSurface);
		mLastFootstep = 0.5f;
	}
	
	mCameraPos = GetPosition() - GetForward() * 200.0f + Vector3::UnitZ * 100.0f;
	Vector3 target = GetPosition() + GetForward() * 100.0f;
	Vector3 up = Vector3::UnitZ;

	Matrix4 view = Matrix4::CreateLookAt(mCameraPos, target, up);
	mGameContext->renderer->SetViewMatrix(view);
}

void CameraActor::ActorInput(const InputState& state)
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
}

void CameraActor::SetFootstepSurface(float value)
{
	mFootstepSurface = value;
}