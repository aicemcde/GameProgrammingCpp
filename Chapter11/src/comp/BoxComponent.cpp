#include "BoxComponent.h"
#include "../actor/Actor.h"
#include "../sys/PhysWorld.h"

BoxComponent::BoxComponent(Actor* owner)
	:ComponentBase(owner)
	,mShouldRotate(true)
{
	mOwner->GetPhysWorld()->AddBox(this);
}

BoxComponent::~BoxComponent()
{
	mOwner->GetPhysWorld()->RemoveBox(this);
}

void BoxComponent::OnUpdateWorldTransform(float deltaTime)
{
	mWorldBox = mObjectBox;
	mWorldBox.mMin *= mOwner->GetScale();
	mWorldBox.mMax *= mOwner->GetScale();

	if (mShouldRotate)
	{
		mWorldBox.Rotate(mOwner->GetRotation());
	}

	mWorldBox.mMin += mOwner->GetPosition();
	mWorldBox.mMax += mOwner->GetPosition();
}