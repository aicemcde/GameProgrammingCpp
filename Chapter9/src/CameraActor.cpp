#include "CameraActor.h"
#include "MoveComponent.h"
#include "Game.h"
#include "Renderer.h"
#include "AudioComponent.h"
#include "AudioSystem.h"
#include "MeshComponent.h"
#include "ResourceManager.h"

CameraActor::CameraActor(Game* game)
	:Actor(game)
	,mLastFootstep(0.0f)
	,mFootstepSurface(0.0f)
{
	mMoveComp = AddComponent_Pointer<MoveComponent>(this);
	mAudioComp = AddComponent_Pointer<AudioComponent>(this);
	MeshComponent* mc = AddComponent_Pointer<MeshComponent>(this);
	mc->SetMesh(mGame->GetResourceManager()->GetMesh("Assets/Sphere.gpmesh"));
	mFootstep = mAudioComp->PlayEvent("event:/Footstep");
	mFootstep.SetPaused(true);
}

void CameraActor::UpdateActor(float deltaTime)
{
	Actor::UpdateActor(deltaTime);

	mLastFootstep -= deltaTime;
	if (!Math::NearZero(mMoveComp->GetForwardSpeed()) && mLastFootstep <= 0.0f)
	{
		SoundEvent footstep = mAudioComp->PlayEvent("event:/Footstep");
		footstep.SetParameter("Surface", mFootstepSurface);
		mLastFootstep = 0.5f;
	}
	
	mCameraPos = GetPosition() - GetForward() * 200.0f + Vector3::UnitZ * 100.0f;
	Vector3 target = GetPosition() + GetForward() * 100.0f;
	Vector3 up = Vector3::UnitZ;

	Matrix4 view = Matrix4::CreateLookAt(mCameraPos, target, up);
	Game::GetRendererInstance()->SetViewMatrix(view);
}

void CameraActor::ActorInput(const InputState& state)
{
	float forwardSpeed = 0.0f;
	float angularSpeed = 0.0f;
	if (mGame->GetInputSystemInstance()->GetMappedButtonState("Move_Forward") == EHeld)
	{
		forwardSpeed += 300.0f;
	}
	if (mGame->GetInputSystemInstance()->GetMappedButtonState("Move_Back") == EHeld)
	{
		forwardSpeed -= 300.0f;
	}
	if (mGame->GetInputSystemInstance()->GetMappedButtonState("Perspective_Left") == EHeld)
	{
		angularSpeed -= Math::TwoPi;
	}
	if (mGame->GetInputSystemInstance()->GetMappedButtonState("Perspective_Right") == EHeld)
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