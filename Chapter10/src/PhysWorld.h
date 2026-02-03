#pragma once
#include <vector>
#include "PhysicsTypes.h"
#include "Collision.h"
#include <functional>

struct GameContext;

class PhysWorld
{
public:
	PhysWorld(GameContext* context);
	~PhysWorld();

	void AddBox(class BoxComponent* box);
	void RemoveBox(class BoxComponent* box);

	bool SegmentCast(const LineSegment& l, Physics::CollisionInfo& outInfo);
	void TestSweepAndPrune(std::function<void(Actor*, Actor*)> f);
private:
	GameContext* mContext;
	std::vector<class BoxComponent*> mBoxes;
};