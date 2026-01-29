#include <vector>
#pragma once
#include "Math.h"

struct Spline
{
	std::vector<Vector3> mControlPoints;

	Vector3 Compute(size_t startIdx, float t) const;
	size_t GetNumPoints() const { return mControlPoints.size(); }
};