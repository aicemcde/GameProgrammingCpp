#include "PhysWorld.h"
#include <algorithm>
#include "../comp/BoxComponent.h"

PhysWorld::PhysWorld(GameContext* context)
	:mContext(context)
{

}

PhysWorld::~PhysWorld()
{
	
}

void PhysWorld::AddBox(BoxComponent* box)
{
	mBoxes.emplace_back(box);
}

void PhysWorld::RemoveBox(BoxComponent* box)
{
	auto iter = std::remove(mBoxes.begin(), mBoxes.end(), box);
	mBoxes.erase(iter, mBoxes.end());
}

bool PhysWorld::SegmentCast(const LineSegment& l, Physics::CollisionInfo& outInfo)
{
	bool collided = false;
	float closestT = Math::Infinity;
	Vector3 norm;

	for (auto box : mBoxes)
	{
		float t;
		if (Collision::Intersect(l, box->GetWorldBox(), t, norm))
		{
			if (t < closestT)
			{
				outInfo.mPoint = l.PointOnSegment(t);
				outInfo.mNormal = norm;
				outInfo.mBox = box;
				outInfo.mOwner = box->GetOwner();
				collided = true;
			}
		}
	}
	return collided;
}

void PhysWorld::TestSweepAndPrune(std::function<void(Actor*, Actor*)> f)
{
	std::ranges::sort(mBoxes, [](BoxComponent* a, BoxComponent* b)
		{
			return a->GetWorldBox().mMin.x < b->GetWorldBox().mMin.x;
		});

	for (size_t i = 0; i < mBoxes.size(); ++i)
	{
		BoxComponent* a = mBoxes[i];
		float max = a->GetWorldBox().mMax.x;
		for (size_t j = i + 1; j < mBoxes.size(); j++)
		{
			BoxComponent* b = mBoxes[j];
			if (b->GetWorldBox().mMin.x < max)
			{
				break;
			}
			else if (Collision::Intersect(a->GetWorldBox(), b->GetWorldBox()))
			{
				f(a->GetOwner(), b->GetOwner());
			}
		}
	}
}