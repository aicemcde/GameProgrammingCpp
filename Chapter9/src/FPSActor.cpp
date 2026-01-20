#include "FPSActor.h"
#include "Game.h"
#include "Renderer.h"
#include "Scene.h"
#include "MoveComponent.h"
#include "MeshComponent.h"
#include "ResourceManager.h"
#include "InputSystem.h"

FPSActor::FPSActor(Game* game)
	:Actor(game)
{
	mMoveComp = AddComponent_Pointer<MoveComponent>(this);
	MeshComponent* mc = AddComponent_Pointer<MeshComponent>(this);
	mc->SetMesh(mGame->GetResourceManager()->GetMesh("Assets/Sphere.gpmesh"));
}

void FPSActor::UpdateActor(float deltaTime)
{
	Actor::UpdateActor(deltaTime);

	Vector3 cameraPos = GetPosition();
	Vector3 target = cameraPos + GetForward() * 100.0f;
	Vector3 up = Vector3::UnitZ;

	Matrix4 view = Matrix4::CreateLookAt(cameraPos, target, up);
	Game::GetRendererInstance()->SetViewMatrix(view);
}

void FPSActor::ActorInput(const InputState& state)
{
	float forwardSpeed = 0.0f;
	float strafeSpeed = 0.0f;

	if (mGame->GetInputSystemInstance()->GetMappedButtonState("Move_Forward") == EHeld)
	{
		forwardSpeed += MAX_MOVE_SPEED;
	}
	if (mGame->GetInputSystemInstance()->GetMappedButtonState("Move_Back") == EHeld)
	{
		forwardSpeed -= MAX_MOVE_SPEED;
	}
	if (mGame->GetInputSystemInstance()->GetMappedButtonState("Move_Right") == EHeld)
	{
		strafeSpeed += MAX_MOVE_SPEED;
	}
	if (mGame->GetInputSystemInstance()->GetMappedButtonState("Move_Left") == EHeld)
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
}