#pragma once
#include "Math.h"


class BoxComponent;
class Actor;

namespace Physics
{
	struct CollisionInfo
	{
		Vector3 mPoint;
		Vector3 mNormal;
		BoxComponent* mBox;
		Actor* mOwner;
	};
}
