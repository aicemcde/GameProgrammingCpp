#pragma once
#include "Component.h"
#include "../util/Math.h"

class CircleComponent : public ComponentBase<CircleComponent>
{
public:
	CircleComponent(class Actor* owner);

	void SetRadius(float radius) { mRadius = radius; }
	float GetRadius() const;
private:
	float mRadius;
};

bool Intersect(const CircleComponent& a, const CircleComponent& b);