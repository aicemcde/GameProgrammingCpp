#include "FPSActor.h"
#include "Renderer.h"
#include "Scene.h"
#include "MoveComponent.h"
#include "MeshComponent.h"
#include "ResourceManager.h"
#include "InputSystem.h"
#include "Log.h"
#include "GameSystem.h"
#include "FPSCamera.h"
#include "SpriteComponent.h"

FPSActor::FPSActor(GameContext* context)
	:Actor(context)
	, mAudioComp(nullptr)
{
	mMoveComp = AddComponent_Pointer<MoveComponent>();
	MeshComponent* mc = AddComponent_Pointer<MeshComponent>();
	mc->SetMesh(mGameContext->resource->GetMesh("Assets/Sphere.gpmesh"));
	mFPSCamera = AddComponent_Pointer<FPSCamera>();
	mFPSCamera->SetMaxPitch(Math::PiOver2 / 2.0f);

	mFPSModel = context->scene->CreateActor<Actor>();
	mFPSModel->SetScale(2.0f);
	mCrosshair = mFPSModel->AddComponent_Pointer<SpriteComponent>();
	mCrosshair->SetTexture(context->resource->GetTexture("Assets/Crosshair.png"));

	mFPSModel = context->scene->CreateActor<Actor>();
	mFPSModel->SetScale(0.75f);
	mMeshForFPSModel = mFPSModel->AddComponent_Pointer<MeshComponent>();
	mMeshForFPSModel->SetMesh(context->resource->GetMesh("Assets/Rifle.gpmesh"));
	Log::Info("Generate FPSActor");
}

void FPSActor::UpdateActor(float deltaTime)
{
	Actor::UpdateActor(deltaTime);

	const Vector3 modelOffset(Vector3(10.0f, 10.0f, -10.0f));
	Vector3 modelPos = GetPosition();
	modelPos += GetForward() * modelOffset.x;
	modelPos += GetRight() * modelOffset.y;
	modelPos.z += modelOffset.z;

	mFPSModel->SetPosition(modelPos);

	Quaternion q = GetRotation();

	q = Quaternion::Concatenate(q, Quaternion(GetRight(), mFPSCamera->GetPitch()));
	mFPSModel->SetRotation(q);

	mFPSModel->ComputeWorldTransform(deltaTime);
}

void FPSActor::ActorInput(const InputState& state)
{
	float forwardSpeed = 0.0f;
	float strafeSpeed = 0.0f;

	if (mGameContext->input->GetMappedButtonState("Move_Forward") == EHeld)
	{
		forwardSpeed += MAX_MOVE_SPEED;
	}
	if (mGameContext->input->GetMappedButtonState("Move_Back") == EHeld)
	{
		forwardSpeed -= MAX_MOVE_SPEED;
	}
	if (mGameContext->input->GetMappedButtonState("Move_Right") == EHeld)
	{
		strafeSpeed += MAX_MOVE_SPEED;
	}
	if (mGameContext->input->GetMappedButtonState("Move_Left") == EHeld)
	{
		strafeSpeed -= MAX_MOVE_SPEED;
	}

	mMoveComp->SetForwardSpeed(forwardSpeed);
	mMoveComp->SetStrafeSpeed(strafeSpeed);

	if (SDL_GetRelativeMouseMode() != SDL_TRUE)
	{
		Log::Warn("SDL relative mouse mode off");
		return;
	}
	
	float angularSpeed = 0.0f;
	if (state.Mouse.GetPosition().x != 0)
	{
		angularSpeed = static_cast<float>(state.Mouse.GetPosition().x) / MAX_MOUSE_SPEED;
		angularSpeed *= MAX_ANGULAR_SPEED;
	}
	mMoveComp->SetAngularSpeed(angularSpeed);

	angularSpeed = 0.0f;
	if (state.Mouse.GetPosition().y != 0)
	{
		angularSpeed = static_cast<float>(state.Mouse.GetPosition().y) / MAX_MOUSE_SPEED;
		angularSpeed *= MAX_ANGULAR_SPEED;
	}
	mFPSCamera->SetPitchSpeed(angularSpeed);
}