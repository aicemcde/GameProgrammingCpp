#include "BallMove.h"
#include "Actor.h"
#include "Collision.h"
#include "PhysWorld.h"

BallMove::BallMove(Actor* owner)
	:MoveComponent(owner)
{

}

void BallMove::Update(float deltaTime)
{
	const float segmentLength = 30.0f;
	Vector3 start = mOwner->GetPosition();
	Vector3 dir = mOwner->GetForward();
	Vector3 end = start + dir * segmentLength;
	LineSegment ls(start, end);

	PhysWorld* phys = mOwner->GetPhysWorld();
	if (Physics::CollisionInfo info; phys->SegmentCast(ls, info) && info.mOwner != mPlayer)
	{
		dir = Vector3::Reflect(dir, info.mNormal);
		mOwner->RotateToNewForward(dir);
	}

	MoveComponent::Update(deltaTime);
}