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
#include "Collision.h"
#include "BoxComponent.h"
#include "Game.h"
#include "PlaneActor.h"
#include "BallActor.h"
#include "AudioComponent.h"

FPSActor::FPSActor(GameContext* context)
	:Actor(context)
	, mAudioComp(nullptr)
{
	mMoveComp = AddComponent_Pointer<MoveComponent>();
	mFPSCamera = AddComponent_Pointer<FPSCamera>();
	mFPSCamera->SetMaxPitch(Math::PiOver2 / 2.0f);
	mBoxComp = AddComponent_Pointer<BoxComponent>();
	AABB myBox(Vector3(-25.0f, -25.0f, -87.5f), Vector3(25.0f, 25.0f, 87.5f));
	mBoxComp->SetObjectBox(myBox);
	mAudioComp = AddComponent_Pointer<AudioComponent>();

	mFPSModel = context->scene->CreateActor<Actor>();
	mFPSModel->SetScale(2.0f);
	mCrosshair = mFPSModel->AddComponent_Pointer<SpriteComponent>();
	mCrosshair->SetTexture(context->resource->GetTexture("Assets/Crosshair.png"));

	mFPSModel = context->scene->CreateActor<Actor>();
	mFPSModel->SetScale(0.75f);
	mMeshForFPSModel = mFPSModel->AddComponent_Pointer<MeshComponent>();
	mMeshForFPSModel->SetMesh(mGameContext->resource->GetMesh("Assets/Rifle.gpmesh"));
	Log::Info("Generate FPSActor");
}

void FPSActor::UpdateActor(float deltaTime)
{
	Actor::UpdateActor(deltaTime);

	FixCollisions(deltaTime);

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
	
	if (mGameContext->input->GetMappedButtonState("Mouse_Left") == EPressed)
	{
		Shoot();
	}

}

void FPSActor::FixCollisions(float deltaTime)
{
	ComputeWorldTransform(deltaTime);

	const AABB& playerBox = mBoxComp->GetWorldBox();
	Vector3 pos = GetPosition();

	auto& planes = mGameContext->game->GetPlanes();
	for (auto pa : planes)
	{
		const AABB& planeBox = pa->GetBoxComp()->GetWorldBox();
		if (Collision::Intersect(playerBox, planeBox))
		{
			float dx1 = planeBox.mMax.x - playerBox.mMin.x;
			float dx2 = planeBox.mMin.x - playerBox.mMax.x;
			float dy1 = planeBox.mMax.y - playerBox.mMin.y;
			float dy2 = planeBox.mMin.y - playerBox.mMax.y;
			float dz1 = planeBox.mMax.z - playerBox.mMin.z;
			float dz2 = planeBox.mMin.z - playerBox.mMax.z;

			float dx = (Math::Abs(dx1) < Math::Abs(dx2)) ? dx1 : dx2;
			float dy = (Math::Abs(dy1) < Math::Abs(dy2)) ? dy1 : dy2;
			float dz = (Math::Abs(dz1) < Math::Abs(dz2)) ? dz1 : dz2;

			if (Math::Abs(dx) <= Math::Abs(dy) && Math::Abs(dx) <= Math::Abs(dz))
			{
				pos.x += dx;
			}
			else if (Math::Abs(dy) <= Math::Abs(dx) && Math::Abs(dy) <= Math::Abs(dz))
			{
				pos.y += dy;
			}
			else
			{
				pos.z += dz;
			}

			SetPosition(pos);
			ComputeWorldTransform(deltaTime);
			mBoxComp->OnUpdateWorldTransform(deltaTime);
		}
	}
}

void FPSActor::Shoot()
{
	Vector3 screenPoint(0.0f, 0.0f, 0.0f);
	Vector3 start = mGameContext->renderer->UnProject(screenPoint);
	screenPoint.z = 0.9f;
	Vector3 end = mGameContext->renderer->UnProject(screenPoint);
	Vector3 dir = end - start;
	dir.Normalize();

	BallActor* ball = mGameContext->scene->CreateActor<BallActor>();
	ball->SetPlayer(this);
	ball->SetPosition(start + dir * 20.0f);
	ball->RotateToNewForward(dir);
	mAudioComp->PlayEvent("event:/Shot", mGameContext);
}