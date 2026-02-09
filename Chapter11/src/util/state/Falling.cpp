#include "Falling.h"
#include "../../comp/MoveComponent.h"
#include "../../actor/Actor.h"
#include "../../comp/StateComponent.h"
#include "../../core/Constants.h"

Falling::Falling(class StateComponent* owner)
	: StateBase(owner)
{

}

void Falling::OnEnter()
{
	mCachedMove = mOwner->GetOwner()->GetComponent<MoveComponent>();
}

void Falling::Update(float deltaTime)
{
	float fallSpeed = mCachedMove->GetUpSpeed();
	fallSpeed -= GRAVITY * deltaTime;
	mCachedMove->SetUpSpeed(fallSpeed);
}

void Falling::OnExit()
{
	mCachedMove->SetUpSpeed(0.0f);
}