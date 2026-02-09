#pragma once
#include <vector>
#include "../util/PhysicsTypes.h"
#include "../util/Collision.h"
#include <functional>

struct GameContext;
class BoxComponent;

class PhysWorld
{
public:
	explicit PhysWorld(GameContext* context);
	~PhysWorld();

	void AddBox(class BoxComponent* box);
	void RemoveBox(class BoxComponent* box);

	bool SegmentCast(const LineSegment& l, Physics::CollisionInfo& outInfo);
	void TestSweepAndPrune(std::function<void(BoxComponent*, BoxComponent*)> f);
private:
	GameContext* mContext;
	std::vector<class BoxComponent*> mBoxes;
};