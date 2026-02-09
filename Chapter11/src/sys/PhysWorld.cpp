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

void PhysWorld::TestSweepAndPrune(std::function<void(BoxComponent*, BoxComponent*)> f)
{
	std::ranges::sort(mBoxes, [](BoxComponent* a, BoxComponent* b)
		{
			return a->GetWorldBox().mMin.x < b->GetWorldBox().mMin.x;
		});

	for (size_t i = 0; i < mBoxes.size(); ++i)
	{
		BoxComponent* a = mBoxes[i];
		const auto& aBox = a->GetWorldBox();
		for (size_t j = i + 1; j < mBoxes.size(); j++)
		{
			BoxComponent* b = mBoxes[j];
			const auto& bBox = b->GetWorldBox();
			if (bBox.mMin.x > aBox.mMax.x)
			{
				break;
			}

			if (bBox.mMin.y > aBox.mMax.y)
			{
				continue;
			}

			if (bBox.mMin.z > aBox.mMax.z)
			{
				continue;
			}

			if (Collision::Intersect(a->GetWorldBox(), b->GetWorldBox()))
			{
				f(a, b);
			}
		}
	}
}