#include "FPSActor.h"
#include "../sys/Renderer.h"
#include "../sys/Scene.h"
#include "../comp/MoveComponent.h"
#include "../comp/MeshComponent.h"
#include "../sys/ResourceManager.h"
#include "../sys/InputSystem.h"
#include "../util/Log.h"
#include "../sys/GameSystem.h"
#include "../comp/FPSCamera.h"
#include "../comp/SpriteComponent.h"
#include "../util/Collision.h"
#include "../comp/BoxComponent.h"
#include "../core/Game.h"
#include "PlaneActor.h"
#include "BallActor.h"
#include "../comp/AudioComponent.h"
#include "../comp/StateComponent.h"
#include "../util/state/Jump.h"
#include "../util/state/Falling.h"
#include "../util/state/Ground.h"
#include "../sys/PhysWorld.h"

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
	AddComponent<StateComponent>();

	mStateComp = GetComponent<StateComponent>();
	mStateComp->RegisterState<Jump>();
	mStateComp->RegisterState<Falling>();
	mStateComp->RegisterState<Ground>();
	mStateComp->ChangeState("ground");

	mFPSModel = context->scene->CreateActor<Actor>();
	mFPSModel->SetScale(2.0f);
	mCrosshair = mFPSModel->AddComponent_Pointer<SpriteComponent>();
	mCrosshair->SetTexture(context->resource->GetTexture("Assets/Crosshair.png"));

	mFPSModel = context->scene->CreateActor<Actor>();
	mFPSModel->SetScale(0.75f);
	mMeshForFPSModel = mFPSModel->AddComponent_Pointer<MeshComponent>();
	mMeshForFPSModel->SetMesh(mGameContext->resource->GetMesh("Assets/Rifle.gpmesh"));

	mCurrModelPosZ = GetPosition().z;
	LOG_INFO("Generate FPSActor");
}

void FPSActor::UpdateActor(float deltaTime)
{
	Actor::UpdateActor(deltaTime);

	FixCollisions(deltaTime);

	const Vector3 modelOffset(Vector3(10.0f, 10.0f, -10.0f));
	Vector3 modelPos = GetPosition();
	modelPos += GetForward() * modelOffset.x;
	modelPos += GetRight() * modelOffset.y;

	if (mCurrModelPosZ == 0) mCurrModelPosZ = modelPos.z + modelOffset.z;
	float targetPosZ = modelPos.z + modelOffset.z;
	mCurrModelPosZ = Math::Lerp(mCurrModelPosZ, targetPosZ, 35.0f * deltaTime);
	modelPos.z = mCurrModelPosZ;


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
		LOG_WARN("SDL relative mouse mode off");
		return;
	}
	
	float angularSpeed = 0.0f;
	if (state.Mouse.GetPosition().x != 0)
	{
		angularSpeed = state.Mouse.GetPosition().x / MAX_MOUSE_SPEED;
		angularSpeed *= MAX_ANGULAR_SPEED;
	}
	mMoveComp->SetAngularSpeed(angularSpeed);

	angularSpeed = 0.0f;
	if (state.Mouse.GetPosition().y != 0)
	{
		angularSpeed = state.Mouse.GetPosition().y / MAX_MOUSE_SPEED;
		angularSpeed *= MAX_ANGULAR_SPEED;
	}
	mFPSCamera->SetPitchSpeed(angularSpeed);
	
	if (mGameContext->input->GetMappedButtonState("Mouse_Left") == EPressed)
	{
		Shoot();
	}

	if (mGameContext->input->GetMappedButtonState("Mouse_Right") == EPressed)
	{
		mStateComp->ChangeState("jump");
	}
}

void FPSActor::FixCollisions(float deltaTime)
{
	ComputeWorldTransform(deltaTime);
	mBoxComp->OnUpdateWorldTransform(deltaTime);

	mGameContext->physWorld->TestSweepAndPrune([this, deltaTime](BoxComponent* a, BoxComponent* b)
		{
			Vector3 pos = this->GetPosition();
			const BoxComponent* playerBoxComp = nullptr;
			const BoxComponent* planeBoxComp = nullptr;

			if (this == a->GetOwner())
			{
				playerBoxComp = a;
				planeBoxComp = b;
			}
			else if (this == b->GetOwner())
			{
				playerBoxComp = b;
				planeBoxComp = a;
			}
			else
			{
				return;
			}

			const auto& playerBox = playerBoxComp->GetWorldBox();
			const auto& planeBox = planeBoxComp->GetWorldBox();

			float dx1 = planeBox.mMax.x - playerBox.mMin.x;
			float dx2 = planeBox.mMin.x - playerBox.mMax.x;
			float dy1 = planeBox.mMax.y - playerBox.mMin.y;
			float dy2 = planeBox.mMin.y - playerBox.mMax.y;
			float dz1 = planeBox.mMax.z - playerBox.mMin.z;
			float dz2 = planeBox.mMin.z - playerBox.mMax.z;

			float dx = (Math::Abs(dx1) < Math::Abs(dx2)) ? dx1 : dx2;
			float dy = (Math::Abs(dy1) < Math::Abs(dy2)) ? dy1 : dy2;
			float dz = (Math::Abs(dz1) < Math::Abs(dz2)) ? dz1 : dz2;

			float absDx = Math::Abs(dx);
			float absDy = Math::Abs(dy);
			float absDz = Math::Abs(dz);

			if (absDx <= absDy && absDx <= absDz)
			{
				pos.x += dx;
			}
			else if (absDy <= absDx && absDy <= absDz)
			{
				pos.y += dy;
			}
			else
			{
				pos.z += dz;
				if (dz > 0.0f && mStateComp->GetState()->IsA<Falling>())
				{
					mStateComp->ChangeState("ground");
				}
			}

			SetPosition(pos);
			ComputeWorldTransform(deltaTime);
			mBoxComp->OnUpdateWorldTransform(deltaTime);
		});

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