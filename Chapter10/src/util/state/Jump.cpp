#include "Jump.h"
#include "../../comp/StateComponent.h"
#include "../../actor/Actor.h"
#include "../../util/Log.h"
#include "../../comp/MoveComponent.h" 
#include "../../core/Constants.h"
#include "../../sys/InputSystem.h"

Jump::Jump(class StateComponent* owner)
	:StateBase(owner)
{

}

void Jump::OnEnter()
{
	mCachedMove = mOwner->GetOwner()->GetComponent<MoveComponent>();
	if (!mCachedMove)
	{
		Log::Warn("Could not find moveComp from Actor");
	}
	mElapsedTime = 0.0f;
	mIsJumping = true;
	mCachedMove->SetUpSpeed(JUMP_SPEED);
}

void Jump::Update(float deltaTime)
{
	float upSpeed = mCachedMove->GetUpSpeed();
	if (upSpeed <= 0)
	{
		OnExit();
		mOwner->ChangeState("falling");
		return;
	}
	if (!mIsJumping)
	{
		upSpeed -= GRAVITY * deltaTime;
	}
	mCachedMove->SetUpSpeed(upSpeed);
	mElapsedTime += deltaTime;
	mDeltaTime = deltaTime;
}

void Jump::OnExit()
{
	
}

void Jump::ProcessInput(const InputState& state)
{
	float upSpeed = mCachedMove->GetUpSpeed();
	if (mOwner->GetOwner()->GetInputSystem()->GetMappedButtonState("Mouse_Right") == EHeld&& mElapsedTime <= 0.1f)
	{
		upSpeed += JUMP_SPEED * mDeltaTime;
	}
	else if (mOwner->GetOwner()->GetInputSystem()->GetMappedButtonState("Mouse_Right") == EHeld && mElapsedTime <= 0.5f)
	{
		upSpeed += JUMP_SPEED * 0.5f * mDeltaTime;
	}
	else
	{
		mIsJumping = false;
	}

	mCachedMove->SetUpSpeed(upSpeed);
}